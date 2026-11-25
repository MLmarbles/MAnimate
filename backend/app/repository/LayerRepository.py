from sqlalchemy.orm import Session
from sqlalchemy import func
from models.Layer import Layer


def create_layer(db: Session, frame_id: int, name: str):
    last_position = (
        db.query(func.max(Layer.position))
        .filter(Layer.frame_id == frame_id)
        .scalar()
    )
    layer = Layer(
        frame_id=frame_id,
        name=name,
        position=0 if last_position is None else last_position + 1,
    )
    db.add(layer)
    db.flush()
    return layer


def get_layer(db: Session, frame_id: int, layer_id: int):
    return (
        db.query(Layer)
        .filter(Layer.frame_id == frame_id, Layer.id == layer_id)
        .first()
    )


def list_layers(db: Session, frame_id: int):
    return (
        db.query(Layer)
        .filter(Layer.frame_id == frame_id)
        .order_by(Layer.position, Layer.id)
        .all()
    )


def reorder_layers(db: Session, layers: list[Layer]):
    offset = max((layer.position for layer in layers), default=-1) + 1
    for position, layer in enumerate(layers):
        layer.position = offset + position
    db.flush()

    for position, layer in enumerate(layers):
        layer.position = position
    db.flush()


def update_layer(db: Session, layer: Layer, changes: dict):
    for field, value in changes.items():
        setattr(layer, field, value)
    db.flush()
    return layer


def delete_layer(db: Session, layer: Layer):
    db.delete(layer)
    db.flush()

