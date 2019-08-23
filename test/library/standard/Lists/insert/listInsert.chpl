use List;

type listType = int;
param listLock = true;
config const testIters = 16;
config const testElem = 42;

var lst: list(listType, listLock);

for i in 1..testIters do {
  const elem = i:listType;
  lst.append(elem);
  lst.insert(1, elem);
}

writeln(lst);

// Insert at zero should fail.
try {
  lst.insert(0, testElem:listType);
} catch e {
  writeln(e);
}

// Insert at size +1 should always work.
try {
  lst.insert((lst.size + 1), testElem:listType);
} catch e {
  writeln(e);
}

writeln(lst);

lst.clear();

// Insert at size +1 should always work.
try {
  lst.insert(1, testElem:listType);
} catch e {
  writeln(e);
}

writeln(lst);

