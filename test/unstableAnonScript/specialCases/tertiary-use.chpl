// Copied from test/unstable/tertiary/tertiary-use.chpl

module M1 {
    record R {}
}
module M2 {
    import M1.{R};

    proc R.doWeirdThing(): string { return "hi"; }
}
module M3 {
    use M1 only R;
    use M2 only R;

    proc main() {
        var r: R;
        writeln(r.doWeirdThing());
    }
}
