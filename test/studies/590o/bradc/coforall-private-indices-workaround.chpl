config const sourceText = "<a><ii>end</ii><none /></a>";
const AllIndices: domain(1) = {1..(sourceText.length)};
const AllPairs: domain(2) = {1..(sourceText.length),
                             1..(sourceText.length)};
var StartIndices: sparse subdomain(AllIndices);
var EndIndices: sparse subdomain(AllIndices);
var lock: sync int = 0;

proc main {
  forall z in AllIndices with (ref StartIndices, ref EndIndices) do {
    if sourceText.substring[z] == '<' then {
      lock;
      StartIndices += z;
      if z > 1 && sourceText.substring[z-1] != ">" then
      EndIndices += z-1;
      lock = 0;
    }
    else if sourceText.substring[z] == '>' then {
      lock;
      EndIndices += z;
      if z < (sourceText.length) &&
      sourceText.substring[z+1] != "<"  then StartIndices += z+1;
      lock = 0;
    }
  }

  var A: [AllPairs] real;
  coforall x in StartIndices do {
    coforall y in EndIndices do {
      if (x <= y) {
        A[x, y] = x + y/10.0;
      }  
    }
  }
  writeln("A is:\n", A);
  return;
}

