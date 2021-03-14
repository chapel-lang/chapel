use BlockDist;
const D = {1..5} dmapped Block({1..5});

export proc makeBlockArray(): [D] int {
  var x: [D] int;
  return x;
}

// Will generate a different signature than the return type for the previous
// function
export proc printBlock(const ref x: [] int) {
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
