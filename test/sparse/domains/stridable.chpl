use BlockDist, LayoutCS;

config const n = 10;

const D = {1..n};
const D2 = D by 2;

const SD: sparse subdomain(D);
const SD2: sparse subdomain(D2);

const BD = D dmapped Block({1..n});
const BSD: sparse subdomain(BD);

const SDCS: sparse subdomain({1..n, 1..n by 2}) dmapped CS();

if SD.stridable then
  compilerError("SD should not be stridable");

if !SD2.stridable then
  compilerError("SD2 should be stridable");

if !SDCS.stridable then
  compilerError("SD2CS should be stridable");

if BSD.stridable then
  compilerError("BSD should not be stridable");

writeln((SD.stridable, SD2.stridable, SDCS.stridable, BSD.stridable));
