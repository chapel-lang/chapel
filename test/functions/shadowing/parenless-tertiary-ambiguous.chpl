module Library {
  record R { }
}

module A {
  use Library;
  proc R.field { compilerWarning("In A.R.field"); return 8; }
}

module B {
  use Library;
  proc R.field { compilerWarning("In b.R.field"); return 1024; }
}

module Main {
  use Library, A, B;
  proc main() {
    var myR: R;
    writeln(myR.field);
  }
}
