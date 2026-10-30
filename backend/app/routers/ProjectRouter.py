from fastapi import APIRouter, HTTPException, Response, status, Depends
from dto import ProjectDTO, LayerDto
from sqlalchemy.orm import Session
from database import get_db
from models.User import User
from security import get_current_user
from service import ProjectService, LayerService

router = APIRouter(
    prefix="/project",
    tags=["Project"]
)

@router.post(
    "/create",
    status_code=status.HTTP_201_CREATED,
    response_model=ProjectDTO.ProjectResponse,
)
def create_project(
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


@router.post(
    "/{project_id}/layers",
    status_code=status.HTTP_201_CREATED,
    response_model=LayerDto.LayerResponse,
)
def create_layer(
    project_id: int,
    layer: LayerDto.LayerCreate,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    try:
        return LayerService.create(db, project_id, layer.name, current_user)
    except ProjectService.ProjectNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Project not found",
        ) from None


@router.get(
    "/{project_id}/layers",
    response_model=list[LayerDto.LayerResponse],
)
def list_layers(
    project_id: int,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    try:
        return LayerService.list_layers(db, project_id, current_user)
    except ProjectService.ProjectNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Project not found",
        ) from None


@router.get(
    "/{project_id}/layers/{layer_id}",
    response_model=LayerDto.LayerResponse,
)
def get_layer(
    project_id: int,
    layer_id: int,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    try:
        return LayerService.get(db, project_id, layer_id, current_user)
    except ProjectService.ProjectNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Project not found",
        ) from None
    except LayerService.LayerNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Layer not found",
        ) from None


@router.patch(
    "/{project_id}/layers/{layer_id}",
    response_model=LayerDto.LayerResponse,
)
def update_layer(
    project_id: int,
    layer_id: int,
    layer: LayerDto.LayerUpdate,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    try:
        return LayerService.update(db, project_id, layer_id, layer, current_user)
    except ProjectService.ProjectNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Project not found",
        ) from None
    except LayerService.LayerNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Layer not found",
        ) from None


@router.delete(
    "/{project_id}/layers/{layer_id}",
    status_code=status.HTTP_204_NO_CONTENT,
    response_class=Response,
)
def delete_layer(
    project_id: int,
    layer_id: int,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    try:
        LayerService.delete(db, project_id, layer_id, current_user)
        return Response(status_code=status.HTTP_204_NO_CONTENT)
    except ProjectService.ProjectNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Project not found",
        ) from None
    except LayerService.LayerNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Layer not found",
        ) from None
