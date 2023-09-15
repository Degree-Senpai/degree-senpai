from fastapi import FastAPI, Body, Response
from fastapi.middleware.cors import CORSMiddleware
from tortoise import Tortoise

from src.database.register import register_tortoise
from src.database.config import TORTOISE_ORM


Tortoise.init_models(["src.database.models"], "models")

from src.routes import users, notes

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:8080"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)
app.include_router(users.router)
app.include_router(notes.router)

register_tortoise(app, config=TORTOISE_ORM, generate_schemas=False)


@app.post("/api/newuser")
async def createuser(userid:str = Body(...), region:str = Body(...)):
    print('creating user')

    print('created user')

@app.get("/")
async def root():
    print('hi')
    return {"message": "message"}

@app.post("/api/setregion")
async def setregion(userid:str = Body(...), region:str = Body(...)):
    print(f'set user {userid} region to {region}')
    return Response(content='success', status_code=200)
