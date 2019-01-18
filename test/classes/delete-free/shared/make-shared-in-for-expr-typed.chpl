class C {
  var x:int;
}
var arr:[1..3] shared C = for i in 1..3 do new shared C(i);
writeln(arr);
