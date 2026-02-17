module M1 {
  proc foo(x: real) {
    return x + 1;
  }
}

module M2 {
  import M1; // changing to use allows it to resolve
  proc main() {
    const n = 10;
    var x: [1..n] real;
    x = 1..n;
    var y = M1.foo(x); // promoted call falls to resolve
    writeln(y);
  }
}
