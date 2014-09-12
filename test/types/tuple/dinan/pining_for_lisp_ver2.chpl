class NilClass { }

proc Nil(): NilClass
  return nil;

proc isNil(x): bool {
  if x.type == NilClass then
    return true;
  else
    return false;
}

proc car(x: _tuple) ref: x(1).type where x.size == 2 {
  return x(1);
}

proc cdr(x: _tuple) ref: x(2).type where x.size == 2 {
  return x(2);
}

proc cons(x: ?T1, y: ?T2): (T1, T2) {
  return (x, y);
}

if isNil(cdr(cons(5, Nil()))) then
  writeln("isNil() test1 ok");
if !isNil(cdr(cons(Nil(), 5))) then
  writeln("isNil() test2 ok");
if !isNil(nil) then
  writeln("isNil() test3 ok");
  
var x = cons("Hello", " ");
var y = cons("World", "!");

writeln(car(x), cdr(x), car(y), cdr(y));

writeln(x, y);

writeln(cons("Hello", cons("World!", Nil())));
