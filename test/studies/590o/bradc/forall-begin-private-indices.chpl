config const sourceText = "<a><ii>end</ii><none /></a>";
const AllIndices: domain(1) = {0..<sourceText.size};
const AllPairs: domain(2) = {0..<sourceText.size,
                             0..<sourceText.size};
var StartIndices: sparse subdomain(AllIndices);
var EndIndices: sparse subdomain(AllIndices);
var lock: sync int = 0;

proc main {
  forall z in AllIndices with (ref StartIndices, ref EndIndices) do {
    if sourceText[z] == '<' then {
      lock;
      StartIndices += z;
      if z > 0 && sourceText[z-1] != ">" then
      EndIndices += z-1;
      lock = 0;
    }
    else if sourceText[z] == '>' then {
      lock;
      EndIndices += z;
      if z < (sourceText.size-1) &&
      sourceText[z+1] != "<"  then StartIndices +=
      z+1;
      lock = 0;
    }
  }

  var A: [AllPairs] real;
  sync {
    for x in StartIndices {
      for y in EndIndices {
        begin {
          if (x <= y) {
            A[x, y] = (x+1) + (y+1)/10.0;
          }
        }
      }
    }
  }
  writeln("A is:\n", A);
  return;
}

