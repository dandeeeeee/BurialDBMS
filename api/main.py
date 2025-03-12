from fastapi import FastAPI
import uvicorn

app = FastAPI()

@app.get("/get_layout")
def get_layout():
    return [
        {"x": 100, "y": 200, "width": 50, "height": 50, "status": "occupied"},
        {"x": 160, "y": 200, "width": 50, "height": 50, "status": "available"}
    ]

if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8000)
