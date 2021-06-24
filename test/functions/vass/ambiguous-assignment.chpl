// ambiguous assignment

record R{}

operator =(ref r1:R, r2:R){}
operator =(ref r3:R, r4:R){}

var r5 = new R();
var r6 = new R();;
r5 = r6;
