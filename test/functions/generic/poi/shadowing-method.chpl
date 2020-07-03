
module Main {
  use Library;
  proc MyRecord.method() {
    compilerWarning("in Main.MyRecord.method");
    return 1;
  }
  var myR: MyRecord;
  proc main {
    myR.genericMethod(int);
    compilerError("in Main.main, done");
  }
}

module Library {
  record MyRecord {
    proc method() {
      compilerWarning("in Library.MyRecord.method");
      return 2;
    }
    proc genericMethod(type t) {
      writeln(this.method());
    }
  }
}
