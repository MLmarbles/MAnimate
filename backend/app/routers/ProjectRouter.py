from fastapi import APIRouter, HTTPException, Response, status, Depends
from dto import ProjectDTO
from sqlalchemy.orm import Session
from database import get_db
from models.User import User
from security import get_current_user
from service import ProjectService

router = APIRouter(
    prefix="/project",
    tags=["Project"]
)

@router.post(
    "/create",
    status_code=status.HTTP_201_CREATED,
    response_model=ProjectDTO.ProjectResponse,
)
def create(
    project: ProjectDTO.ProjectCreate,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    return ProjectService.create(db, project.name, current_user)

@router.get(
    "/{project_id}",
    response_model=ProjectDTO.ProjectResponse,
)
def get_project(
    project_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    try:
        return ProjectService.get(db, project_id, current_user)
    except ProjectService.ProjectNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Project not found",
        ) from None

@router.patch(
    "/{project_id}",
    response_model=ProjectDTO.ProjectResponse,
)
def update_project(
    project_id: int,
    project: ProjectDTO.ProjectUpdate,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db),
):
    try:
        return ProjectService.update(db, project_id, project.name, current_user)
    except ProjectService.ProjectNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Project not found",
        ) from None


@router.delete(
    "/{project_id}",
    status_code=status.HTTP_204_NO_CONTENT,
    response_class=Response,
)
def delete_project(
    project_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db),
):
    try:
        ProjectService.delete(db, project_id, current_user)
        return Response(status_code=status.HTTP_204_NO_CONTENT)
    except ProjectService.ProjectNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Project not found",
        ) from None
