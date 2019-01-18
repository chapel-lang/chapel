// Ensure that 'ref' aliases cannot alias 'const' arrays.

var   AAV: [1..2] int;
const AAC: [1..2] int;
var   DDV: domain(1);
const DDC: domain(1);

proc main {
  ref aa1 = AAV;
  ref aa2 = AAC;
  const ref cc1 = AAV;
  const ref cc2 = AAC;
  ref dd1 = DDV;
  ref dd2 = DDC;
  const ref ee1 = DDV;
  const ref ee2 = DDC;
}
