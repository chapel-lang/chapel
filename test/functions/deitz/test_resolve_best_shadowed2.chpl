module OuterModule {
  proc main() {
    proc f() { }
    {
      proc f() { } // shadows other f based on call to f
      f();
    }

    public use M2;
    proc g() { } // shadows other g based on call to g
    g();

    public use M3;
    public use M4;
    h();
  }

  module M2 { 
    proc g() { }
  }

  module M3 {
    public use super.M5;
    proc h() { } // does not shadow other h based on call to h since
    // there is another path (through M4) to the other h
  }

  module M4 {
    public use super.M5;
  }

  module M5 {
    proc h() { }
  }
}
