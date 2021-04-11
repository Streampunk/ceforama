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

export async function test() {
    let cl = await ceforamaNative.client({ url: 'https://raw.githubusercontent.com/chromiumembedded/cef-project/master/CMakeLists.txt' })
    for ( let x = 0 ; x < 100 ; x++ ) {
        let fr = await cl.frame();
        let isBlack = true
        for ( let y = 0 ; y < fr.frame.length; y++ ) {
            if (fr.frame[y] !== 0) {
                isBlack = false;
                break;
            }
        }
        console.log(fr.seq, isBlack);
    }
    console.log('Done asking!')
}