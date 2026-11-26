from database import base
from sqlalchemy import Boolean, Column, String, Integer, ForeignKey, DateTime, Float, func
from sqlalchemy.orm import relationship
from sqlalchemy import CheckConstraint, UniqueConstraint
from sqlalchemy import Uuid
from uuid import uuid4

class Layer(base):
    __tablename__ = "Layer"

    __table_args__ = (
        CheckConstraint("position >= 0", name="ck_layer_position"),
        CheckConstraint(
            "opacity BETWEEN 0 AND 1",
            name="ck_layer_opacity"
        ),
        UniqueConstraint(
            "frame_id",
            "position",
            name="uq_layer_frame_position"
        ),
    )

    id = Column(
        Uuid(as_uuid=True),
        primary_key=True,
        default=uuid4
    )
    frame_id = Column(
        Uuid(as_uuid=True),
        ForeignKey("Frame.id"),
        nullable=False,
        index=True
    )
    name = Column(String)
    position = Column(Integer, nullable=False)
    visible = Column(Boolean, nullable=False, default=True)
    opacity = Column(Float, nullable=False, default=1.0)

    storage_key = Column(String)


    frame = relationship("Frame", back_populates="layers")