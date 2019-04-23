export class Mod {
    constructor(path: string);

    reload(): void;
    get name(): string;
    get description(): string;
    get author(): string;
    get version(): string;
    get hasSetup(): boolean;
}