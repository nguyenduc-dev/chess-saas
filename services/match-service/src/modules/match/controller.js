import * as matchService from './service.js'
import { getBestMoveFromAI } from './aiClient.js'
import { catchAsync } from '../../shared/utils/catchAsync.js'

export const handleSocketEvents = (io) => {
    io.on('connection', (socket) => {
        // Sự kiện: Người chơi bắt đầu ván đấu
        // Sự kiện: Người chơi bắt đầu ván đấu
        socket.on('startMatch', async (data) => {
            try {
                // Khai báo 3 biến này từ data client gửi lên
                const whiteId =
                    data.whiteId === 'ID-USER-THAT' || !data.whiteId
                        ? null
                        : data.whiteId
                const blackId =
                    data.blackId === 'ID-AI' || !data.blackId
                        ? null
                        : data.blackId
                const mode = data.mode || 'pve' // <--- BẠN ĐANG THIẾU DÒNG NÀY

                // Truyền 3 biến vào hàm createMatch
                const match = await matchService.createMatch(
                    whiteId,
                    blackId,
                    mode
                )

                socket.join(match.id)
                socket.emit('matchStarted', match)
                console.log(`✅ Đã tạo ván đấu mới: ${match.id}`)
            } catch (error) {
                // Bắt lỗi Foreign Key...
                if (error.code === 'P2003') {
                    console.error(
                        '⚠️ Lỗi ID người chơi không tồn tại trong Database.'
                    )
                    socket.emit('error', {
                        message: 'Tài khoản người chơi không tồn tại!'
                    })
                } else {
                    console.error('Lỗi khi tạo ván đấu:', error)
                    socket.emit('error', {
                        message: 'Lỗi hệ thống khi tạo ván đấu.'
                    })
                }
            }
        })

        // Sự kiện: Người chơi thực hiện nước đi
        socket.on('sendMove', async (raw_data) => {
            try {
                // 1. Ép kiểu dữ liệu (phòng trường hợp Client gửi text thuần thay vì JSON)
                let data = raw_data
                if (typeof raw_data === 'string') {
                    data = JSON.parse(raw_data)
                }

                console.log('📥 [sendMove] Dữ liệu nhận được:', data)

                const { matchId, move, currentHistory = '', isVsAI } = data

                // 2. KHIÊN BẢO VỆ SERVER: Thiếu data thì chặn ngay lập tức
                if (!matchId || !move) {
                    return socket.emit('error', {
                        message:
                            'Dữ liệu nước đi không hợp lệ (Thiếu matchId hoặc move)!'
                    })
                }

                // 3. Cập nhật vào Database
                const updatedHistory = currentHistory
                    ? `${currentHistory} ${move}`
                    : move
                const match = await matchService.updateMatchMove(
                    matchId,
                    move,
                    updatedHistory
                )

                if (!match) {
                    return socket.emit('error', {
                        message: 'Ván đấu không tồn tại hoặc đã bị xóa!'
                    })
                }

                // 4. Gửi nước đi cho đối thủ (nếu là PvP)
                socket.to(matchId).emit('opponentMove', { move })

                // 5. Nếu đấu với Máy (AI)
                if (isVsAI) {
                    console.log(`🧠 Đang nhờ AI tính toán nước đi tiếp theo...`)
                    const aiMove = await getBestMoveFromAI(updatedHistory)
                    if (aiMove) {
                        const finalHistory = `${updatedHistory} ${aiMove}`
                        await matchService.updateMatchMove(
                            matchId,
                            aiMove,
                            finalHistory
                        )
                        io.to(matchId).emit('opponentMove', { move: aiMove })
                        console.log(`🤖 AI đã đi: ${aiMove}`)
                    }
                }
            } catch (error) {
                console.error('❌ Lỗi xử lý nước đi:', error)
                socket.emit('error', {
                    message: 'Lỗi hệ thống khi xử lý nước đi.'
                })
            }
        })
    })
}
