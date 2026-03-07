import { AppError } from '../utils/AppError.js'

const handlePrismaDuplicateFields = (err) => {
    const message = `Dữ liệu đã tồn tại. Vui lòng sử dụng giá trị khác!`
    return new AppError(message, 400)
}

export const globalErrorHandler = (err, req, res, next) => {
    err.statusCode = err.statusCode || 500
    err.status = err.status || 'error'

    let error = Object.assign(err)
    if (error.code === 'P2002') error = handlePrismaDuplicateFields(error)
    if (error.name === 'JsonWebTokenError')
        error = new AppError('Token không hợp lệ. Vui lòng đăng nhập lại!', 401)
    if (error.name === 'TokenExpiredError')
        error = new AppError('Token đã hết hạn. Vui lòng đăng nhập lại!', 401)

    res.status(error.statusCode).json({
        success: false,
        status: error.status,
        message: error.message,
        stack: process.env.NODE_ENV === 'development' ? error.stack : undefined
    })
}
