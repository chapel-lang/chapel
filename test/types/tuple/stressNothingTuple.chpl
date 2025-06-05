
proc foo(args...) {
  bar((...args));
}

proc bar(args...) {
  callme(args);
}

proc callme(args) {
  writeln("calling on: ", args.type:string);
  for param x in 0..<args.size {
    var t = args[x];
    writeln("got t ", t.type:string);
  }
}

for param i in 1..5 {
  var t: nothing;
  var tup: i*nothing;
  var tuptup: i*tup.type;

  foo(t);
  foo(tup);
  foo(tuptup);
}

