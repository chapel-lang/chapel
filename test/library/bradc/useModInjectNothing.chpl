module M1 {
  writeln("In M1's initialization");
  var x: real = 3.4;
}

module M2 {
  proc main() {
    writeln("x is: ", x);
  }
}
