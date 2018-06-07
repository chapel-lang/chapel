class C {
  var x:int;
}
var arr:[1..3] unmanaged C = for i in 1..3 do new unmanaged C(i);
writeln(arr);
for c in arr {
  delete c;
}
