
use IO;

proc test(param endian : ioendian) {
  writeln("===== ", endian:string, " =====");
  {
    write("Actual EOF: ");
    var f = openMemFile();
    {
      f.writer().writeBinary(123:uint(16), endian);
    }
    {
      var val : uint;
      try {
        var ret = f.reader().readBinary(val, endian);
        writeln("UNINTENDED SUCCESS; val = ", val, "; ret = ", ret);
      } catch e {
        writeln("ERROR: ", e.message());
      }
    }
  }
  {
    write("Short Region: ");
    var f = openMemFile();
    {
      f.writer().writeBinary(123:uint(64), endian);
    }
    {
      var val : uint;
      try {
        var ret = f.reader(region=0..4).readBinary(val, endian);
        writeln("UNINTENDED SUCCESS; val = ", val, "; ret = ", ret);
      } catch e {
        writeln("ERROR: ", e.message());
      }
    }
  }
  {
    write("Short Seek Region: ");
    var f = openMemFile();
    {
      var w = f.writer();
      w.writeBinary(123:uint(64), endian);
      w.writeBinary(456:uint(64), endian);
    }
    {
      var val : uint;
      try {
        var r = f.reader(locking=false);
        r.seek(8..12);
        var ret = r.readBinary(val, endian);
        writeln("UNINTENDED SUCCESS; val = ", val, "; ret = ", ret);
      } catch e {
        writeln("ERROR: ", e.message());
      }
    }
  }
}

proc main() {
  test(ioendian.native);
  test(ioendian.little);
  test(ioendian.big);
}
