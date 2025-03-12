from fastapi import FastAPI
import uvicorn
from pydantic import BaseModel
from typing import List, Optional
import json
import os

app = FastAPI()

DATA_FILE = "temporaryDB/graveplots.json"

# Model for grave plots
class GravePlot(BaseModel):
    ID: int
    status: str
    occupantName: Optional[str] = ""
    positionX: int
    positionY: int
    width: int
    height: int


# Load existing data from file
def load_data():
    if os.path.exists(DATA_FILE):
        with open(DATA_FILE, "r") as file:
            try:
                return json.load(file)
            except json.JSONDecodeError:
                return []
    return []


# Save data to file
def save_data(data):
    with open(DATA_FILE, "w") as file:
        json.dump(data, file, indent=4)


# Retrieve all grave plots
@app.get("/graveplots")
async def get_grave_plots():
    return load_data()


# Save updated grave plots
@app.post("/graveplots")
async def save_grave_plots(graveplots: List[GravePlot]):
    data = [plot.dict() for plot in graveplots]
    save_data(data)
    return {"message": "Grave plots updated successfully"}

if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8000)
