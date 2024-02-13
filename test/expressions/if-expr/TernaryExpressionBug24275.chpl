proc test1() {
  var arr1: [0..3] int;
  var n = 1;
  var arr2 = if n then for i in 0..3 do i else arr1;
  writeln(arr2);
}

proc main {
  test1();
}
