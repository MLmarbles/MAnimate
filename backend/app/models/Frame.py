from database import base
from sqlalchemy import Column, Integer, ForeignKey
from sqlalchemy.orm import relationship
from sqlalchemy import CheckConstraint, UniqueConstraint

class Frame(base):
    __tablename__ = "Frame"

    __table_args__ = (
        CheckConstraint("position >= 0", name="ck_frame_position"),
        UniqueConstraint(
            "project_id",
            "position",
            name="uq_frame_project_position"
        ),
    )

    id = Column(Integer, primary_key = True)
    project_id = Column(
        Integer,
        ForeignKey("Project.id"),
        nullable=False,
        index=True
    )
    position = Column(Integer, nullable=False)


    project = relationship("Project", back_populates="frames")
    layers = relationship("Layer", back_populates="frame", cascade="all, delete-orphan")
