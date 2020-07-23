require "bar.h";

extern record R {
  var b: int;
}

export proc printsArg(const ref x: R) {
  writeln(x.b);
}
