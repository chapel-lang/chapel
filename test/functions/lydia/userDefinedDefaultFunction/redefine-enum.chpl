enum bar {a=0, b=1, c=2};

inline proc _defaultOf(type t: bar) param {
  return bar.b;
}

var foo: bar;
writeln(foo);
