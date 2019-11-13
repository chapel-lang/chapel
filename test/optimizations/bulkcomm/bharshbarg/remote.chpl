use IO;

const dom = {1..10, 1..10};
const slice = {1..10, 1..5};
var A : [dom] int;

assert(numLocales >= 3, "Test requires at least three locales.");

proc test(dest, src) {
  dest = 1;
  src = 3;
  // Assumes we enter the strided transfer path
  dest[slice] = src[slice];
  forall idx in dest.domain {
    if slice.contains(idx) then assert(dest[idx] == 3);
    else assert(dest[idx] == 1);
  }
}


// A and B on locale 0
{
  writeln("\nA and B on same locale\n");
  var B : [dom] int;
  test(A, B);
  writeln();
  test(B, A);
  writeln("End of assignment\n");
}

// A on L0, B on L1
{
  writeln("\nB on different locale\n");
  on Locales[1] {
    var B : [dom] int;
    test(A, B);
    writeln();
    test(B, A);
  }
  writeln("End of assignment\n");
}

// A on L0, B on L1, assign from L2
{
  writeln("\nA and B on different locale\n");
  on Locales[1] {
    var B : [dom] int;
    on Locales[2] {
      test(A, B);
      writeln();
      test(B, A);
    }
  }
  writeln("End of assignment\n");
}
