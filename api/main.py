from fastapi import FastAPI
import uvicorn
from fastapi.staticfiles import StaticFiles

app = FastAPI()

# Serve images from the "static/images" directory
app.mount("/images", StaticFiles(directory="static/images"), name="images")

if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8000)
