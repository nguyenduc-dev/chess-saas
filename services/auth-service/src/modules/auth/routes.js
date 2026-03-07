import express from 'express'
import * as authController from './controller.js'
import { verifyToken } from '../../shared/middlewares/authMiddleware.js'

const router = express.Router()

router.post('/register', authController.register)
router.post('/login', authController.login)

router.get('/me', verifyToken, authController.getMe)

export default router
