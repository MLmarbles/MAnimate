from pydantic import BaseModel, ConfigDict, Field
from dto.LayerDTO import LayerResponse

class FrameReorder(BaseModel):
    model_config = ConfigDict(extra="forbid")

    frame_ids: list[int]


class FrameResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    id: int
    project_id: int
    position: int

class FrameDetailResponse(FrameResponse):
    layers: list[LayerResponse]
