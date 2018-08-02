/*
 * 'scanCounts' scan implementation. See also, 'counts' reduction.
 *
 * Given a list of particles with locations in one of eight octants, a
 * reduction could determine how many particles are in each location. A scan
 * could determine a ranking of the particles within each octant.
 */

class scanCounts : ReduceScanOp {
  type eltType;
  const k: int = 8;
  var v: [1..k] int;

  // Track the current value, so it can be used in generate() method. Use min()
  // as arbitrary default value here, in lieu of no "default of type" support
  // for user code.
  var _curValue: eltType = min(eltType);

  proc accumulate(value: eltType) {
    _curValue = value;
    v[value] += 1;
  }

  proc combine(state: counts(eltType)) {
    // rely on promotion, instead of iterating over state.v
    v += state.v;
  }

  proc generate() {
    return v[_curValue];
  }
}
