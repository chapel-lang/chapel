use BlockDist, LayoutCS;

config const n = 10;

const D = {1..n};
const D2 = D by 2;

const SD2: sparse subdomain(D2);
const SD22: sparse subdomain(SD2);

const BD = D2 dmapped Block({1..n});
const BSD: sparse subdomain(BD);
const BSD2: sparse subdomain(BSD);

if !SD2.stridable then
  compilerError("SD2 should be stridable");

if !SD22.stridable then
  compilerError("SD22 should be stridable");

if !BSD.stridable then
  compilerError("BSD should be stridable");

if !BSD2.stridable then
  compilerError("BSD2 should be stridable");

writeln((SD2.stridable, SD22.stridable, BSD.stridable, BSD2.stridable));
