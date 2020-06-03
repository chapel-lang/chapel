class MyClass {
  var x:int;
}


proc test() {
  var o4 : owned MyClass? = new owned MyClass(4);

  var s4:shared  = o4;


  writeln(s4, " ", o4);
}

test();
