require "bar.h";

extern record R {
  var b: int;
}

export proc printsArg(in x: R) {
  writeln(x.b);
}
