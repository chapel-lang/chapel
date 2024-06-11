var a = 0;
var b = 0;
var c = 0;
var d = 0;

var x = a < b < c;
var y = (a < b) < c;
var z = a < (b < c);

var x1 = a < b <= c;
var y1 = (a < b) < c;
var z1 = a < (b < c);

var x2 = a > b <= c;
var y2 = (a > b) < c;
var z2 = a < (b > c);

var x3 = a == b == c;
var y3 = (a == b) == c;
var z3 = a == (b == c);

var x4 = a != b != c;
var y4 = (a != b) != c;
var z4 = a != (b != c);

var x = a < b < c < d;
var x1 = a < b <= c <= d;
