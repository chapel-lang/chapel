record mytuple {
  param rank : int;
  var component ...rank;

  fun this(ind : int) {
    var x;
    for param i in 1..rank do
      if i == ind then
        x = component(i);
    return x;
  }

  fun =this(ind : int, x) {
    for param i in 1..rank do
      if i == ind then
        component(i) = x;
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

var x = mytuple(1, "hello world");

//writeln(x.component(1));
/*
x(1) = "good bye";

writeln(x);

var y = mytuple(4, 5, 6, 7, 8);

writeln(y);
*/