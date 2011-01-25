proc main() {
  proc f() { }
  {
    proc f() { } // shadows other f based on call to f
    f();
  }

  use M2;
  proc g() { } // shadows other g based on call to g
  g();

  use M3;
  use M4;
  h();
}

module M2 { 
  proc g() { }
}

module M3 {
  use M5;
  proc h() { } // does not shadow other h based on call to h since
  // there is another path (through M4) to the other h
}

module M4 {
  use M5;
}

module M5 {
  proc h() { }
}
