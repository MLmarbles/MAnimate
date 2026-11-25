from sqlalchemy import Column, String, Integer, DateTime, ForeignKey, func
from database import base
from sqlalchemy.orm import relationship
from sqlalchemy import CheckConstraint, UniqueConstraint

class Project(base):
    __tablename__ = "Project"

    __table_args__ = (
        CheckConstraint("width > 0", name="ck_project_width"),
        CheckConstraint("height > 0", name="ck_project_height"),
        CheckConstraint("fps BETWEEN 1 AND 60", name="ck_project_fps"),
    )

    id = Column(Integer, primary_key=True)
    name = Column(String)
    owner_id = Column(
        Integer,
        ForeignKey("User.id"),
        nullable=False,
        index=True
        )
    created_at = Column(
        DateTime(timezone=True),
        server_default=func.now(),
        nullable=False
        )
    updated_at = Column(
        DateTime(timezone=True),
        server_default=func.now(),
        onupdate=func.now(),
        nullable=False
        )

    width = Column(Integer, nullable=False, default=800)
    height = Column(Integer, nullable=False, default=600)
    fps = Column(Integer, nullable=False, default=12)


    owner = relationship("User", back_populates="projects")
    frames = relationship(
        "Frame",
        back_populates="project",
        cascade="all, delete-orphan",
        order_by="Frame.position"
    )