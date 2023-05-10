
use IO;
use FormatHelper;
use MemDiagnostics;

record R {
  var x : int;

  proc init(x: int = 0) {
    this.x = x;
  }

  proc init(reader: fileReader, ref deserializer) {
    writeln("IN R.init");
    ref des = deserializer;
    des.startRecord(reader, "R", 1);
    this.x = des.deserializeField(reader, "x", int);
    des.endRecord(reader);
  }

  proc deserialize(reader: fileReader,
                   ref deserializer: reader.deserializerType) {
    writeln("IN R.deserialize");
    this.x = reader.read(int);
  }
}

proc main() {
  var r = new R(5);
  var f = openMemFile();
  writeln("r = ", r);
  {
    f.writer().write(42, serializer=FormatWriter);
  }
  {
    f.reader().read(r, deserializer=FormatReader);
  }
  writeln("r = ", r);

  var A : [1..10] int = 1..10;
  {
    f.writer().write(A, serializer=FormatWriter);
    A = 0;
  }
  {
    var r = f.reader(deserializer=FormatReader);
    startVerboseMem();
    r.read(A);
    stopVerboseMem();
    writeln(A);
  }
}
