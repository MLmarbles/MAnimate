from sqlalchemy.orm import Session
from dto import UserDTO
from repository import UserRepository
from sqlalchemy.exc import IntegrityError
import security

class AlreadyRegistered(Exception):
    pass

class InvalidCreds(Exception):
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

    return UserDTO.Token(
        access_token=access_token,
        token_type="bearer"
    )
    