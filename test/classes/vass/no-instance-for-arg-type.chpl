// The class type of an argument has no instances.

class Monkey1 {}
proc proc1(arg: Monkey1?) { writeln("in proc1"); }
proc1(nil);

class Monkey2 {}
var var2: Monkey2? = nil;

class Monkey3 {}
var var3: Monkey3? = nil;
writeln(var3);

class Monkey4 {}
class Horse4: Monkey4 {}
proc proc4(arg: Monkey4?) { writeln("in proc4"); }
proc4(nil);

class Monkey5 {}
class Horse5: Monkey5 {}
proc proc5(arg: Horse5?) { writeln("in proc5"); }
proc5(nil);

class Monkey6 {}
class Horse6 {
  proc proc6(arg: Monkey6?) { writeln("in proc6"); }
}
(new unmanaged Horse6()).proc6(nil);

class Monkey7 { type T7; }
proc proc7(arg: Monkey7?) { writeln("in proc7"); }
proc7(nil: Monkey7(int)?);
