export declare function version(): string;
export declare function doWork(): void;
export declare function runLoop(): void;
export declare function stopLoop(): void;
export declare function isLoopRunning(): boolean;
export interface ClientOptions {
}
export interface CeforamaClient {
}
export declare function client(options?: ClientOptions): CeforamaClient;
