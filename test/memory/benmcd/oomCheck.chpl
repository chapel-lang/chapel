use MemDiagnostics;

const size = here.physicalMemory()*2;

const dom = {0..#size};

var myArr = dom.tryCreateArray(int);
