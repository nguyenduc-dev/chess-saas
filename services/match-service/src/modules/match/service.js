import prisma from '../../core/prisma.js'

export const createMatch = async (
    whitePlayerId,
    blackPlayerId,
    mode = 'pvp'
) => {
    return await prisma.match.create({
        data: {
            whitePlayerId,
            blackPlayerId,
            mode,
            moveHistory: '',
            status: 'ongoing'
        }
    })
}

export const updateMatchMove = async (matchId, newMove, fullHistory) => {
    try {
        return await prisma.match.update({
            where: { id: matchId },
            data: {
                moveHistory: fullHistory
            }
        })
    } catch (error) {
        if (error.code === 'P2025') {
            console.error(`⚠️ Không tìm thấy ván đấu với ID: ${matchId}`)
            return null
        }
        throw error
    }
}

export const finishMatch = async (matchId, result) => {
    return await prisma.match.update({
        where: { id: matchId },
        data: {
            status: 'finished',
            result,
            endedAt: new Date()
        }
    })
}
