from uuid import UUID

from fastapi import APIRouter, Depends, HTTPException, Response, status
from dto import FrameDTO
from security import get_current_user
from database import get_db
from models.User import User
from sqlalchemy.orm import Session
from service import FrameService, ProjectService

router = APIRouter(
    prefix="/project/{project_id}/frames",
    tags=["Frame"]
)

@router.post(
    "",
    status_code=status.HTTP_201_CREATED,
    response_model=FrameDTO.FrameDetailResponse
)
def create_frame(
    project_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    try:
        return FrameService.create(db, project_id, current_user)
    except ProjectService.ProjectNotFound:
        raise HTTPException(status_code=404, detail="Project not found") from None


@router.get("", response_model=list[FrameDTO.FrameResponse])
def list_frames(
    project_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db),
):
    try:
        return FrameService.list_frames(db, project_id, current_user)
    except ProjectService.ProjectNotFound:
        raise HTTPException(status_code=404, detail="Project not found") from None


@router.put("/reorder", response_model=list[FrameDTO.FrameResponse])
def reorder_frames(
    project_id: int,
    order: FrameDTO.FrameReorder,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db),
):
    try:
        return FrameService.reorder(db, project_id, order.frame_ids, current_user)
    except ProjectService.ProjectNotFound:
        raise HTTPException(status_code=404, detail="Project not found") from None
    except FrameService.InvalidFrameOrder:
        raise HTTPException(
            status_code=400,
            detail="frame_ids must contain every frame in the project exactly once",
        ) from None


@router.get("/{frame_id}", response_model=FrameDTO.FrameDetailResponse)
def get_frame(
    project_id: int,
    frame_id: UUID,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db),
):
    try:
        return FrameService.get(db, project_id, frame_id, current_user)
    except ProjectService.ProjectNotFound:
        raise HTTPException(status_code=404, detail="Project not found") from None
    except FrameService.FrameNotFound:
        raise HTTPException(status_code=404, detail="Frame not found") from None


@router.delete("/{frame_id}", status_code=status.HTTP_204_NO_CONTENT, response_class=Response)
def delete_frame(
    project_id: int,
    frame_id: UUID,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db),
):
    try:
        FrameService.delete(db, project_id, frame_id, current_user)
        return Response(status_code=status.HTTP_204_NO_CONTENT)
    except ProjectService.ProjectNotFound:
        raise HTTPException(status_code=404, detail="Project not found") from None
    except FrameService.FrameNotFound:
        raise HTTPException(status_code=404, detail="Frame not found") from None
    except FrameService.LastFrame:
        raise HTTPException(status_code=409, detail="Cannot delete the last frame") from None
