/*
 * 'scanCountsInherit' scan implementation. This is the ideal implementation of
 * 'scanCounts'. Currently, January 2015, inheritance with generic classes is
 * not supported so it does not work.
 *
 * Given a list of particles with locations in one of eight octants, a
 * reduction could determine how many particles are in each location. A scan
 * could determine a ranking of the particles within each octant.
 */

public use counts;

class scanCountsInherit : counts {
  // Track the current value, so it can be used in generate() method. Use min()
  // as arbitrary default value here, in lieu of no "default of type" support
  // for user code.
  var _curValue: eltType = min(eltType);

  override proc accumulate(value: eltType) {
    _curValue = value;
    super.accumulate(value);
  }

  override proc generate() {
    return v[_curValue];
  }
}
