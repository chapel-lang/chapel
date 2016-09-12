/*
 * 'isSorted' reduction implementation. Determines whether values are sorted,
 * or not.
 */

use Sort;
//
// BLC: For some of the reasons noted below in comments, I don't
// believe this test behaves as expected (nor do I think it could,
// given the current user-defined reduction framework).  I think it's
// a pattern that would be interesting to be able to express in the
// future, though (and I think we planned for such capabilities in our
// original paper on user-defined reductions and scans), so I'm
// leaving it in, particularly since it isn't failing at present.  I
// won't be surprised if future changes to reductions did cause it to
// break, though, so I'm leaving it in for now.
//

class isSorted : ReduceScanOp {
  type eltType;
  param communicative = false;  // Note: This has no effect in the current
                                // Chapel compiler; it was part of a plan
                                // that was never implemented to date  (and
                                // should read 'commutative', I think?
  var status = true;
  var first, last: eltType;

  var _accumulatedFirst = false;

  proc accumulate(value: eltType) {
    if !_accumulatedFirst {
      first = value;
      last = value;
      _accumulatedFirst = true;
    }
    if last > value then
      status = false;
    last = value;
  }

  // FIXME: This reference to 'sorted' is a typo that results in the
  // compiler falling back to a (more) serial(ized) implementation
  proc combine(state: sorted(eltType)) { 
    status = status && state.status && last <= state.first;
    last = state.last;
  }

  proc generate() {
    return status;
  }
}
