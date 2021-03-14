module M {
  var x: int = 4;
}

module User {
  module M {
    var x: int = 7;
  }

  module Inner {
    import M;
    proc main() {
      writeln(M.x);
    }
  }
}
