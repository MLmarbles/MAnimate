from datetime import datetime

from sqlalchemy.orm import Session
from models.RefreshToken import RefreshToken

def create_refresh_token(
    db: Session,
    user_id: int,
    token_hash: str,
    expires_at: datetime,
    family_id: str
):
    token = RefreshToken(
        user_id=user_id,
        token_hash=token_hash,
        expires_at=expires_at,
        family_id=family_id
    )

    db.add(token)
    db.flush()
    return token

def get_by_token_hash(db: Session, token_hash: str):
    return db.query(RefreshToken).filter(RefreshToken.token_hash==token_hash).first()

def update_revoke(db: Session, refresh_token: RefreshToken, revoked_at: datetime):
    refresh_token.revoked_at = revoked_at
    db.flush()
    return refresh_token