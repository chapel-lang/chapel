module M1 {
}

module M2 {
  var size = 1;
  proc main {
    use M1;
    writeln(size);
  }
}
