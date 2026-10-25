from fastapi import FastAPI

import models
from database import engine, session, base

base.metadata.create_all(bind=engine)
app = FastAPI()

def get_db():
    db = session()

    try:
        yield db
    finally:
        db.close()

@app.get("/")
def home():
    return {"message": "hello cruel world"}

