use Lists;

type listType = int;
param listLock = true;
config const testIters = 16;

var lst: List(listType, listLock);


for i in 1..testIters do {
  const elem = i:listType;
  lst.append(elem);
  lst.insert(1, elem);
}

writeln(lst);

try {
  lst.insert(0, 42:listType);
} catch e {
  writeln(e);
}

try {
  lst.insert((lst.size + 1), 42:listType);
} catch e {
  writeln(e);
}

lst.clear();

try {
  lst.insert(1, 42:listType);
} catch e {
  writeln(e);
}


