
use driver;

config const n = 5;

//
// In v1.14 there was a bug where resizing a privatized array from a remote
// locale (other than the locale on which the array was declared) would result
// in incorrect behavior when the RAD optimization was enabled. Essentially,
// the RAD data was not being recalculated after a resize.
//
// The problem turned out to be that ChapelArray wasn't calling dsiPostReallocate
// because it was looking for array objects in <privatized dom>._value._arrs.
// At the time of this bug, "._value" will return the local privatized class.
// This class does not contain the list of arrays for this domain. Instead,
// we should use "_instance" directly.
//
// Note: The direct-indexing in this test seems necessary for some reason.
// 

proc buildSpace(Dom) {
  if distType == DistType.default {
    return Dom;
  }
  else if distType == DistType.block {
    // Use a boundingBox smaller than the resize amount to expose the potential
    // bug.
    var bb = {1..3};
    return Dom dmapped Block(boundingBox=bb);
  }
  else if distType == DistType.cyclic {
    return Dom dmapped Cyclic(startIdx=1);
  }
  else if distType == DistType.replicated {
    return Dom dmapped Replicated();
  }
  else {
    compilerError("Compiling with unknown DistType.");
  }
}

proc main() {
  var Dom = {1..0};
  var Space = buildSpace(Dom);
  var A : [Space] int;

  Space = {1..n};
  for i in Space do A[i] = 42;

  on Locales[numLocales-1] {
    Space = {1..n*2};
  }
  for i in n+1..n*2 do A[i] = 99;

  for i in 1..n*2 {
    write(A[i], " ");
  }
  writeln();
}
