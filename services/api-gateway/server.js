import express from 'express'
import cors from 'cors'
import morgan from 'morgan'
import dotenv from 'dotenv'
import { createProxyMiddleware } from 'http-proxy-middleware'

dotenv.config()

const app = express()

app.use(cors())
app.use(morgan('dev'))

const AUTH_URL = process.env.AUTH_SERVICE_URL
const AI_URL = process.env.AI_SERVICE_URL
const MATCH_URL = process.env.MATCH_SERVICE_URL

app.use(
    createProxyMiddleware({
        pathFilter: '/api/auth',
        target: AUTH_URL,
        changeOrigin: true
    })
)

app.use(
    createProxyMiddleware({
        pathFilter: '/api/engine',
        target: AI_URL,
        changeOrigin: true
    })
)

app.use(
    createProxyMiddleware({
        pathFilter: '/api/match',
        target: MATCH_URL,
        changeOrigin: true
    })
)

app.use(
    '/socket.io',
    createProxyMiddleware({
        target: MATCH_URL,
        changeOrigin: true,
        ws: true
    })
)

const PORT = process.env.PORT || 3000
app.listen(PORT, () => {
    console.log(`=========================================`)
    console.log(`🌐 [API-Gateway] is listening on port ${PORT}`)
    console.log(`=========================================`)
    console.log(`Routes Map:`)
    console.log(` 🔑 /api/auth   -> ${AUTH_URL}`)
    console.log(` 🧠 /api/engine -> ${AI_URL}`)
    console.log(` ⚔️  /socket.io  -> ${MATCH_URL}`)
    console.log(`=========================================`)
})
