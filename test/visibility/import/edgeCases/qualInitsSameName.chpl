module M {
  proc y() {
    return 33;
  }
}

module N {
  proc main() {
    import M;
    var y = M.y();
    writeln(y);
  }
}
