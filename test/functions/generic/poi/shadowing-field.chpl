
module Main {
  use Library;
  proc MyRecord.field {
    compilerWarning("in Main.MyRecord.field");
    return 66;
  }
  var myR: MyRecord;
  proc main {
    myR.genericMethod(int);
    compilerWarning("in Main.main");
  }
}

module Library {
  record MyRecord {
    var field: int;
    proc genericMethod(type t) {
      writeln(this.field);
      compilerWarning("in genericMethod()");
    }
  }
}
