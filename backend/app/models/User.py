from sqlalchemy import Column, Integer, String
from sqlalchemy.orm import relationship
from database import base

class User(base):
    __tablename__ = "User"

    id = Column(Integer, primary_key=True)
    username = Column(String, unique=True)
    email = Column(String, unique=True)
    password = Column(String)
    projects = relationship("Project", back_populates="owner")