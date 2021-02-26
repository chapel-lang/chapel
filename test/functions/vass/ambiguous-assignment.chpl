// ambiguous assignment

record R{}

proc =(ref r1:R, r2:R){}
proc =(ref r3:R, r4:R){}

var r5 = new R();
var r6 = new R();;
r5 = r6;
