use Map;

record MyRecord : hashable {
  var i = 1;

  proc hash() {
    writeln("called user hash");
    return i : uint;
  }
}

var m = new map(MyRecord, int);

var myR: MyRecord;

m[myR] = 5;
writeln("value stored: ", m[myR]);
