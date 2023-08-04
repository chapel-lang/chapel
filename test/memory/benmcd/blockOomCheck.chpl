use BlockDist;
use MemDiagnostics;

const size = here.physicalMemory()*2;

const dom = Block.createDomain({0..#size});

var myArr = dom.tryCreateArray(int);
