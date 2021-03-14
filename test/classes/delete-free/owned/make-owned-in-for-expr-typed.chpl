class C {
  var x:int;
}
var arr:[1..3] owned C? = for i in 1..3 do new owned C?(i);
writeln(arr);
