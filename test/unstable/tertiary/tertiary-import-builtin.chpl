module M1 {
    proc string.doWeirdThing(): string {
        return "(" + this + ")";
    }
}
module M2 {
    import M1.{string};

    proc main() {
        writeln("hello".doWeirdThing());
    }
}
