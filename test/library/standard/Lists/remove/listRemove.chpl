use Lists;

config const testElem = 42;

type listType = int;
param listLock = true;

var lst: List(listType, listLock);

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

