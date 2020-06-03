module M {
  var x = -1;
}

module N {
  record R {
    var x: int;
  }
  var M = new R(42);
}

module O {
  proc main() {
    use M only;
    use N;
    writeln(M.x);
  }
}
