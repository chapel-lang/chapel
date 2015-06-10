use FileSystem;

iter myIter(param tag: iterKind)
  where tag == iterKind.standalone
{
  forall f in findfiles() {
    if f == "./ParallelOnly.chpl" {
      yield f;
    }
  }
}

forall i in myIter() {
  writeln(i);
}
