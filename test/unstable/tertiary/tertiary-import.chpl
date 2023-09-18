module M1 {
    record R {}
}
module M2 {
    import M1.{R};

    proc R.doWeirdThing(): string { return "hi"; }
}
module M3 {
    import M1.{R};
    import M2.{R};

    proc main() {
        var r: R;
        writeln(r.doWeirdThing());
    }
}
