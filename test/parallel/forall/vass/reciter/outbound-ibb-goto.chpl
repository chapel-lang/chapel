// from #18218
// also, the following test keeps a snapshot of findfiles() et al.
// that exhibited an undesired behavior:
//   test/functions/iterators/recursive/recursive-iter-findfilesfailure.chpl

module CheckHttp {
  use FileSystem;
  use IO;
  config const flag = false;

  proc throwingFunction(f: string) throws {
    writeln(f);
    if flag then throw new Error();
  }

  proc main() throws {
    for f in findfiles(".") do throwingFunction(f);
    writeln("---");
    forall f in findfiles(".") do throwingFunction(f);
  }
}
