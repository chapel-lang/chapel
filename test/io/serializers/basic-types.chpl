
use IO;
use List;
use Types;
use List, Set;

use JSON;
use FormatHelper;

//
// TODO: error-handling for failures
//

var failures : list(string);

proc test(val, type T = val.type) {
  if FormatWriter.type == binarySerializer && isClassType(T) && val == nil then return;

  writeln();
  const header = "===== " + (T: string) + " =====";
  writeln(header);

  try {
    var f = openMemFile();
    {
      printDebugFmt(val);

      f.writer(locking=false).withSerializer(FormatWriter).write(val);
    }

    // Avoid bug when reading domains for now.
    if isDomainType(T) then return;

    {
      var readVal = f.reader(locking=false).withDeserializer(FormatReader).read(T);
      writeln("--- read: ---");
      stdout.withSerializer(defaultSerializer).writeln(readVal);
      writeln("-------------");

      var compare = if isNilableClassType(val.type) then
                      if val == nil then val == readVal
                      else val!.equals(readVal)
                    else if isClassType(val.type) then val!.equals(readVal)
                    else if T == real || T == complex || T == imag
                      then abs(val - readVal) < 1e-5
                      else val == readVal;
      if !compare {
        writeln("FAILURE");
        failures.pushBack(T:string);
      } else writeln("SUCCESS");

      if isUnmanagedClassType(val.type) {
        delete readVal;
      }
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

record CustomizedRecord : writeSerializable, initDeserializable {
  var x : int;
  var y : real;

  proc init(x: int = 0, y: real = 0.0) {
    this.x = x;
    this.y = y;
  }

  proc init(reader: fileReader(?), ref deserializer) throws {
    const ref r = reader;
    this.init();
    r.readLiteral("<");
    this.x = r.read(int);
    r.readLiteral(", ");
    this.y = r.read(real);
    r.readLiteral(">");
  }

  proc serialize(writer: fileWriter(?), ref serializer) {
    writer.writeLiteral("<");
    writer.write(x);
    writer.writeLiteral(", ");
    writer.write(y);
    writer.writeLiteral(">");
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

// Disabled due to bug with virtual dispatch
//class GenericChild : Parent {
//  type T;
//  var y : T;
//
//  proc equals(other: borrowed GenericChild?(?)) {
//    return other.type != borrowed Parent? && x == other!.x &&
//           T == other!.T &&
//           this.y == other!.y;
//  }
//}

class ChildChild : SimpleChild {
  var z : int;

  proc equals(other: borrowed ChildChild?) {
    return x == other!.x &&
           y == other!.y &&
           z == other!.z;
  }
}

// Test printing class hierarchy with a parent in the middle that has no
// fields.
class GrandParentOne {
  var grandParentField : int;
}
class ParentZeroOne : GrandParentOne {}
class Child101 : ParentZeroOne  {
  var field = (1,0,1);
  proc equals(other: borrowed Child101?) {
    return this.grandParentField == other!.grandParentField &&
           this.field == other!.field;
  }
}

proc main() {
  test(true);
  test(5);
  test(42.0);
  test(2.71828182845);
  test(1.12345678i);
  test(7.12345678 - 9.12345678i);
  test("a-b-c-d-e-f-g");
  test(b"12345");
  test((1, 2, 3));
  test((1, 42.0, false));
  test(colors.red);
  test(1..10);
  test(1..10 by 2);
  test(1..10 by -1);
  test(1..20 by 3 align 2);
  test({1..10, 1..10});
  test({1..10, 1..10} by 2);
  test(new SimpleRecord(5, 42.0));
  test(new CustomizedRecord(7, 3.14));
  test(new GenericRecord(int, 3, 42, (1,2,3)));
  test(new owned Parent(5));
  test(new owned SimpleChild(5, 42.0));
  //test(new owned GenericChild(5, int, 42));
  test(new owned ChildChild(1, 42.0, 5));

  // Make sure we can read an initialized value into a nilable type.
  // Needs to be 'new owned Parent?' in case the format includes type names.
  test(new owned Parent?(5), owned Parent?);

  var nilTemp : owned Parent?;
  test(nilTemp);
  test(new shared Parent(5));

  test(new owned Child101());

  var x = new unmanaged SimpleChild(5, 42.0);
  test(x);
  delete x;

  var s : set(int);
  for i in 1..10 do s.add(i);
  test(s);

  if failures.size > 0 {
    writeln("FAILURES:");
    for f in failures do
      writeln("- ", f);
  } else {
    writeln("ALL SUCCESS");
  }
}
