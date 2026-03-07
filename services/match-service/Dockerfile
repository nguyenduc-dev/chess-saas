FROM node:20-alpine

WORKDIR /app

# 1. Copy package.json từ đúng thư mục của service
COPY services/auth-service/package*.json ./
RUN npm install

# 2. Copy mã nguồn của service
COPY services/auth-service .

# 4. Chạy Prisma (nếu có)
RUN npx prisma generate

EXPOSE 3003
CMD npm run dev