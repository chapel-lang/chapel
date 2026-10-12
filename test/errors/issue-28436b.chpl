config type ErrorType = Error;
class SimpleError : Error {}

record R {
  type T;

  proc doit() {
    try! {
      throw new T();
    } catch e: T {
      writeln("caught error of type ", T:string);
    }
  }
}
var r = new R(owned ErrorType); /* note: added owned */
r.doit();
