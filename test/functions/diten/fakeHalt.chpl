proc halt(a: int) {
  writeln("fake halt");
}

proc canFakeReturn(n: int) {
  if n > 0 then
    return 1;
  else
    halt(n);
}

proc killStack(a,b,c,d,e) {
  writeln((a,b,c,d,e));
}

var v1 = canFakeReturn(1);
killStack(3,2,1,2,3);
var v2 = canFakeReturn(0);

writeln(v1);
writeln(v2);
