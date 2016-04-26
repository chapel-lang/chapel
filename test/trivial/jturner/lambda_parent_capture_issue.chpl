proc testme() {
  var i = 5;

  class parent {
    proc this() { return 0; }
  }

  class child : parent {
    proc this() { return i; }
  }

  var f : parent = new child();

  writeln(f());

  delete f;
}

testme();
