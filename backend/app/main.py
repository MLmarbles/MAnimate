from fastapi import FastAPI
from database import engine, base
from routers.UserRouter import router as user_router

base.metadata.create_all(bind=engine)
app = FastAPI()
app.include_router(user_router)

@app.get("/")
def home():
    return {"message": "hello cruel world"}

