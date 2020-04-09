class C {
  var x:int;
}
var arr = for i in 1..3 do new shared C(i);
writeln(arr);
