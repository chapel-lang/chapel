use List;

class MyClass { var x: int; }

proc acceptListC(lst: list(MyClass)) {
  writeln(lst);
}

var lst: list(owned MyClass);
lst.append(new MyClass(1));
acceptListC(lst);
