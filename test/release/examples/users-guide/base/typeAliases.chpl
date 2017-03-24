type age = uint(8);
var mgAge: age = 10,
    votingAge: age = 18;

proc birthday(ref a: age) {
  a += 1;
}

birthday(mgAge);

writeln(mgAge);
writeln(votingAge);

type float = real(32),
     double = real(64);

var f: float;
var d: double;

writeln(f);
writeln(d);

type vec = 3*real,
     bigvec = [1..100] real;

var V: vec,
    V2: bigvec;

writeln(V);
writeln(V2);

