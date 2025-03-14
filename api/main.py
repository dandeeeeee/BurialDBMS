from fastapi import FastAPI
import uvicorn
from fastapi.staticfiles import StaticFiles

app = FastAPI()


app.mount("/images", StaticFiles(directory="static/images"), name="images")

app.mount("/fonts", StaticFiles(directory="static/fonts"), name="fonts")

if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8000)
