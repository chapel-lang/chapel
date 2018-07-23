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

  proc accumulate(value: eltType) {
    v[value] += 1;
  }

  proc combine(state: counts(eltType)) {
    // rely on promotion, instead of iterating over state.v
    v += state.v;
  }

  proc generate() {
    return v;
  }
}
