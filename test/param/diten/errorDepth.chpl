proc foo(param d: int) {
  if d == 0 then
    compilerError("error 0", 0);
  else if d == 1 then
    compilerError("error 1", 1);
  else if d == 2 then
    compilerError("error 2", 2);
}

proc bar(param d: int) { foo(d); }
proc baz(param d: int) { bar(d); }

config param depth = 0;

proc main { baz(depth); }
