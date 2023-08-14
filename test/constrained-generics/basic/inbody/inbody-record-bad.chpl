interface Bla {
    proc Self.bla(): uint;
}

record R {
    implements Bla(R);
    proc bla(): uint do return 42;
}

proc f(b: Bla) {
  writeln("The value of the bla is: ", b.bla());
}
f(new R());
