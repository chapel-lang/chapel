use MemDiagnostics;

const size = here.physicalMemory()*2;

const dom = {0..#size};

try {
  var myArr = dom.tryBuildArray(int);
} catch e {
  writeln("Array allocation failed; continuing anyway");
}

const otherDom = {0..#10};

var myArr = otherDom.tryBuildArray(int);
writeln(myArr);
