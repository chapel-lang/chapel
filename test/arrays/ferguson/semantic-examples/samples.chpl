public use myrecord;

printcopies = false;
var GlobalArray:[1..2] R;
var ArrTwo = [new R(2)];
var ArrTwoThree = [new R(2), new R(3)];
var one = new R(1);
var two = new R(2);
printcopies = true;

/*
class Globals {
  var GlobalArray:[1..2] R;
  var ArrTwo = [new R(2)];
  var ArrTwoThree = [new R(2), new R(3)];
  var one = new R(1);
  var two = new R(2);
}

// Set up some test variables
printcopies = false;
var g = new Globals();
printcopies = true;

proc GlobalArray ref {
  return g.GlobalArray;
}
proc ArrTwo ref {
  return g.ArrTwo;
}
proc ArrTwoThree ref {
  return g.ArrTwoThree;
}
proc one ref {
  return g.one;
}
proc two ref {
  return g.two;
}


record OnExit {
}

proc OnExit.deinit() {
  delete g;
}

var onexit:OnExit;
*/
