// Example 5'' from 21668 née 15937
module Main {
  public use Library;
  proc R.field { compilerWarning("In R.field"); return 8; }

  proc main() {
    var myR: R;
    myR.method();
  }
}

module Library {
  record R {
    var field = 77;
    proc method() { writeln(this.field); }
  }
}
