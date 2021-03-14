proc main() {
  {
    type intalias = int(64);
    type int = real(64);
    type real = intalias;

    var a: int = 3.4;
    var b: real = 5;

    writeln("a is: ", a);
    writeln("b is: ", b);
  }
}