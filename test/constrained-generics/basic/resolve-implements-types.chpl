/*
This test checks the situation where the actual argument
of an 'implements' statement needs to be resolved.
*/

interface MyArray {
  proc Self.reverse();
  proc writeln(arg: Self);
}

proc cgFun(arg: MyArray) {
  arg.reverse();
  writeln(arg);
}

var AR = [1, 2, 5];
proc f(arg1: real, arg2: string) return AR;
implements MyArray(f(1, 2:string).type);
cgFun(AR);
