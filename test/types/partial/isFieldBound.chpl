
use Reflection;

record R {
  type T;
  param x : T;
  type U;
  param y : U;
}

proc main() {
  type T = R(int, U=real);

  for param i in 0..<numFields(T) {
    param name = getFieldName(T, i);
    writeln("T.", name, ": ", isFieldBound(T, name));
  }

  writeln("----- partial -----");
  writeln(T:string, " is partial? ", isGeneric(T));
  writeln(R:string, " is partial? ", isGeneric(R));
  type X = R(int, 5, real, 10.0);
  writeln(X:string, " is partial? ", isGeneric(X));
}
