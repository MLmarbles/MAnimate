from fastapi import APIRouter, status, Depends, HTTPException, Response, Cookie
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
    response: Response,
    db: Session = Depends(get_db)
):
    try:
        refresh, jwt = UserService.login(db, user)

        response.set_cookie(
            key="refresh_token",
            value=refresh,
            httponly=True,
            secure=False,  #http local dev, change later for https
            samesite="lax",
            max_age=30 * 24 * 60 * 60,
            path="/users",
        )

        return jwt
    
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


@router.post(
    "/refresh",
    response_model=UserDTO.Token
)
def refresh(
    response: Response,
    refresh_token: str | None = Cookie(default=None),
    db: Session = Depends(get_db),
):
    if not refresh_token:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Missing refresh token",
        )

    try:
        new_refresh_token, token_response = UserService.refresh_session(db, refresh_token)
    except UserService.InvalidRefreshToken:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Invalid or expired refresh token",
        )

    response.set_cookie(
        key="refresh_token",
        value=new_refresh_token,
        httponly=True,
        secure=False,  #change
        samesite="lax",
        path="/users",
    )

    return token_response
