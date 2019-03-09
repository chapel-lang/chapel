var globalId = 0;

record MyRecord {
  var id : int;

  proc init() {
    writeln('    MyRecord.init()');

    globalId = globalId + 100;

    id       = globalId;

  }

  proc init=(other : MyRecord) {
    writeln('    MyRecord.init(other : MyRecord)');

    id       = other.id + 1;

  }

  proc deinit() {
    writeln('    MyRecord.deinit() ', this);
  }
}




proc main() {
  writeln('main            DefaultInit  rec100');

  var rec100 : MyRecord;

  fnCall(rec100);

  writeln();
  writeln();

  writeln('main            De-Init      rec100      ', rec100);
}




proc fnCall(arg100) {
  {
    writeln(' ');
    writeln('  fnCall        CopyInit     rec101 with ', arg100);

    var rec101 = arg100;

    writeln(' ');
    writeln('  fnCall        De-Init      rec101      ', rec101);
  }

  writeln(' ');

  {
    var rec200 = returnRec();

    writeln(' ');
    writeln('  fnCall        De-Init      rec200      ', rec200);
  }

  writeln(' ');
}




proc returnRec() {
  writeln('    returnRec   DefaultInit  rec200');

  var rec200 : MyRecord;

  writeln('    returnRec   No De-Init');

  return rec200;
}




