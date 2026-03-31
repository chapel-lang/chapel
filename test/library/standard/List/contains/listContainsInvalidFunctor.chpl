private use List;

record NotAFunctor {
  var x: int;
}

var lst = new list(int);
lst.pushBack(1);

lst.contains(new NotAFunctor(42));
