config const sourceText = "<a><ii>end</ii><none /></a>";
const AllIndices: domain(1) = [1..length(sourceText)];
const AllPairs: domain(2) = [1..length(sourceText),
                             1..length(sourceText)];
var StartIndices: sparse subdomain(AllIndices);
var EndIndices: sparse subdomain(AllIndices);

def main {
  forall z in AllIndices do {
    if sourceText.substring[z] == '<' then {
      StartIndices += z;
      if z > 1 && sourceText.substring[z-1] != ">" then
      EndIndices += z-1;
    }
    else if sourceText.substring[z] == '>' then {
      EndIndices += z;
      if z < length(sourceText) &&
      sourceText.substring[z+1] != "<"  then StartIndices +=
      z+1;
    }
  }

  var A: [AllPairs] real;
  end {
    for (x,y) in [StartIndices, EndIndices] {
      begin {
        if (x <= y) {
          A[x, y] = x + y/10.0;
        }	 
      }
    }
  }
  writeln("A is:\n", A);
  return;
}

