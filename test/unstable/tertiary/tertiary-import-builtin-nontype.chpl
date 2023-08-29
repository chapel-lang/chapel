module M1 {}
module M2 {
    import M1.{boundsChecking};

    proc main() {
        writeln("hello");
    }
}
