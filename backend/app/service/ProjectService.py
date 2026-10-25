from sqlalchemy.orm import Session
from models.User import User
from repository import ProjectRepository
from dto.ProjectDTO import ProjectUpdate

class ProjectNotFound(Exception):
    pass

def create(db: Session, name: str, user: User):
    try: 
        project = ProjectRepository.create_project(
            db=db,
            name=name,
            id=user.id
        )
        db.commit()
    except Exception:
        db.rollback()
        raise

    db.refresh(project)
    return project

def update(db: Session, project_id: int, name: str, user: User):
    try:
        project = get(db, project_id, user)
        ProjectRepository.update_project(db, project, name)
        db.commit()
    except Exception:
        db.rollback()
        raise
    
    db.refresh(project)
    return project

def delete(db: Session, project_id: int, user: User):
    try:
        project = get(db, project_id, user)
        ProjectRepository.delete_project(db, project)
        db.commit()
    except Exception:
        db.rollback()
        raise


def get(db: Session, project_id: int, user: User):
    project = ProjectRepository.get_project(db, project_id, user.id)

    if project is None:
        raise ProjectNotFound()

    return project
