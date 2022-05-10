module OuterModule {
  proc main() {
    proc f() { }
    {
      proc f() { } // shadows other f based on call to f
      f();
    }

    public use M2;
    proc g() { }
    g();

  }

  module M2 { 
    proc g() { }
  }
}
