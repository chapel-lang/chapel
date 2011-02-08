proc main() {
  {
    type int = real(64);
    type real = imag(64);
    type imag = string;
    type string = complex(128);
    type complex = bool;

    var a: int = 3.4;
    var b: real = 3.4i;
    var c: imag = "maybe";
    var d: string = 1.2 + 3.4i;
    var e: complex = false;

    writeln("a is: ", a);
    writeln("b is: ", b);
    writeln("c is: ", c);
    writeln("d is: ", d);
    writeln("e is: ", e);
  }
}