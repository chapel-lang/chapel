
use IO;
use List;

record R {
  var x : int;
  var y : real;
}

record Z {
  var z : string;
  var r : R;
}

proc testList(orig: list(?)) {
  const header = "--- " + orig.type:string + " ---";
  writeln(header);
  var f = openMemFile();
  {
    f.writer().writef("%jt", orig);
    writeln("Writing list: ");
    writeln(orig);
    writeln();
    writeln("Wrote JSON: ");
    writef("%jt\n", orig);
    writeln();
  }
  {
    var r = f.reader();
    var li : orig.type;
    r.readf("%jt", li);
    writeln("Read ", li.type:string, ": ");
    writeln(li);
    writeln("Equals original: ", li == orig);
  }
  writeln("-"*header.size);
}

proc main() {
  {
    var li : list(int);
    for i in 1..4 do li.append(i);
    testList(li);
  }
  {
    var li : list(string);
    for i in 1..4 do li.append("val" + i:string);
    testList(li);
  }
  {
    var li : list(R);
  }
  {
    var li : list(R);
    li.append(new R(5, 42.0));
    li.append(new R(9, 9.9));
    li.append(new R(3, 3.3));
    testList(li);
  }
  {
    var li : list(Z);
    li.append(new Z("red", new R(1, 0.31)));
    li.append(new Z("yellow", new R(0, 0.68)));
    testList(li);
  }
}
