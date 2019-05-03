require "bar.h";

extern record R {
  var b: int;
}

export proc printsArg(x: R) {
  writeln(x.b);
}
