
iter badIterator() {}
iter badIterator(param tag : iterKind) where tag == iterKind.standalone {
  writeln("in badIterator");
}

writeln("start");
forall x in badIterator() do writeln("yielded ", x);
writeln("end");
