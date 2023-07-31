use MemDiagnostics;

const size = here.physicalMemory()*2;

const dom = {0..#size};

try {
  var myArr = dom.tryCreateArray(int);
} catch e {
  writeln("Array allocation failed; continuing anyway");
}

const otherDom = {0..#10};

var myArr = otherDom.tryCreateArray(int);
writeln(myArr);
