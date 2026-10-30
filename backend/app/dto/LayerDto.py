from datetime import datetime
from pydantic import BaseModel, ConfigDict, Field, field_validator

class LayerCreate(BaseModel):
    name: str


class LayerUpdate(BaseModel):
    model_config = ConfigDict(extra="forbid")

    name: str | None = None
    position: int | None = Field(default=None, ge=0)
    visible: bool | None = None
    opacity: float | None = Field(default=None, ge=0, le=1, allow_inf_nan=False)
    storage_key: str | None = None

    @field_validator("name", "position", "visible", "opacity")
    @classmethod
    def reject_null(cls, value):
        if value is None:
            raise ValueError("Field cannot be null")
        return value


class LayerResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    id: int
    project_id: int
    name: str | None
    position: int
    visible: bool
    opacity: float
    storage_key: str | None
    created_at: datetime
    updated_at: datetime
