use MemDiagnostics;

const size = here.physicalMemory()*3/4;

const dom = {0..#size};

var myArr = dom.tryCreateArray(int);

var cp = myArr.tryCopy();
