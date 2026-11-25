from datetime import datetime
from pydantic import BaseModel, ConfigDict, Field, field_validator

class ProjectCreate(BaseModel):
    name: str
    width: int = Field(default=800, gt=0)
    height: int = Field(default=600, gt=0)
    fps: int = Field(default=12, ge=1, le=60)

class ProjectUpdate(BaseModel):
    model_config = ConfigDict(extra="forbid")

    name: str | None = None
    fps: int | None = Field(default=None, ge=1, le=60)

    @field_validator("name", "fps")
    @classmethod
    def reject_null(cls, value):
        if value is None:
            raise ValueError("Field cannot be null")
        return value

class ProjectResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)
    id: int
    name: str | None
    owner_id: int
    created_at: datetime
    updated_at: datetime
    width: int
    height: int
    fps: int
