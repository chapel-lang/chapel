// Regression test for bug fixed by https://github.com/chapel-lang/chapel/pull/22227
// You are allowed to import methods (how else would you get them into scope?)
module M {
    proc int.printMe() {
        writeln(this);
    }
}
module N {
    import M.{printMe};

    proc main() {
        42.printMe();
    }
}
