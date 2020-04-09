use Reflection;

record myRecord { var x: int; }
enum myEnum { v=1 }

proc acceptsRecord(arg: record) {
  writeln(arg.type:string, " ", arg);
}

proc acceptsEnum(arg: enum) {
  writeln(arg.type:string, " ", arg);
}

proc main() {
  var r: myRecord;
  var i: int;
  var e: myEnum;

  acceptsRecord(r);
  assert(!canResolve("acceptsRecord", i));
  assert(!canResolve("acceptsRecord", e));
  
  assert(!canResolve("acceptsEnum", r));
  assert(!canResolve("acceptsEnum", i));
  acceptsEnum(e);
}
