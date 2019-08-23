use List;

config const testElem = 42;
config const testIters = 1024;

type listType = int;
param listLock = true;

var lst: list(listType, listLock);

for i in 1..8 do {
  lst.append(i);
}

writeln(lst);

for i in 5..8 do {
  lst.insert(i, testElem);
}

writeln(lst);

while true do
  try {
    lst.remove(testElem);
    writeln(lst);
  } catch {
    break;
  }

try {
  lst.remove(testElem);
} catch e {
  writeln(e);
}

