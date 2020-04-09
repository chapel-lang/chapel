use BlockDist;
const D = {1..5} dmapped Block({1..5});

export proc makeBlockArray(): [D] int {
  var x: [D] int;
  return x;
}

export proc printBlock(x: [D] int) {
  var output = "";
  var first = true;
  for idx in x.dom {
    if (first) {
      first = false;
    } else {
      output += ", ";
    }
    output += idx: string + "=";
    output += x[idx]: string;
  }
  writeln(output);
}

export proc addEltBlock(x: [D] int, idx: int, val: int) {
  x[idx] = val;
}
