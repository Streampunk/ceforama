"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.test = exports.client = exports.isLoopRunning = exports.stopLoop = exports.runLoop = exports.doWork = exports.version = void 0;
const ceforamaNative = require('../build/Release/ceforama');
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
        setImmediate(() => {
            ceforamaNative.doWork();
            loopy();
        });
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
    let cl = await ceforamaNative.client({ url: 'https://raw.githubusercontent.com/chromiumembedded/cef-project/master/CMakeLists.txt' });
    for (let x = 0; x < 100; x++) {
        let fr = await cl.frame();
        let isBlack = true;
        for (let y = 0; y < fr.frame.length; y++) {
            if (fr.frame[y] !== 0) {
                isBlack = false;
                break;
            }
        }
        console.log(fr.seq, isBlack);
    }
    console.log('Done asking!');
}
exports.test = test;
