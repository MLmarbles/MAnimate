from sqlalchemy.orm import Session
from models.Project import Project


def create_project(db: Session, name: str, id: int):
    project = Project(
        name=name,
        owner_id=id,
    )

    db.add(project)
    db.flush()
    return project

def update_project(db: Session, project: Project, name: str):
    project.name = name
    db.flush()
    return project

def delete_project(db: Session, project: Project):
    db.delete(project)
    db.flush()


def get_project(db: Session, project_id: int, owner_id: int):
    return (
        db.query(Project)
        .filter(
            Project.id == project_id,
            Project.owner_id == owner_id,
        )
        .first()
    )
