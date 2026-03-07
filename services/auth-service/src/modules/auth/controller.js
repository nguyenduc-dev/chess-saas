import bcrypt from 'bcrypt'
import jwt from 'jsonwebtoken'
import * as authService from './service.js'
import { AppError } from '../../shared/utils/AppError.js'
import { catchAsync } from '../../shared/utils/catchAsync.js'

const JWT_SECRET = process.env.JWT_SECRET

const signToken = (user) => {
    return jwt.sign({ userId: user.id, username: user.username }, JWT_SECRET, {
        expiresIn: '7d'
    })
}

export const register = catchAsync(async (req, res, next) => {
    const { username, email, password } = req.body

    if (!username || !email || !password) {
        return next(new AppError('Vui long dien day du thong tin!', 400)) // Quăng lỗi, hệ thống tự bắt
    }

    const existingaUser = await authService.findUserByEmailOrUsername(
        email,
        username
    )

    if (existingaUser) {
        return next(new AppError('Tai khoan hoc Email da ton tai!', 400))
    }

    const user = await authService.createUser(username, email, password)

    const token = signToken(user)

    res.status(201).json({
        success: true,
        message: 'Dang ky thanh cong',
        token,
        user: {
            id: user.id,
            username: user.username,
            elo: user.elo
        }
    })
})

export const login = catchAsync(async (req, res, next) => {
    const { username, password } = req.body

    if (!username || !password) {
        return next(new AppError('Vui long nhap tai khoan va mat khau!', 400))
    }

    const user = await authService.findUserByUsername(username)
    if (!user) {
        return next(new AppError('Tai khoan khong ton tai!', 401))
    }

    const isMatch = await bcrypt.compare(password, user.passwordHash)
    if (!isMatch) {
        return next(new AppError('Username hoac mat khau khong dung!', 401))
    }

    const token = signToken(user)

    res.status(200).json({
        success: true,
        message: 'Dang nhap thanh cong',
        token,
        user: { id: user.id, username: user.username, elo: user.elo }
    })
})

export const getMe = catchAsync(async (req, res, next) => {
    const user = await authService.findUserByUsername(req.user.username)

    if (!user) {
        return next(new AppError('Tai khoan khong ton tai.', 404))
    }

    res.status(200).json({
        success: true,
        data: {
            id: user.id,
            username: user.username,
            email: user.email,
            elo: user.elo,
            createdAt: user.createdAt
        }
    })
})
