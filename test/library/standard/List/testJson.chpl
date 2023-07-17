
use IO;
use List;
use Map;

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
    for i in 1..4 do li.pushBack(i);
    testList(li);
  }
  {
    var li : list(string);
    for i in 1..4 do li.pushBack("val" + i:string);
    testList(li);
  }
  {
    var li : list(R);
  }
  {
    var li : list(R);
    li.pushBack(new R(5, 42.0));
    li.pushBack(new R(9, 9.9));
    li.pushBack(new R(3, 3.3));
    testList(li);
  }
  {
    var li : list(Z);
    li.pushBack(new Z("red", new R(1, 0.31)));
    li.pushBack(new Z("yellow", new R(0, 0.68)));
    testList(li);
  }
  {
    var li : list(list(int));
    var x : list(int) = [1, 2, 3];
    var y : list(int) = [4, 5, 6];
    li.pushBack(x);
    li.pushBack(y);
    testList(li);
  }
  {
    var li : list(map(string, int));

    var x : map(string,int);
    x.add("A", 1);
    x.add("B", 2);

    var y : map(string,int);
    y.add("X", 3);
    y.add("Y", 4);

    li.pushBack(x);
    li.pushBack(y);
    testList(li);
  }
}
