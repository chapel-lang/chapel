use BlockDist, CompressedSparseLayout;

config const n = 10;

const D = {1..n};
const D2 = D by 2;

const SD: sparse subdomain(D);
const SD2: sparse subdomain(D2);

const BD = D dmapped new blockDist({1..n});
const BSD: sparse subdomain(BD);

const SDCS: sparse subdomain({1..n, 1..n by 2}) dmapped new csrLayout();

if SD.strides != strideKind.one then
  compilerError("SD should not be stridable");

if SD2.strides != strideKind.positive then
  compilerError("SD2 should be stridable");

if SDCS.strides != strideKind.positive then
  compilerError("SD2CS should be stridable");

if BSD.strides != strideKind.one then
  compilerError("BSD should not be stridable");

writeln((SD.strides, SD2.strides, SDCS.strides, BSD.strides));
