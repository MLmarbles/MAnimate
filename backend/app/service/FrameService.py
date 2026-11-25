from repository import FrameRepository
from sqlalchemy.orm import Session
from models.User import User
from models.Layer import Layer
from service import ProjectService, StorageService


class FrameNotFound(Exception):
    pass


class InvalidFrameOrder(Exception):
    pass


class LastFrame(Exception):
    pass

def create(db: Session, project_id: int, user: User):
    try:
        project = ProjectService.get(db, project_id, user)

        max_position = FrameRepository.get_max_position(
            db, project.id
        )

        position = 0 if max_position is None else max_position + 1

        frame = FrameRepository.create_frame(
            db=db,
            project_id=project.id,
            position=position
        )

        db.add(Layer(
            frame_id=frame.id,
            name="Layer 1",
            position=0
        ))

        db.commit()

    except Exception:
        db.rollback()
        raise

    db.refresh(frame)
    return frame


def get(db: Session, project_id: int, frame_id: int, user: User):
    ProjectService.get(db, project_id, user)
    frame = FrameRepository.get_frame(db, project_id, frame_id)
    if frame is None:
        raise FrameNotFound()
    return frame


def list_frames(db: Session, project_id: int, user: User):
    ProjectService.get(db, project_id, user)
    return FrameRepository.get_frames(db, project_id)


def reorder(db: Session, project_id: int, frame_ids: list[int], user: User):
    try:
        ProjectService.get(db, project_id, user)
        frames = FrameRepository.get_frames(db, project_id)
        frames_by_id = {frame.id: frame for frame in frames}
        if len(frame_ids) != len(frames) or set(frame_ids) != set(frames_by_id):
            raise InvalidFrameOrder()

        ordered_frames = [frames_by_id[frame_id] for frame_id in frame_ids]
        FrameRepository.reorder_frames(db, ordered_frames)
        db.commit()
    except Exception:
        db.rollback()
        raise

    return ordered_frames


def delete(db: Session, project_id: int, frame_id: int, user: User):
    storage_keys = []
    try:
        frame = get(db, project_id, frame_id, user)
        if FrameRepository.count_frames(db, project_id) <= 1:
            raise LastFrame()

        storage_keys = [layer.storage_key for layer in frame.layers if layer.storage_key]
        FrameRepository.delete_frame(db, frame)
        frames = FrameRepository.get_frames(db, project_id)
        FrameRepository.reorder_frames(db, frames)
        db.commit()
    except Exception:
        db.rollback()
        raise

    for storage_key in storage_keys:
        try:
            StorageService.delete_layer(storage_key)
        except Exception:
            pass

