/// <reference types="node" />
export declare function version(): string;
export declare function doWork(): void;
export declare function runLoop(): void;
export declare function stopLoop(): void;
export declare function isLoopRunning(): boolean;
export interface ClientOptions {
    width?: number;
    height?: number;
    fps?: number;
    url: string;
}
export interface CeforamaClient {
    type: 'clientorama';
    width: number;
    height: number;
    fps: number;
    url: string;
    frame: () => Promise<CeforamaFrame>;
    update: () => void;
}
export interface CeforamaFrame {
    type: 'frame';
    width: number;
    height: number;
    size: number;
    seq: number;
    frame: Buffer;
}
export declare function client(options?: ClientOptions): CeforamaClient;
export declare function test(): Promise<void>;
