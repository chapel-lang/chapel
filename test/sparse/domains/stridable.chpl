config const n = 10;

const D = {1..n};
const D2 = D by 2;

const SD: sparse subdomain(D);
const SD2: sparse subdomain(D2);

if SD.stridable then
  compilerError("SD should not be stridable");

if !SD2.stridable then
  compilerError("SD2 should be stridable");

writeln((SD.stridable, SD2.stridable));
