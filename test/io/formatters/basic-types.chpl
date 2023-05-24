
use IO;
use Json;
use List;
use FormatHelper;

//
// TODO: error-handling for failures
//

var failures : list(string);

proc test(val, type T = val.type) {
  writeln();
  const header = "===== " + (T: string) + " =====";
  writeln(header);

  try {
    var f = openMemFile();
    {
      writeln("--- writing: ---");
      stdout.withFormatter(FormatWriter).writeln(val);
      writeln("----------------");

      f.writer().withFormatter(FormatWriter).write(val);
    }
    {
      var readVal = f.reader().withFormatter(FormatReader).read(T);
      writeln("--- read: ---");
      stdout.withFormatter(FormatWriter).writeln(readVal);
      writeln("-------------");

      var compare = if isNilableClassType(val.type) then
                      if val == nil then val == readVal
                      else val!.equals(readVal)
                    else if isClassType(val.type) then val!.equals(readVal)
                    else val == readVal;
      if !compare {
        writeln("FAILURE");
        failures.pushBack(T:string);
      } else writeln("SUCCESS");
    }
  } catch e : Error {
    writeln("FAILURE: ", e.message());
    failures.pushBack(T:string);
  }

  writeln(header.size * "=");
}

enum colors { red, green, blue }

record SimpleRecord {
  var x : int;
  var y : real;
}

record CustomizedRecord {
  var x : int;
  var y : real;

  proc init(x: int = 0, y: real = 0.0) {
    this.x = x;
    this.y = y;
  }

  proc init(r: fileReader) throws {
    this.init();
    r.readLiteral("<");
    this.x = r.read(int);
    r.readLiteral(", ");
    this.y = r.read(real);
    r.readLiteral(">");
  }

  proc encodeTo(w: fileWriter) {
    w.writeLiteral("<");
    w.write(x);
    w.writeLiteral(", ");
    w.write(y);
    w.writeLiteral(">");
  }
}

record GenericRecord {
  type T;
  param P : int;
  var x : T;
  var y : P*T;
}

class Parent {
  var x : int;

  proc equals(other: borrowed Parent?) {
    return x == other!.x;
  }
}

class SimpleChild : Parent {
  var y : real;

  proc equals(other: borrowed SimpleChild?) {
    return x == other!.x &&
           this.y == other!.y;
  }
}

proc main() {
  test(true);
  test(5);
  test(42.0);
  test("a-b-c-d-e-f-g");
  test(1..10);
  test(1..10 by 2);
  test(0..9 by 2 align 1);
  test((1, 2, 3));
  test((1, 42.0, false));
  test(colors.red);
  test(new SimpleRecord(5, 42.0));
  test(new CustomizedRecord(7, 3.14));
  test(new GenericRecord(int, 3, 42, (1,2,3)));
  test(new owned Parent(5));
  test(new owned SimpleChild(5, 42.0));
  test(new owned Parent(5), owned Parent?);
  var nilTemp : owned Parent?;
  test(nilTemp);
  test(new shared Parent(5));
  // TODO: arrays

  if failures.size > 0 {
    writeln("FAILURES:");
    for f in failures do
      writeln("- ", f);
  } else {
    writeln("ALL SUCCESS");
  }
}
