proc foo(args) {
  record R {
    var x: args.type;
  }
  var r = new R();
  writeln(r.x);
}
foo(1);
