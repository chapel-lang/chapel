
use IO;
use Json;
use FormatHelper;

record InOrder {
  var z : string;
  var x : int;
  var y : real;
}

record Alpha {
  var x : int;
  var y : real;
  var z : string;
}

record Reverse {
  var y : real;
  var x : int;
  var z : string;
}

record Missing {
  var x : int;
  var z : string;
}

record Extra {
  var z : string;
  var x : int;
  var y : real;
  var w : int;
}

proc main() {
  var f = openMemFile();
  {
    var data = new InOrder("hello", 5, 42.0);
    f.writer().withSerializer(FormatWriter).write(data);
  }

  {
    var reader = f.reader().withDeserializer(FormatReader);
    var r = reader.read(InOrder);
    writeln(r);
  }
  {
    var reader = f.reader().withDeserializer(FormatReader);
    var r = reader.read(Alpha);
    writeln(r);
  }
  {
    var reader = f.reader().withDeserializer(FormatReader);
    var r = reader.read(Reverse);
    writeln(r);
  }
  {
    var reader = f.reader().withDeserializer(FormatReader);
    var r = reader.read(Missing);
    writeln(r);
  }
  {
    // Expecting an error in this case
    var reader = f.reader().withDeserializer(FormatReader);
    var r = reader.read(Extra);
    writeln(r);
  }
}
