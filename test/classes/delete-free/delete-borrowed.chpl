class MyClass {
  var field:int;
}

var x = (new owned MyClass(1)).borrow();
delete x;
