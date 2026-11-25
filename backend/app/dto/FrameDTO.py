from pydantic import BaseModel, ConfigDict, Field

class FrameCreate(BaseModel):
    position: int = Field(ge=0)


class FrameReorder(BaseModel):
    model_config = ConfigDict(extra="forbid")

    frame_ids: list[int]


class FrameResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    id: int
    project_id: int
    position: int