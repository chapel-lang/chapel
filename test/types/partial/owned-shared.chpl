
config param useOwned : bool = true;
config param mode : int = 0;

class Parent {
  type T;
  param n : int;
  var x : n*T;
}

class Child : Parent {
  type U;
  var y : U;
}

proc print(x) {
  writeln(x.type:string, ": ", x);
}

proc testOwned() {
  var a : owned Parent = new owned Parent(int, 3);
  print(a);
  var b : owned Parent(int) = new owned Parent(int, 3);
  print(b);

  var c : owned Parent = new owned Child(int, 3, U=real);
  print(c);
  var d : owned Child(int) = new owned Child(int, 3 , U=real);
  print(d);
}

proc testShared() {
  var a : shared Parent = new shared Parent(int, 3);
  print(a);
  var b : shared Parent(int) = new shared Parent(int, 3);
  print(b);

  var c : shared Parent = new shared Child(int, 3, U=real);
  print(c);
  var d : shared Child(int) = new shared Child(int, 3 , U=real);
  print(d);
}

proc main() {
  testOwned();
  testShared();
}
