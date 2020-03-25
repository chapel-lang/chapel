use FileSystem, IO;
forall n in glob("*.chpl") {
  const f = open(n,iomode.r), l=f.size;
  if l > 140 then writeln(n, " too big:", l);
}