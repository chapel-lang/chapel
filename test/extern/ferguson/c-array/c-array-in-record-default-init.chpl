use CPtr;
record R {
  var dat: c_array(int, 3);
}
proc test1() {
  var rec: R;
  
  writeln(rec);
  
  rec.dat[0] = 100;
  rec.dat[1] = 200;
  rec.dat[2] = 300;

  writeln(rec);
}
test1();
