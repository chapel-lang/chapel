class C {
  var x:int;
}
var arr = forall i in 1..3 do new shared C(i);
writeln(arr);
