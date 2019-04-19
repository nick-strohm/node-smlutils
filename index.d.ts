declare module smlutils {
    interface Mod {
        constructor(path: string);

        getName(): string;
        getDescription(): string;
        getAuthor(): string;
        getVersion(): string;
    }
}