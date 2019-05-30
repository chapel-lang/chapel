use Lists;

type listType = int;
param listLock = true;

var lst: List(listType, listLock);


for i in 1..8 do {
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


