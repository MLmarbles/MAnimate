from database import base
from sqlalchemy import Column, Integer, ForeignKey, String, DateTime

class RefreshToken(base):
    __tablename__ = "RefreshToken"

    id = Column(Integer, primary_key=True)
    user_id = Column(
        Integer,
        ForeignKey("User.id"), 
        nullable=False
        )
    token_hash = Column(
        String,
        nullable=False,
        unique=True
        )
    expires_at = Column(
        DateTime(timezone=True),
        nullable=False
        )
    revoked_at = Column(
        DateTime(timezone=True),
        nullable=True
        )
    family_id = Column(String, nullable=False)

