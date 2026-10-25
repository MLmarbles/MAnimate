from fastapi import APIRouter, status, Depends, HTTPException
from dto import UserDTO
from sqlalchemy.orm import Session
from database import get_db
from service import UserService
from models.User import User
from security import get_current_user

router = APIRouter(
    prefix="/users",
    tags=["Users"]
)

@router.post(
        "/signup",
        status_code=status.HTTP_201_CREATED,
        response_model=UserDTO.Token
)
def sign_up(
    user: UserDTO.UserCreate,
    db: Session = Depends(get_db)
):
    try:
        return UserService.sign_up(db, user)
    
    except UserService.AlreadyRegistered:
        raise HTTPException(
            status_code=status.HTTP_409_CONFLICT,
            detail="email already registered",
        )

@router.post(
    "/login",
    response_model=UserDTO.Token
)
def login(
    user: UserDTO.UserLogin,
    db: Session = Depends(get_db)
):
    try:
        return UserService.login(db, user)
    
    except UserService.InvalidCreds:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="incorrect username or password"
        )

@router.get(
    "/me"
)
def me(
    current_user: User = Depends(get_current_user)
):
    return current_user