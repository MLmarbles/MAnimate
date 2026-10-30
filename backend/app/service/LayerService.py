from sqlalchemy.orm import Session
from dto.LayerDto import LayerUpdate
from models.User import User
from repository import LayerRepository
from service import ProjectService


class LayerNotFound(Exception):
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
    try:
        layer = get(db, project_id, layer_id, user)
        LayerRepository.delete_layer(db, layer)
        db.commit()
    except Exception:
        db.rollback()
        raise
