record R { }

var r1 = new R();
var r2 = new R();

r1 <=> r2;

operator <=>(ref a: owned R, ref b: owned R) { }
