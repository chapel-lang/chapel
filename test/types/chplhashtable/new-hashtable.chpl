use ChapelHashtable;

record mmap {
  type keyType;
  type valType;

  pragma "no doc"
  var table = new chpl__hashtable(keyType, valType);

  proc method() {
    for slot in table.allSlots() { }
  }
}

class C {
  var i: int;
}

var m: mmap(int, borrowed C);

m.method();
