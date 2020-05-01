var b1 = b"asd";
var b2 = b"ASD";

var b3 = b"asd";

// equality
writeln(b1==b2);  // false
writeln(b2==b1);  // false
writeln(b1!=b2);  // true
writeln(b2!=b1);  // true

// inequality
writeln(b1<b2);  // false
writeln(b2<b1);  // true
writeln(b1>b2);  // true
writeln(b2>b1);  // false

writeln(b1<=b2);  // false
writeln(b2<=b1);  // true
writeln(b1>=b2);  // true
writeln(b2>=b1);  // false

writeln(b1<=b3);  // true
writeln(b3<=b1);  // true
