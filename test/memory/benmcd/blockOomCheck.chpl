use BlockDist;
use MemDiagnostics;

const size = here.physicalMemory()*2;

const dom = blockDist.createDomain({0..#size});

var myArr = dom.tryCreateArray(int);
