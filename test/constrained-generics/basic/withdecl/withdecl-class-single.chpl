interface Bla {
    proc Self.bla(): uint;
}

class R: Bla {
    proc bla(): uint do return 42;
}

proc f(b: Bla) {
  writeln("The value of the bla is: ", b.bla());
}
f(new R());
