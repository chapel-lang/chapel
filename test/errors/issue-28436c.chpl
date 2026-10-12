config type ErrorType = Error;
class SimpleError : Error {}

proc foo(type T) {
  try! {
    throw new T();
  } catch e: T {
    writeln("caught error of type ", T:string);
  }
}
foo(ErrorType);
