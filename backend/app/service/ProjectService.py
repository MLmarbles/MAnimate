from sqlalchemy.orm import Session
from models.User import User
from repository import ProjectRepository, FrameRepository, LayerRepository
from service import StorageService

class ProjectNotFound(Exception):
    pass

def create(db: Session, name: str, width: int, height: int, fps: int, user: User):
    try: 
        project = ProjectRepository.create_project(
            db=db,
            name=name,
            width=width,
            height=height,
            fps=fps,
            id=user.id
        )
        frame = FrameRepository.create_frame(db, project.id, position=0)
        LayerRepository.create_layer(db, frame.id, name="Layer 1")
        db.commit()
    except Exception:
        db.rollback()
        raise

    db.refresh(project)
    return project

def update(db: Session, project_id: int, name: str, fps: int, user: User):
    try:
        project = get(db, project_id, user)
        ProjectRepository.update_project(db, project, name, fps)
        db.commit()
    except Exception:
        db.rollback()
        raise
    
    db.refresh(project)
    return project

def delete(db: Session, project_id: int, user: User):
    try:
        project = get(db, project_id, user)
        storage_keys = [
            layer.storage_key
            for frame in project.frames
            for layer in frame.layers
            if layer.storage_key is not None
        ]
        ProjectRepository.delete_project(db, project)
        db.commit()
    except Exception:
        db.rollback()
        raise

    for storage_key in storage_keys:
        try:
            StorageService.delete_layer(storage_key)
        except Exception:
            # Match layer deletion: a cleanup failure cannot undo the committed delete.
            pass


def get(db: Session, project_id: int, user: User):
    project = ProjectRepository.get_project(db, project_id, user.id)

    if project is None:
        raise ProjectNotFound()

    return project
