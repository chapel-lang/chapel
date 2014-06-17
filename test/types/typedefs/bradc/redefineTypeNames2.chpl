proc main() {
  {
    type int = real(64);
    type real = int(64);

    var a: int = 3.4;
    var b: real = 5;

    writeln("a is: ", a);
    writeln("b is: ", b);
  }
}