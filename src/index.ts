const ceforamaNative = require('../build/Release/ceforama')
const fs = require('fs');

export function version() : string {
    return ceforamaNative.version()
}

export function doWork(): void {
    return ceforamaNative.doWork()
}

let loopRunning = false

const loopy = () => {
    if (loopRunning) {
        setTimeout(() => {
            ceforamaNative.doWork()
            loopy()
        }, 0)
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
    let cl = await ceforamaNative.client({ url: 'https://app.singular.live/output/5AJz1INQDZ8EmHunA5H3et/Default?aspect=16:9' })
    let fr
    let updater
    let start = process.hrtime();
    for ( let x = 0 ; x < 1000 ; x++ ) {
        fr = await cl.frame();
        if (x === 3) { updater = setInterval(() => cl.update(), 40) }
        // let isBlack = true
        // for ( let y = 0 ; y < fr.frame.length; y++ ) {
        //     if (fr.frame[y] !== 0) {
        //         isBlack = false;
        //         break;
        //     }
        // }
        console.log(fr.seq, process.hrtime(start), fr.frame.length);
        // if ((x - 9) % 10 === 0) {
        //     fs.writeFileSync(`test-${x}.rgba`, fr.frame)
        // }
    }
    console.log('Done asking!', fr.frame.length)
    fs.writeFileSync('test.rgba', fr.frame)
    if (updater) clearInterval(updater)
}