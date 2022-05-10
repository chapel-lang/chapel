module G {
  proc genericfunc(param p) {
    helper1();
  }
}

module M {
  use G;
  proc mCallFunc() {
    writeln("mCallFunc");
    genericfunc(1);
  }
  proc helper1() { writeln("M.helper1"); }
}

module N {
  use G,M;
  proc helper1() { writeln("N.helper1"); }
  proc helper1() { writeln("N.helper1 mark two"); }
  proc nCallFunc() {
    writeln("nCallFunc");
    genericfunc(1);
  }

  proc main() {
    nCallFunc();
    mCallFunc();
  }
}
