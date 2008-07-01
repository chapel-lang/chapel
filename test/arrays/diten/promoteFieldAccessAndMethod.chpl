class C {
  var classField: int;
  def classMethod(i: int, j: int) {
    return (classField, i, j);
  }
}

def main {
  var array: [0..1] C;
  [i in 0..1] array(i) = new C(i);
  writeln(array);
  array.classField = array.classField + 1;
  writeln(array);
  array.classField -= 1;
  writeln(array.classMethod[array.classField, array.classField]);
}
