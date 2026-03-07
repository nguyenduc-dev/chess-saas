import express from 'express'
import { createServer } from 'http'
import { Server } from 'socket.io'
import dotenv from 'dotenv'
import cors from 'cors'
import { handleSocketEvents } from './src/modules/match/controller.js'
import { globalErrorHandler } from './src/shared/middlewares/errorHandler.js'

dotenv.config()

const app = express()

app.use(cors())
app.use(express.json())

const httpServer = createServer(app)

const io = new Server(httpServer, {
    cors: { origin: '*' }
})

io.on('connection', (socket) => {
    console.log(`📡 Thiet bi moi ket noi: ${socket.id}`)

    socket.on('joinMatch', (matchId) => {
        socket.join(matchId)
        console.log(`👤 User tham gia tran: ${matchId}`)
    })

    socket.on('sendMove', async (data) => {
        const { matchId, move, history } = data

        // 1. Gửi nước đi của người chơi cho đối thủ trong phòng
        socket.to(matchId).emit('opponentMove', move)

        // 2. Nếu đang đấu với máy, gọi AI Service
        // (Đây là logic demo, chúng ta sẽ làm kỹ hơn ở phần Controller)
        console.log(`♟️ Nước đi mới trong trận ${matchId}: ${move}`)
    })

    socket.on('disconnect', () => {
        console.log('❌ Một kết nối đã ngắt.')
    })
})

handleSocketEvents(io)

app.use(globalErrorHandler)

const PORT = process.env.PORT || 3001
httpServer.listen(PORT, () => {
    console.log(`🚀 [Match-Service] Real-time engine running on port ${PORT}`)
    console.log(`🤖 AI Bridge connected to: ${process.env.AI_SERVICE_URL}`)
})
