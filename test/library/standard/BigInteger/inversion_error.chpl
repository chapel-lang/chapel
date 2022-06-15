use BigInteger;

// inverse exists
var a = new bigint(0);
var b = new bigint(5);
var c = new bigint(2);

// a = (b % c)^(-1)
// a = (5 % 2)^(-1)
// a = (1)^(-1)
// a = 1
a.invert(b, c);

writeln("Inverse Exists Case:");
writeln("\tcorrect inverse: ", a == 1);

// inverse does not exist (error case)
var d = new bigint(0);
var e = new bigint(100);
var f = new bigint(10);

writeln("Inverse Does Not Exist Case:");
try {
    // d = (e % f)^(-1)
    // d = (100 % 10)^(-1)
    // d = (0)^(-1)
    // d = undefined
    d.invert(e, f);    
} catch e: InversionError {
    writeln("\tcaught: ", e);
} catch {
    writeln("\tcaught unexpected error");
}
