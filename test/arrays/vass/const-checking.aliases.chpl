// Ensure that 'var' aliases cannot alias 'const' arrays.

var   AAV: [1..2] int;
const AAC: [1..2] int;
var   DDV: domain(1);
const DDC: domain(1);

proc main {
  AAV = AAV;
  AAC = AAV;
  AAV[1] = 55;
  AAC[2] = 66;
  var aa1 => AAV;
  var aa2 => AAC;
  const cc1 => AAV;
  const cc2 => AAC;
  var dd1 => DDV;
  var dd2 => DDC;
  const ee1 => DDV;
  const ee2 => DDC;
  aa1[1] = 1111;
  aa2[1] = 1111;
  cc1[1] = 6666;
  cc2[1] = 6666;
  dd1 = DDC;
  dd2 = DDC;
  ee1 = DDV;
  ee2 = DDV;
}
