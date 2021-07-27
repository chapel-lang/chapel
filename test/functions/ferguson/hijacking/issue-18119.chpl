module G {
  proc genericfunc(param p) {
    helper1();
    helper2();
    helper3();
  }
}

module M {
  use G;
  proc mCall() {
    writeln("mCall");
    genericfunc(1); // M.helper1, M.call.helper2, M.helper3
    proc helper2() { writeln("M.mCall.helper2"); }
  }
  proc mGenericCall(param p) {
    writeln("mGenericCall");
    genericfunc(p); // this call has POI here, parent POI N
                    // uses M.helper1, N.helper2, M.helper3
                    // -- so used scopes is {M,N}
  }
  proc helper1() { writeln("M.helper1"); }
  proc helper3() { writeln("M.helper3"); }
}

module N {
  use G,M;
  proc nCall() {
    writeln("nCall");
    genericfunc(1); // this call has POI here, no parent POI
                    // uses N.helper1, N.helper2, M.helper3
                    // -- so used scopes is {M,N}
  }
  proc helper1() { writeln("N.helper1"); }
  proc helper2() { writeln("N.helper2"); }

  proc main() {
    nCall();
    mCall();
    mGenericCall(1); // no bug if this call is first
  }
}

