/* How to check generic implements statements? #16820 */

module Lib
{
  interface IFC {
    proc reqFun(arg: Self);
  }

  record R {
    type T;
  }

  private proc reqFun(arg: R): void {
    writeln("reqFun argument of type ", arg.type: string);
    if arg.T == string then
      compilerError("some error here");
  }

  R implements IFC;  // should this succeed or ... ?
}

module App
{
  use Lib;

  proc cgFun(arg: ?Q) where Q implements IFC {
    writeln("in cgFun...");
    reqFun(arg);
  }

  proc main {
    var myR: R(int);
    cgFun(myR);
  }
}
