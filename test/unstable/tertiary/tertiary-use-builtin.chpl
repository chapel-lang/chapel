module M1 {
    proc string.doWeirdThing(): string {
        return "(" + this + ")";
    }
}
module M2 {
    use M1 only string;

    proc main() {
        writeln("hello".doWeirdThing());
    }
}
