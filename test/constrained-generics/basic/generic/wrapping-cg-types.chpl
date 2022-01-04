// In this test, the return type of a required function
// is a generic record wrapping an interface type.

record RR {
  var vv;
}

interface IFC {
  proc reqFn(arg: Self): RR(Self);
}

RR implements IFC;

proc reqFn(arg): RR(arg.type) {
  writeln("reqFn(", arg, ")");
  return new RR(arg);
}

implements IFC(RR(int));

proc icFun(arg: ?Q) where Q implements IFC {
  writeln("icFun");
  reqFn(arg);
}

icFun(new RR("hello"));
