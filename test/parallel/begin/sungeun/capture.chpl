config const n = 2000;
proc work(i) {
  writeln(i);
}

{
  var i = 0;
  while i < n {
      begin work(i);
      i += 1;
    }
}
