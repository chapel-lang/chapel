module M1 {}
module M2 {
    use M1 only boundsChecking;

    proc main() {
        writeln("hello");
    }
}
