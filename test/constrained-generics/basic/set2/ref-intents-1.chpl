
interface IFC {
  proc req(ref arg: Self): void;
  proc write(arg: Self): bool;
}

int implements IFC;

proc req(ref arg: int) {
  const prev = arg;
  arg += 10;
  writeln("req.int  ", prev, " -> ", arg);
}

proc test(ref TA: ?Q) where Q implements IFC {
  write("test.IFC "); write(TA); writeln();
  req(TA);
  write("test.IFC   -> "); write(TA); writeln();
}

var xx = 9;
test(xx);
