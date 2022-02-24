/*
 * 'counts' reduction implementation. See also, 'scanCounts' scan.
 *
 * Given a list of particles with locations in one of eight octants, a
 * reduction could determine how many particles are in each location. A scan
 * could determine a ranking of the particles within each octant.
 */

class counts : ReduceScanOp {
  type eltType;
  const k: int = 8;
  var v: [1..k] int;

  proc identity {
    var v: [1..k] int; return v;
  }

  proc accumulateOntoState(ref v, value: eltType) {
    v[value] += 1;
  }

  proc accumulate(value: eltType) {
    accumulateOntoState(v, value);
  }

  proc accumulate(accumState: []) {
    // rely on promotion, instead of iterating over state.v
    v += accumState;
  }

  proc combine(state: borrowed counts(eltType)) {
    accumulate(state.v);
  }

  proc generate() {
    return v;
  }

  proc clone() {
    return new unmanaged counts(eltType=eltType, k=k);
  }
}
