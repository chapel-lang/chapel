config const iterations = 1;

config const N = 2**20;
const R = 1..N;
const D = {R};
var A: [D] real;

{
  extern proc chpl_topo_showMemLocality(p: c_ptr, size: size_t, indentLen: int);
  extern proc sizeof(type x): size_t;

  writeln("A localization:");
  if A._value.oneDData {
    chpl_topo_showMemLocality(c_ptrTo(A),
                              A.domain.numIndices:size_t * sizeof(A.eltType),
                              2);
  } else {
    for i in 0..#A._value.mdNumChunks {
      writeln("  chunk ", i);
      const size = A.domain.numIndices / A._value.mdRLen
                   * A._value.mData(i).pdr.length;
      chpl_topo_showMemLocality(c_ptrTo(A._value.mData(i).data(0)),
                                size:size_t * sizeof(A.eltType),
                                4);
    }
  }
}
