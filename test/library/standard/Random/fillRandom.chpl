use Random;

var a : [0..<10] int;
var b : [0..<10] real;
var c : [0..<10] real(32);
var d : [0..<10] uint;

// Correct types, no coercion
fillRandom(a, min=-1, max=1);
fillRandom(b, min=-1.0, max=1.0);

// Coercion
fillRandom(b, min=-1, max=1);
fillRandom(c, min=-1, max=1);
fillRandom(d, min=-1, max=1);


// Illegal
// fillRandom(a, min=-1.0, max=1.0);
