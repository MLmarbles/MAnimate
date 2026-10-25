from sqlalchemy import Column, Integer, String

from database import base

class User(base):
    __tablename__ = "User"

    id = Column(Integer, primary_key=True)
    username = Column(String)
    email = Column(String, unique=True)
    password = Column(String)