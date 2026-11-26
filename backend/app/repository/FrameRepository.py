from uuid import UUID

from sqlalchemy import func
from sqlalchemy.orm import Session
from models.Frame import Frame


def create_frame(
    db: Session,
    project_id: int,
    position: int,
    frame_id: UUID | None = None,
):
    frame = Frame(
        project_id=project_id,
        position=position
    )

    if frame_id is not None:
        frame.id = frame_id

    db.add(frame)
    db.flush()
    return frame

def get_frame(db: Session, project_id: int, frame_id: UUID):
    return (
        db.query(Frame)
        .filter(
            Frame.id == frame_id,
            Frame.project_id == project_id
        )
        .first()
    )

def get_frames(db: Session, project_id: int):
    return (
        db.query(Frame)
        .filter(Frame.project_id == project_id)
        .order_by(Frame.position.asc())
        .all()
    )

def get_max_position(db: Session, project_id: int):
    return (
        db.query(func.max(Frame.position))
        .filter(Frame.project_id == project_id)
        .scalar()
    )

def delete_frame(db: Session, frame: Frame):
    db.delete(frame)
    db.flush()

def count_frames(db: Session, project_id: int):
    return (
        db.query(func.count(Frame.id))
        .filter(Frame.project_id == project_id)
        .scalar()
    )


def reorder_frames(db: Session, frames: list[Frame]):
    offset = max((frame.position for frame in frames), default=-1) + 1
    for position, frame in enumerate(frames):
        frame.position = offset + position
    db.flush()

    for position, frame in enumerate(frames):
        frame.position = position
    db.flush()
