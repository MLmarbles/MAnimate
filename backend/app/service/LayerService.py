from sqlalchemy.orm import Session
from dto.LayerDto import LayerUpdate
from models.User import User
from repository import LayerRepository
from service import ProjectService, StorageService


class LayerNotFound(Exception):
    pass

class InvalidLayerOrder(Exception):
    pass

class ContentNotFound(Exception):
    pass

def create(db: Session, project_id: int, name: str, user: User):
    try:
        project = ProjectService.get(db, project_id, user)
        layer = LayerRepository.create_layer(
            db=db, project_id=project.id, name=name
        )
        db.commit()
    except Exception:
        db.rollback()
        raise

    db.refresh(layer)
    return layer


def get(db: Session, project_id: int, layer_id: int, user: User):
    ProjectService.get(db, project_id, user)
    layer = LayerRepository.get_layer(db, project_id, layer_id)
    if layer is None:
        raise LayerNotFound()
    return layer


def list_layers(db: Session, project_id: int, user: User):
    ProjectService.get(db, project_id, user)
    return LayerRepository.list_layers(db, project_id)


def reorder(db: Session, project_id: int, layer_ids: list[int], user: User):
    try:
        ProjectService.get(db, project_id, user)
        layers = LayerRepository.list_layers(db, project_id)
        layers_by_id = {layer.id: layer for layer in layers}
        if len(layer_ids) != len(layers) or set(layer_ids) != set(layers_by_id):
            raise InvalidLayerOrder()

        ordered_layers = [layers_by_id[layer_id] for layer_id in layer_ids]
        LayerRepository.reorder_layers(db, ordered_layers)
        db.commit()
    except Exception:
        db.rollback()
        raise

    return ordered_layers


def update(db: Session, project_id: int, layer_id: int, changes: LayerUpdate, user: User):
    try:
        layer = get(db, project_id, layer_id, user)
        LayerRepository.update_layer(db, layer, changes.model_dump(exclude_unset=True))
        db.commit()
    except Exception:
        db.rollback()
        raise

    db.refresh(layer)
    return layer


def delete(db: Session, project_id: int, layer_id: int, user: User):
    storage_key = None
    try:
        layer = get(db, project_id, layer_id, user)
        storage_key = layer.storage_key
        LayerRepository.delete_layer(db, layer)
        db.commit()
    except Exception:
        db.rollback()
        raise

    if storage_key is not None:
        try:
            StorageService.delete_layer(storage_key)
        except Exception:
            # cleanup later
            pass

def save_content(
    db: Session,
    project_id: int,
    layer_id: int,
    data: bytes,
    user: User,
):
    layer = get(db, project_id, layer_id, user)
    old_storage_key = layer.storage_key
    new_storage_key = None

    try:
        new_storage_key = StorageService.save_layer(project_id, layer_id, data)
        LayerRepository.update_layer(db, layer, {"storage_key": new_storage_key})
        db.commit()
    except Exception:
        db.rollback()
        if new_storage_key is not None:
            StorageService.delete_layer(new_storage_key)
        raise

    if old_storage_key is not None:
        try:
            StorageService.delete_layer(old_storage_key)
        except Exception:
            # cleanup later
            pass

def get_content(
    db: Session,
    project_id: int,
    layer_id: int,
    user: User,
):
    layer = get(db, project_id, layer_id, user)

    if layer.storage_key is None:
        raise ContentNotFound()

    return StorageService.get_layer(layer.storage_key)
