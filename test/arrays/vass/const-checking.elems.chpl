// Ensure that 'ref' aliases cannot alias 'const' arrays.

var   AAV: [1..2] int;
const AAC: [1..2] int;
var   DDV: domain(1);
const DDC: domain(1);

proc main {
  AAV = AAV;
  AAC = AAV;
  AAV[1] = 55;
  AAC[2] = 66;
  ref aa1 = AAV;
  const ref aa2 = AAC;
  const ref cc1 = AAV;
  const ref cc2 = AAC;
  ref dd1 = DDV;
  const ref dd2 = DDC;
  const ref ee1 = DDV;
  const ref ee2 = DDC;
  aa1[1] = 1111;
  aa2[1] = 1111;
  cc1[1] = 6666;
  cc2[1] = 6666;
  dd1 = DDC;
  dd2 = DDC;
  ee1 = DDV;
  ee2 = DDV;
}
