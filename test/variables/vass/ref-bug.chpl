
var myArr: [1..3] int = 51;

// alias refArr1 is not used as lvalue
ref refArr1 = myArr[1];
myArr[1] = 62;
writeln("refArr1 = ", refArr1, "  myArr[1] = ", myArr[1]);

// alias refArr2 IS used as lvalue
ref refArr2 = myArr[2];
refArr2 = 62;
writeln("refArr2 = ", refArr2, "  myArr[2] = ", myArr[2]);
