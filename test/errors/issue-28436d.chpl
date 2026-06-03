config type ErrorType = Error;
class SimpleError : Error {}

class R {
  type T;

  proc doit() {
    type locT = T;
    try! {
      throw new locT();
    } catch e: locT { // error: mismatched number of arguments in call to 'chpl_error'
      writeln("caught error of type ", T:string);
    }
  }
}
var r = new R(owned ErrorType); /* note: added owned */
r.doit();
