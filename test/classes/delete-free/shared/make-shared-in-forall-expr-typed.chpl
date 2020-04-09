class C {
  var x:int;
}
var arr:[1..3] shared C = forall i in 1..3 do new shared C(i);
writeln(arr);
