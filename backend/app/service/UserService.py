from sqlalchemy.orm import Session
from dto import UserDTO
from repository import UserRepository, RefreshTokenRepository
from sqlalchemy.exc import IntegrityError
import security
from datetime import datetime, timedelta, timezone
import uuid

class AlreadyRegistered(Exception):
    pass

class InvalidCreds(Exception):
    pass

class InvalidRefreshToken(Exception):
    pass

def sign_up(db: Session, user: UserDTO.UserCreate):
    email = user.email
    username = user.username
    if (UserRepository.get_user_by_email(db, email) or
        UserRepository.get_user_by_username(db, username)):
        raise AlreadyRegistered

    hashed = security.hash_password(user.password)

    try:
        user = UserRepository.create_user(
            db=db,
            username=username,
            email=email,
            password=hashed
        )
        db.commit()
    except IntegrityError:
        db.rollback()
        raise AlreadyRegistered

    db.refresh(user)

    access_token = security.create_token(
        data={"sub": str(user.id)}
    )

    return UserDTO.Token(
        access_token=access_token,
        token_type="bearer"
    )

def login(db: Session, user: UserDTO.UserLogin):
    username = user.username

    userFound = UserRepository.get_user_by_username(db, username)

    if userFound is None:
        raise InvalidCreds()

    if not security.verify_password(user.password, userFound.password):
        raise InvalidCreds()

    access_token = security.create_token(
        data={"sub": str(userFound.id)}
    )

    refresh_token = security.generate_refresh_token()
    hashed_refresh = security.hash_refresh_token(refresh_token)
    expires_at = datetime.now(timezone.utc) + timedelta(days=30)
    family_id = str(uuid.uuid4())

    try:
        refresh = RefreshTokenRepository.create_refresh_token(
            db,
            userFound.id,
            hashed_refresh,
            expires_at,
            family_id
        )
        db.commit()
    except Exception:
        db.rollback()
        raise
    
    db.refresh(refresh)

    return (refresh_token, UserDTO.Token(
        access_token=access_token,
        token_type="bearer"
    ))

def refresh_session(db: Session, raw_token: str):
    hash_refresh = security.hash_refresh_token(raw_token)

    refresh = RefreshTokenRepository.get_by_token_hash(db, hash_refresh)

    if refresh is None:
        raise InvalidRefreshToken

    if refresh.revoked_at is not None:
        raise InvalidRefreshToken()

    if refresh.expires_at <= datetime.now(timezone.utc):
        raise InvalidRefreshToken()

    userFound = UserRepository.get_user_by_id(db, refresh.user_id)

    if userFound is None:
        raise InvalidRefreshToken()

    refresh_token = security.generate_refresh_token()
    hashed_refresh = security.hash_refresh_token(refresh_token)
    expires_at = refresh.expires_at

    try:
        RefreshTokenRepository.update_revoke(db, refresh, datetime.now(timezone.utc))

        new_refresh = RefreshTokenRepository.create_refresh_token(
            db,
            userFound.id,
            hashed_refresh,
            expires_at,
            refresh.family_id
        )

        access_token = security.create_token(
            data={"sub": str(userFound.id)}
        )

        db.commit()
    except Exception:
        db.rollback()
        raise

    db.refresh(refresh)
    db.refresh(new_refresh)

    return (refresh_token, UserDTO.Token(
        access_token=access_token,
        token_type="bearer"
    ))