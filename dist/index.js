"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.test = exports.client = exports.isLoopRunning = exports.stopLoop = exports.runLoop = exports.doWork = exports.version = void 0;
const ceforamaNative = require('../build/Release/ceforama');
const fs = require('fs');
function version() {
    return ceforamaNative.version();
}
exports.version = version;
function doWork() {
    return ceforamaNative.doWork();
}
exports.doWork = doWork;
let loopRunning = false;
const loopy = () => {
    if (loopRunning) {
        setTimeout(() => {
            ceforamaNative.doWork();
            loopy();
        }, 0);
    }
};
function runLoop() {
    if (!loopRunning) {
        loopRunning = true;
        loopy();
    }
}
exports.runLoop = runLoop;
function stopLoop() {
    if (loopRunning) {
        loopRunning = false;
    }
}
exports.stopLoop = stopLoop;
function isLoopRunning() {
    return loopRunning;
}
exports.isLoopRunning = isLoopRunning;
function client(options) {
    return ceforamaNative.client(options);
}
exports.client = client;
async function test() {
    let cl = await client({ url: 'https://app.singular.live/output/5AJz1INQDZ8EmHunA5H3et/Default?aspect=16:9' });
    let fr;
    let updater;
    let start = process.hrtime();
    for (let x = 0; x < 1000; x++) {
        fr = await cl.frame();
        if (x === 3) {
            updater = setInterval(() => cl.update(), 40);
        }
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
    console.log('Done asking!', fr === null || fr === void 0 ? void 0 : fr.frame.length);
    fs.writeFileSync('test.rgba', fr === null || fr === void 0 ? void 0 : fr.frame);
    if (updater)
        clearInterval(updater);
}
exports.test = test;
