from fastapi import APIRouter, HTTPException, Response, UploadFile, status, Depends
from fastapi.params import File
from dto import LayerDto
from sqlalchemy.orm import Session
from database import get_db
from models.User import User
from security import get_current_user
from service import ProjectService, LayerService

router = APIRouter(
    prefix="/project/{project_id}/layers",
    tags=["Layer"]
)

@router.post(
    "",
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
    "",
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


@router.put(
    "/reorder",
    response_model=list[LayerDto.LayerResponse],
)
def reorder_layers(
    project_id: int,
    order: LayerDto.LayerReorder,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    try:
        return LayerService.reorder(db, project_id, order.layer_ids, current_user)
    except ProjectService.ProjectNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Project not found",
        ) from None
    except LayerService.InvalidLayerOrder:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="layer_ids must contain every layer in the project exactly once",
        ) from None


@router.get(
    "/{layer_id}",
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
    "/{layer_id}",
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
    "/{layer_id}",
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

@router.put("/{layer_id}/content")
async def upload_layer_content(
    project_id: int,
    layer_id: int,
    file: UploadFile = File(...),
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db),
):
    data = await file.read()

    try:
        LayerService.save_content(
            db=db,
            project_id=project_id,
            layer_id=layer_id,
            data=data,
            user=current_user,
        )
    except LayerService.LayerNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Layer not found",
        ) from None

@router.get("/{layer_id}/content")
async def get_layer_content(
    project_id: int,
    layer_id: int,
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db),
):
    try:
        data = LayerService.get_content(
            db=db,
            project_id=project_id,
            layer_id=layer_id,
            user=current_user,
        )

        return Response(
            content=data,
            media_type="image/png",
        )

    except LayerService.LayerNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Layer not found",
        ) from None

    except LayerService.ContentNotFound:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Layer content not found",
        ) from None
