module M {
  proc y() {
    return 33;
  }
}

module N {
  proc main() {
    use M;
    if false then
      M.y;
    else
      M.y();
    writeln("Success");
  }
}
