use BlockDist;
use CommUtil;

config const n = 100000;

const Space = {1..n};
const D = Space dmapped Block(boundingBox=Space);
var A: [D] int = 1..n;
var B: [D] int = 0;

start();

B = A;

stop();

writeln(B[1]);
writeln(B[n]);

report(maxGets=0, maxPuts=0,
       maxOns = if CHPL_NETWORK_ATOMICS == 'none' then 2 else 1);
