use Lists;

config type listType = int;
config param listLock = true;

config const testIters = 8;

var lst = new list(listType, listLock);

try {
  lst.indexOf(0);
} catch e {
  writeln(e);
}

for i in 1..testIters do {
  lst.append(i);
  const idx = lst.indexOf(i);
  writeln(idx);
}

for i in 1..testIters by -1 do {
  const idx = lst.indexOf(i, i);
  writeln(idx);
}

lst.clear();

try {
  lst.indexOf(0);
} catch e {
  writeln(e);
}

