// What happens if the interface contains an iterator?
// Currently not implemented.

interface IFC {
  iter reqIter();
  iter dfltIter() { // MUST BE AN ERROR even if iterators are allowed:
    yield 5;        // a non-void yield type must be declared explicitly
  }
}
