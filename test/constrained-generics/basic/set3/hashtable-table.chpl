// This is a small reproducer of a problem with --no-local compilations
// of rev. fe4b408912 of these two files when compiled together:
//   test/constrained-generics/hashtable/MyHashtable.chpl
//   test/constrained-generics/hashtable/test-chpl-hashtable.chpl

record my__hashtable {
  var table: _ddata(int) = _ddata_allocate(int, 2, initElts=true);
}

interface chpl_Hashtable(HT) {
  proc HT.table ref: tableType;

  // represents '_ddata(int)'
  type tableType;
  proc tableType.this(idx: int) ref: int;

  // to support updateTable()-like code
  proc chpl__initCopy(arg: tableType, definedConst: bool): tableType;
  operator =(ref lhs: tableType, rhs: tableType);
}

my__hashtable implements chpl_Hashtable;

proc my__hashtable.tableType type return _ddata(int);

// We need to support assignment to 'table'.
proc chpl_Hashtable.updateTable() {
  var oldTable = table;
  table = oldTable;
}

// concrete function
proc testConcrete(ref h: my__hashtable) {
  ref entry = h.table[1];
  writeln(entry);
}

// interface-generic function
proc testIC(ref h: chpl_Hashtable) {
  ref entry = h.table[1];
  writeln(entry);
}

var h1 = new my__hashtable();
h1.updateTable();
testConcrete(h1);
testIC(h1);
