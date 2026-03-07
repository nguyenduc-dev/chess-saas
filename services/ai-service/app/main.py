from fastapi import FastAPI, HTTPException
from pydantic import BaseModel 
from app.engine_manager import get_best_move

app = FastAPI(title="Chess AI service")

class MoveRequest(BaseModel):
    moves: str = ""

@app.post("/api/engine/move")
async def fetch_ai_move(request: MoveRequest):
    try:
        best_move = get_best_move(request.moves)

        if not best_move:
            raise HTTPException(status_code=500, detail="Engine khong tra ve nuoc di hop le")
        return {"success": True, "bestMove": best_move}
    
    except FileNotFoundError as e:
        raise HTTPException(status_code=404, detail=str(e))
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))
    