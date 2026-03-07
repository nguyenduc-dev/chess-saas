import express from 'express'
import cors from 'cors'
import { authRoutes } from './modules/auth/index.js'
import { AppError } from './shared/utils/AppError.js'
import { globalErrorHandler } from './shared/middlewares/errorHandler.js'

const app = express()

app.use(cors())
app.use(express.json())

app.use('/api/auth', authRoutes)

app.use((req, res, next) => {
    next(
        new AppError(
            `Không tìm thấy đường dẫn ${req.originalUrl} trên máy chủ!`,
            404
        )
    )
})

app.use(globalErrorHandler)

export default app
