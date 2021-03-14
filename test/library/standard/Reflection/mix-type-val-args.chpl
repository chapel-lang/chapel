proc foo(type t, x) {
  writeln(t: string);
  writeln(x);
}
use Reflection only ;
writeln(Reflection.canResolve("foo", int, 3));
