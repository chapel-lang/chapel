record R { var x:int; }

proc getRef( r:R ) const ref {
  return r.x;
}

proc setAndGetRef( ref r:R ) const ref {
  r.x = 2;
  return r.x;
}


proc readem( const ref a:int, const ref b:int ) {
  writeln(a);
  writeln(b);
}

proc run() {
  var r:R;
  readem( getRef(r), setAndGetRef(r) );
}

run();
