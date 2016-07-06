//this should test all of the arithmetic functions
use GMP;

var a = new BigInt(0);    // a = 0
var b = new BigInt(2);      // b = 2
var c = new BigInt(100);      // c = 100

a.add(a, b);              // a = 2
a.add_ui(a, 1000);        // a = 1002
writeln(a);

a.sub(a, c);              // a = 902
a.sub_ui(a, 10);          // a = 892
a.ui_sub(1000, a);        // a = 108
writeln(a);

a.mul(a, b);              // a = 216
c.mul_si(c, -2);              // c = -200
a.mul_ui(a, 3);           // a = 648
writeln(c);
writeln(a);

// a = a + (b * c)
a.addmul(b, c);           // a = 248
a.addmul_ui(b, 10);       // a = 268
writeln(a); 

// a = a - (b * c)
a.submul(b, c);           // a = 668
a.submul_ui(b, 4);        // a = 660
writeln(a);

a.mul_2exp(c, 3);         // a = -1600;
writeln(a);
a.neg(b);                 // a = -2
writeln(a);

a.abs(a);                 // a = 2;
writeln(a);
