use List;

class MyClass { var x: int; }

proc acceptListC(lst: list(MyClass)) {
  writeln(lst);
}

{
  var lst: list(owned MyClass);
  lst.pushBack(new MyClass(1));
  acceptListC(lst);
}

proc acceptListI(lst: list(integral)) {
  writeln(lst);
}

{
  var lst: list(int);
  lst.pushBack(1);
  acceptListI(lst);
}
