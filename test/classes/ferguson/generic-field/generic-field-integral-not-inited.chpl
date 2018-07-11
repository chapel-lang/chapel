pragma "use default init"
class GenericClass {
  var f:integral;
}


proc test() {
  var x:borrowed GenericClass(int);

  writeln(x.type:string, " ", x);
}

test();
