from sqlalchemy import Column, String, Integer, DateTime, ForeignKey, func
from database import base

class Project(base):
    __tablename__ = "Project"

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