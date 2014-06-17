class C {
  var classField: int;
  proc classMethod(i: int, j: int) {
    return (classField, i, j);
  }
}

proc main {
  var array: [0..1] C;
  [i in 0..1] array(i) = new C(i);
  writeln(array);
  array.classField = array.classField + 1;
  writeln(array);
  array.classField -= 1;
  writeln(array.classMethod[array.classField, array.classField]);
  [i in 0..1] delete array(i);
}
