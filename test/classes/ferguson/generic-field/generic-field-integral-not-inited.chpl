pragma "use default init"
class GenericClass {
  var f:integral;
}


proc test() {
  var x:GenericClass(int);

  writeln(x.type:string, " ", x);
}

test();
