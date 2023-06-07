

module FormatHelper {
  use Json;
  use BinaryIO;
  use IO;
  use ChplFormat;

  enum FormatKind {
    default,
    json,
    little,
    big,
    syntax
  }

  config param format : FormatKind = FormatKind.default;

  proc getFormatVal(param writing : bool) {
    select format {
      when FormatKind.default {
        if writing then return new IO.DefaultSerializer();
        else return new IO.DefaultDeserializer();
      }
      when FormatKind.json {
        if writing then return new JsonSerializer();
        else return new JsonDeserializer();
      }
      when FormatKind.little {
        if writing then return new BinarySerializer(endian=IO.ioendian.little);
        else return new BinaryDeserializer(endian=IO.ioendian.little);
      }
      when FormatKind.big {
        if writing then return new BinarySerializer(endian=IO.ioendian.big);
        else return new BinaryDeserializer(endian=IO.ioendian.big);
      }
      when FormatKind.syntax {
        if writing then return new ChplSerializer();
        else return new ChplDeserializer();
      }
      otherwise return nothing;
    }
  }

  var FormatReader = getFormatVal(false);
  var FormatWriter = getFormatVal(true);

  proc printDebugFmt(val) throws {
    writeln("===== writing: =====");
    stdout.writeln(val);
    writeln("--------------------");
    if format == FormatKind.little || format == FormatKind.big {
      var f = openMemFile();
      {
        var w = f.writer();
        w.withSerializer(FormatWriter).write(val);
      }
      var r = f.reader();
      try {
        while true {
          stdout.writef("%02xu", r.readByte());
        }
      } catch { }
      stdout.writeln();
    } else {
      try {
        stdout.withSerializer(FormatWriter).writeln(val);
      } catch e {
        stdout.writeln("ERROR: ", e);
      }
    }
    writeln("====================");
  }

}
