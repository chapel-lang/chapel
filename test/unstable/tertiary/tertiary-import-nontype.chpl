module M1 {
    record R {}
}
module M2 {
    import M1.{R};
    proc R.doWeirdThing(): string { return "hi"; }
}
module M3 {
    import M1.{R};
    import M2.{doWeirdThing}; // import, but not public
}
module M4 {
    import M1.{R};
    import M3.{doWeirdThing}; // not public in M3, shouldn't be visible.

    proc main() {
        var r: R;
        writeln(r.doWeirdThing());
    }
}
