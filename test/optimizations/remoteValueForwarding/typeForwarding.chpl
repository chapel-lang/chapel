record myRecord {
  var f: int;
  
  proc chpl__serialize() {
    writeln("Should not be called!");
    return 1;
  }

  proc type chpl__deserialize(data) {
    writeln("Should not be called!");
    return new myRecord(data);
  }
}

proc foo() {
  type t = myRecord;
  on Locales[numLocales-1] {
    var x: t;
    writeln(x, ": ", t:string);
  }
}

foo();