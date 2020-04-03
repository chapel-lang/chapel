var globalArray : [1..4] int;
config const n = 1;

proc getter() ref {
  writeln("REF");
  return globalArray;
}

proc getter() const ref {
  writeln("CONST REF");
  return globalArray;
}

proc tupleIndexFor(A, B) {
  for tup in zip(A, B) {
    tup(0) = 1;
  }
}

proc tupleIndexForall(A, B) {
  forall tup in zip(A, B) {
    tup(0) = 1;
  }
}

proc tupleZipFor(tup) {
  for (a,b) in zip( (...tup) ) {
    b = 2;
  }
}

proc tupleZipForall(tup) {
  forall (a,b) in zip( (...tup) ) {
    b = 2;
  }
}

proc tupleIndexZipFor(tup) {
  for tupi in zip( (...tup) ) {
    tupi(n) = 3;
  }
}

proc tupleIndexZipForall(tup) {
  forall tupi in zip( (...tup) ) {
    tupi(n) = 3;
  }
}



writeln("tupleIndexFor sets 1st");  
tupleIndexFor(getter(), getter());
writeln("tupleIndexForall sets 1st");  
tupleIndexForall(getter(), getter());

writeln("tupleZipFor sets 2nd");  
tupleZipFor( (getter(), getter()) );
writeln("tupleZipForall sets 2nd");  
tupleZipForall( (getter(), getter()) );

writeln("tupleIndexZipFor sets all");  
tupleIndexZipFor( (getter(), getter()) );
writeln("tupleIndexZipForall sets all");  
tupleIndexZipForall( (getter(), getter()) );

