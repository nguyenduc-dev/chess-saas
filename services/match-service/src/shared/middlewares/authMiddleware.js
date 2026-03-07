import jwt from 'jsonwebtoken'
import { AppError } from '../utils/AppError.js'
import { catchAsync } from '../utils/catchAsync.js'

const JWT_SECRET = process.env.JWT_SECRET

export const verifyToken = catchAsync(async (req, res, next) => {
    let token

    if (
        req.headers.authorization &&
        req.headers.authorization.startsWith('Bearer')
    ) {
        token = req.headers.authorization.split(' ')[1]
    }

    if (!token) {
        return next(new AppError('Ban chua dang nhap!', 401))
    }

    const decoded = jwt.verify(token, JWT_SECRET)

    req.user = decoded

    next()
})
