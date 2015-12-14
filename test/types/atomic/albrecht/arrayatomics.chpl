var a: atomic int;
const n = 5;
const D: domain(1) = {1..n};

var inputs: [D] int = [i in D] i;
var outputs: [D] int;

a.write(0);

// Long hand
for i in D {
  outputs[i] = a.fetchAdd(inputs[i]);
  writeln(outputs[i]);
}

// Clear outputs and 'a'
outputs = [i in D] 0;
a.write(0);

// Short hand (this is what the future is for)
outputs[D] = a.fetchAdd(inputs);
for i in D {
  writeln(outputs[i]);
}
