import subprocess
import os

# (Dựa trên cấu trúc dự án: /services/ai-service/app/engine_manager.py trỏ ngược ra /engine-cpp/build/engine)
BASE_DIR = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
ENGINE_PATH = os.path.join(BASE_DIR, "engine-cpp", "build", "engine")

def get_best_move(moves: str = "") -> str:
    """
    Khởi động engine C++, gửi chuỗi nước đi và nhận về nước đi tốt nhất.
    """

    if not os.path.exists(ENGINE_PATH):
        raise FileNotFoundError(f"Khong tim thay Engine C++ tai: {ENGINE_PATH}")
    
    try:
        process = subprocess.Popen(
            [ENGINE_PATH],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        # Chuẩn bị lệnh UCI
        commands = "uci\nisready\n"

        if moves:
            commands += f"position startpos moves {moves}\n"
        else:
            commands += "position startpos\n"
        commands += "go\n" # Bạn có thể thêm 'go depth 10' hoặc 'go movetime 1000' để giới hạn thời gian AI nghĩ

        # Gửi lệnh và đọc kết quả trả về
        stdout, stderr = process.communicate(input=commands, timeout=10) # Timeout 10s tránh treo máy

        # Quét từng dòng kết quả để tìm "bestmove"
        best_move = ""
        for line in stdout.split('\n'):
            if line.strip().startswith('bestmove'):
                best_move = line.strip().split(' ')[1]
                break

        return best_move

    except subprocess.TimeoutExpired:
        process.kill()
        raise RuntimeError("Engine suy nghĩ quá lâu (Timeout)!")
    except Exception as e:
        raise RuntimeError(f"Lỗi khi chạy Engine: {str(e)}")
    