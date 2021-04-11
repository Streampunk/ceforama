const ceforamaNative = require('../build/Release/ceforama')

export function version() : string {
    return ceforamaNative.version()
}

export function doWork(): void {
    return ceforamaNative.doWork()
}

let loopRunning = false

const loopy = () => {
    if (loopRunning) {
        setImmediate(() => {
            ceforamaNative.doWork()
            loopy()
        })
    }
}

export function runLoop(): void {
    if (!loopRunning) {
        loopRunning = true
        loopy()
    }
}

export function stopLoop(): void {
    if (loopRunning) {
        loopRunning = false
    }
}

export function isLoopRunning(): boolean {
    return loopRunning
}

export interface ClientOptions {

}

export interface CeforamaClient {

}

export function client(options?: ClientOptions): CeforamaClient {
    return ceforamaNative.client(options)
}