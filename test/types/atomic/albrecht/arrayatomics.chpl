use Sort;

const n = 5;

var a: atomic int;

const D = 1..n;

var inputs: [D] int = [i in D] i;
var outputs: [1..n+1] int;
var diffs: [D] int;


//
// Longhand - just  to demonstrate what we want the shorthand do to
//

a.write(0);

coforall i in D {
  outputs[i] = a.fetchAdd(inputs[i]);
}

// Get remaining value in a
outputs[n+1] = a.read();

sort(outputs);

for i in D {
  diffs[i] = outputs[i+1] - outputs[i];
}

sort(diffs);

// 1 2 3 4 5
writeln(diffs);


// Zero out things for the real future test
outputs = 0;
diffs = 0;
a.write(0);

//
// Shorthand - this is what the future is testing for
//

outputs[D] = a.fetchAdd(inputs);

// Get remaining value in a
outputs[n+1] = a.read();

sort(outputs);

for i in D {
  diffs[i] = outputs[i+1] - outputs[i];
}

sort(diffs);

// 1 2 3 4 5
writeln(diffs);
