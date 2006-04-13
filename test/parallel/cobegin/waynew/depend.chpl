var a = 0;
var b = 0;
var c = 0;
var d = 0;

fun jam_records1 () {
  while (0==a) {;}   // wait on thread last statement in cobegin
  b = 1;
}

fun jam_records2 () {
  while (0==b) {;}   // wait on thread 1
  c = 1;
}

fun jam_records3 () {
  while (0==c) {;}   // wait on thread 2
  d = 1;
}

cobegin {
  jam_records3();
  jam_records2();
  jam_records1();
  a = 1;
}

write ("d is ");
writeln (d);
