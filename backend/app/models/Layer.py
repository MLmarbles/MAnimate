from database import base
from sqlalchemy import Boolean, Column, String, Integer, ForeignKey, DateTime, Float, func
from sqlalchemy.orm import relationship

class Layer(base):
    __tablename__ = "Layer"

    id = Column(Integer, primary_key=True)
    project_id = Column(
        Integer,
        ForeignKey("Project.id"),
        nullable=False,
        index=True
    )
    name = Column(String)
    position = Column(Integer, nullable=False)
    visible = Column(Boolean, nullable=False, default=True)
    opacity = Column(Float, nullable=False, default=1.0)

    storage_key = Column(String)

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

    owner = relationship("Project", back_populates="layers")