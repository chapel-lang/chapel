
proc foo(r : range(?,?,?)) {
  writeln(r.type:string);
  writeln(r);
}

foo(1..10 by 3);
foo(1:uint..100:uint);

proc bar(r : range(?, ?, ?S)) {
  writeln(r.type:string);
  writeln(r);
}

bar(1..10);
bar(1..10 by 3);
