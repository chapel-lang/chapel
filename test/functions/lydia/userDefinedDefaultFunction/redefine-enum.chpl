enum bar {a=0, b=1, c=2};

inline proc _defaultOf(type t) param where t: bar {
  return bar.b;
}

var foo: bar;
writeln(foo);
