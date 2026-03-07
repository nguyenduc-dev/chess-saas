import axios from 'axios'

const AI_SERVICE_URL = process.env.AI_SERVICE_URL

export const getBestMoveFromAI = async (moves) => {
    try {
        const response = await axios.post(`${AI_SERVICE_URL}/api/engine/move`, {
            moves
        })

        return response.data.bestMove
    } catch (error) {
        console.error('Loi service: ', error.message)
        return null
    }
}
