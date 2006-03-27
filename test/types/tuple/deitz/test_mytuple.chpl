record mytuple {
  param rank : int;
  var component ...rank;

  fun this(param i : int) var {
    return component(i);
  }

  fun this(i : int) var {
    return component(i);
  }
}

fun fwrite(f : file, t : mytuple) {
  fwrite(f, "(", t.component(1));
  for param i in 2..t.rank do
    fwrite(f, ", ", t.component(i));
  fwrite(f, ")");
}    

fun =(t : mytuple, x : mytuple) {
  for param i in 1..t.rank do
    t.component(i) = x.component(i);
  return t;
}

var x = mytuple(2, "hello world", 3.14);

writeln(x);

x(1) = "good bye";

writeln(x);

var y = mytuple(4, 5, 6, 7, 8);

writeln(y);
