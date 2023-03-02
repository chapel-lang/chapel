// This fails a compiler assertion as of this writing

var a=1, b=2;
var c: [1..3] int;

proc test(tup) {
  tup = (7,8);
}
test((a,c));
