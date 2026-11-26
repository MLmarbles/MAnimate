from pydantic import BaseModel, ConfigDict, Field
from dto.LayerDTO import LayerResponse
from uuid import UUID

class FrameReorder(BaseModel):
    model_config = ConfigDict(extra="forbid")

    frame_ids: list[UUID]


class FrameResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    id: UUID
    project_id: int
    position: int

class FrameDetailResponse(FrameResponse):
    layers: list[LayerResponse]
