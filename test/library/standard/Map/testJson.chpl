
use IO;
use Map;
use List;
use JSON;

record R {
  var x : int;
  var y : real;
}

record Z {
  var z : string;
  var r : R;
}

proc testMap(orig: map(?)) {
  const header = "--- " + orig.type:string + " ---";
  writeln(header);
  var f = openMemFile();
  {
    f.writer(serializer= new jsonSerializer(), locking=false).writef("%?", orig);
    writeln("Writing map: ");
    writeln(orig);
    writeln();
    writeln("Wrote JSON: ");
    stdout.withSerializer(jsonSerializer).writef("%?\n", orig);
    writeln();
  }
  {
    var r = f.reader(deserializer = new jsonDeserializer(), locking=false);
    var m : orig.type;
    r.readf("%?", m);
    writeln("Read ", m.type:string, ": ");
    writeln(m);
    writeln("Equals original: ", m == orig);
  }
  writeln("-"*header.size);
}

proc main() {
  {
    var m : map(string,string);
    m.add("key1", "val1");
    m.add("key2", "val2");
    testMap(m);
  }
  {
    var stringR : map(string, R);
    stringR.add("red", new R(5, 42.0));
    stringR.add("blue", new R(9, 9.9));
    stringR.add("green", new R(3, 3.3));
    testMap(stringR);
  }
  {
    var m : map(string, Z);
    m.add("apple", new Z("red", new R(1, 0.31)));
    m.add("banana", new Z("yellow", new R(0, 0.68)));
    testMap(m);
  }
  {
    var x : map(string, int);
    x.add("A", 5);
    x.add("B", 42);

    var y : map(string, int);
    y.add("R", 99);
    y.add("S", 33);

    var m : map(string, map(string, int));
    m.add("x", x);
    m.add("y", y);

    testMap(m);
  }
  {
    var m : map(string, list(int));
    var x : list(int) = [1, 2, 3];
    var y : list(int) = [4, 5, 6];
    m.add("x", x);
    m.add("y", y);
    testMap(m);
  }
}
