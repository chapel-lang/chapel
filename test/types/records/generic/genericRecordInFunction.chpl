proc foo(args) {
  record R {
    var x: args.type;
  }
}
foo(1);
