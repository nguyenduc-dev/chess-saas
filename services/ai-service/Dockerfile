FROM python:3.11-slim

RUN apt-get update && apt-get install -y g++ make && rm -rf /var/lib/apt/lists/*

# 1. Đưa engine-cpp ra thẳng thư mục gốc (root) để khớp với code Python
COPY engine-cpp /engine-cpp
RUN mkdir -p /engine-cpp/build
RUN g++ -O3 /engine-cpp/src/*.cpp -I/engine-cpp/include -o /engine-cpp/build/engine

# 2. Setup code Python ở thư mục /app như bình thường
WORKDIR /app
COPY services/ai-service/requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY services/ai-service .

EXPOSE 3002
CMD ["uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "3002"]