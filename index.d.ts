export module smlutils {
    export class Mod {
        constructor(path: string);
    
        getName(): string;
        getDescription(): string;
        getAuthor(): string;
        getVersion(): string;
    }
}