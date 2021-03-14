record R {
  var i:integral;
}

proc test() {
  var rec:R(int);
  writeln(rec.type:string, " ", rec);
}
test();
