module M1 {
  var aaa = 321;
}

module M2 {
  use M1;
}

module M3 {
  use M2;
}

module M4 {
  var aaa = 54;
}

module M5 {
  use M4;
}

module M6 {
  var aaa = 6;
  proc main() {
    use M2, M5;
    writeln(aaa); // M2.M1.aaa and M5.M4.aaa are defined at the same use-depth
  }
}
