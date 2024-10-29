use Python;
import Reflection;


proc roundTripFunction(func: borrowed) {

  proc testType(type t, value: t) {
    writeln("  type: ", t:string, " value: ", value);
    var res = func(t, value);
    writeln("    res: ", res);
  }

  testType(int, 42);
  testType(string, "hello");

}
proc roundTripClass(clsType: borrowed) {

  proc testType(type t, value: t, other: t) {
    writeln("  type: ", t:string, " value: ", value);
    var obj = new ClassObject(clsType, value);
    writeln("    obj: ", obj);
    writeln("    obj.value: ", obj.getAttr(t, "value"));
    obj.setAttr("value", other);
    writeln("    obj.getter(): ", obj.callMethod(t, "getter"));
    obj.callMethod(None, "setter", value);
    writeln("    obj.value: ", obj.getAttr(t, "value"));

  }

  testType(int, 42, 43);
  testType(string, "hello", "world");

}

proc main() {
  var interp = new Interpreter();

  var modName = Reflection.getModuleName();
  var m = new Module(interp, modName);
  writeln("module: ", m);

  var func = new Function(m, "round_trip");
  writeln("func: ", func);
  roundTripFunction(func);

  var clsType = new Class(m, "RoundTrip");
  writeln("class: ", clsType);
  roundTripClass(clsType);

}
