from sqlalchemy.orm import Session
from models.User import User

def get_user_by_email(db: Session, email: str):
    return db.query(User).filter(User.email == email).first()

def get_user_by_username(db: Session, username: str):
    return db.query(User).filter(User.username == username).first()

def get_user_by_id(db: Session, id: int):
    return db.query(User).filter(User.id == id).first()

def create_user(db: Session, username: str, email: str, password: str):
    user = User(
        username=username,
        email=email,
        password=password
    )

    db.add(user)
    db.flush()
    return user