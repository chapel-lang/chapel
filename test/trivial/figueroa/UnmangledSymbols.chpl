proc task {
  writeln ("This is a function called \"task\".");
  return 75;
}

proc chpl_begin (double) {
  writeln ("double = ", double);
}

proc _real64 (purge) {
  writeln ("purge = ", purge);
}

proc int32_t (task) {
  writeln ("task = ", task);
}

enum chpl_sync {aux_t};

var _string = task;
chpl_begin (chpl_sync.aux_t);
_real64 (_string);
int32_t (3.2);
