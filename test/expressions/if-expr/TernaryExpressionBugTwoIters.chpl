proc test1() {
  var n = 0;
  var arr = if n then for i in 0..3 do i else for i in 4..7 do i;
  writeln(arr);
}

proc main() {
  test1();
}
