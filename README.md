# ♟️ Chess - Nền tảng SaaS Cờ vua & AI Engine

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)
![Node.js](https://img.shields.io/badge/Node.js-Backend-green.svg?style=flat&logo=nodedotjs)
![Architecture](https://img.shields.io/badge/Architecture-Microservices-orange.svg)
![Status](https://img.shields.io/badge/Status-Under_Development-yellow.svg)

**Chess** là một nền tảng SaaS (Software as a Service) cờ vua trực tuyến, được thiết kế với kiến trúc Microservices để đảm bảo khả năng mở rộng và chịu tải cao. Điểm nhấn của dự án là một **Trí tuệ nhân tạo (AI Chess Engine)** được viết hoàn toàn bằng C++ từ con số 0, sở hữu tốc độ tính toán siêu việt và giao tiếp qua chuẩn UCI quốc tế.

---

## 🏗️ Kiến trúc Hệ thống (Architecture)

Dự án được tổ chức theo mô hình **Monorepo**, bao gồm các thành phần chính:

1. **AI Engine (C++)**: Lõi xử lý cờ vua sử dụng kỹ thuật Bitboard, thuật toán tìm kiếm Negamax với cắt tỉa Alpha-Beta, và bảng lượng giá vị trí (Piece-Square Tables).
2. **Microservices (Node.js)**: Hệ thống Backend phân tán xử lý hàng đợi, ghép trận (Matchmaking), WebSockets real-time và giao tiếp với AI qua Child Process.
3. **Frontend (Dự kiến)**: Giao diện người dùng Web Application tương tác trực tiếp với các dịch vụ lõi.

## 📂 Cấu trúc Thư mục

```text
@chess/
├── engine-cpp/                 # Lõi C++ AI Engine (Bitboard, Negamax, UCI)
│   ├── src/                    # Source code C++
│   ├── include/                # Header files (.h)
│   └── CMakeLists.txt          # Cấu hình biên dịch
├── services/                   # Các dịch vụ Backend (Node.js)
│   ├── server.js               # API Gateway & Engine Wrapper
│   └── package.json
├── shared/                     # Code dùng chung (Types, Utils)
├── .gitignore
└── README.md
```
