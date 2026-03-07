import prisma from '../../core/prisma.js'
import bcrypt from 'bcrypt'

export const findUserByEmailOrUsername = async (email, username) => {
    return await prisma.user.findFirst({
        where: {
            OR: [{ email }, { username }]
        }
    })
}

export const findUserByUsername = async (username) => {
    return await prisma.user.findUnique({
        where: { username }
    })
}

export const createUser = async (email, username, password) => {
    const passwordHash = await bcrypt.hash(password, 10)

    return await prisma.user.create({
        data: {
            username,
            email,
            passwordHash
        }
    })
}
