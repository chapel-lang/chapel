
interface IFC {
  proc req(const ref arg: Self): void;
}

int implements IFC;

proc req(ref arg: int) {
  const prev = arg;
  arg += 10;
  writeln("req.int  ", prev, " -> ", arg);
}
