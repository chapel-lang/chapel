proc foo(paths: string ...?n) : string {
  var ret : string;

  for p in paths do writeln(p);

  return ret;
}

proc main() {
  // some dummy strings built up to emphasize this isn't an issue with string
  // literals
  var x, y : string;
  for i in 0..9 do x += i:string;
  for i in 0..9 by -1 do y += i:string;

  foo(x, y);
}
