
record R {
  var x : int = 5;
  proc postinit() {
    this.x = 42;
  }
}

var A : [1..4] R;
writeln(A);
