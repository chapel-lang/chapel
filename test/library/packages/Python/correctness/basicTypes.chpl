use Python;
import Reflection;
import List;
import Set;

config const print = false;


proc roundTripFunction(func: borrowed) {

  proc testType(type t, value: t) {
    if print then writeln("  type: ", t:string, " value: ", value);
    var res = func(t, value);
    if print then writeln("    res: ", res);
    assert(res == value);
  }

  testType(int(8), 42);
  testType(int(16), 42);
  testType(int(32), 42);
  testType(int(64), 42);
  testType(uint(8), 42);
  testType(uint(16), 42);
  testType(uint(32), 42);
  testType(uint(64), 42);
  testType(real(32), 3.14);
  testType(real(64), 3.14);
  testType(string, "hello");
  testType(bool, true);
  testType(bytes, b"hello");

  var arr: [0..<10] int = 0..<10;
  testType(arr.type, arr);
  var l = new List.list(arr);
  testType(l.type, l);
  var s = new Set.set(int);
  for i in arr {
    s.add(i);
  }
  testType(s.type, s);

}
proc roundTripClass(clsType: borrowed) {

  proc testType(type t, value: t, other: t) {
    var res: t;

    if print then writeln("  type: ", t:string, " value: ", value);
    var obj = clsType(value);
    if print then writeln("    obj: ", obj);

    res = obj.get(t, "value");
    if print then writeln("    obj.value: ", res);
    assert(res == value);

    obj.set("value", other);
    res = obj.call(t, "getter");
    if print then writeln("    obj.getter(): ", res);
    assert(res == other);

    obj.call("setter", value);
    res = obj.get(t, "value");
    if print then writeln("    obj.value: ", res);
    assert(res == value);

  }

  testType(int(8), 42, 43);
  testType(int(16), 42, 43);
  testType(int(32), 42, 43);
  testType(int(64), 42, 43);
  testType(uint(8), 42, 43);
  testType(uint(16), 42, 43);
  testType(uint(32), 42, 43);
  testType(uint(64), 42, 43);
  testType(real(32), 3.14, 2.71);
  testType(real(64), 3.14, 2.71);
  testType(string, "hello", "world");

  var arr: [0..<10] int = 0..<10;
  var otherArr = [arr.domain] 2;
  testType(arr.type, arr, otherArr);
  var l = new List.list(arr);
  testType(l.type, l, new List.list([arr.domain] 2));
  var s = new Set.set(int);
  for i in arr {
    s.add(i);
  }
  testType(s.type, s, new Set.set(int));

}

proc main() {
  var interp = new Interpreter();

  var modName = Reflection.getModuleName();
  var m = interp.importModule(modName);
  if print then writeln("module: ", m);

  var func = m.get("round_trip");
  if print then writeln("func: ", func);
  roundTripFunction(func);

  var clsType = m.get("RoundTrip");
  if print then writeln("class: ", clsType);
  roundTripClass(clsType);

}
