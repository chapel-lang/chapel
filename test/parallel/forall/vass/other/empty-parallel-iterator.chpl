
iter badIterator(): nothing {}
iter badIterator(param tag : iterKind): nothing where tag == iterKind.standalone {
  writeln("in badIterator");
}

writeln("start");
forall x in badIterator() do writeln("yielded ", x);
writeln("end");
