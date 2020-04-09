/*
 * 'isSorted' reduction implementation. Determines whether values are sorted,
 * or not.
 */

public use Sort;
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

record AccumState {
  type eltType;
  var status = true;
  var first, last: eltType;
  var _accumulatedFirst = false;
}

class isSorted : ReduceScanOp {
  type eltType;
  param commutative = false;    // Note: This has no effect in the current
                                // Chapel compiler; it was part of a plan
                                // that was never implemented to date.
  var state: AccumState(eltType);

  proc identity {
    var id: state.type;
    return id;
  }

  proc accumulateOntoState(ref state: AccumState(eltType), value: eltType) {
    if !state._accumulatedFirst {
      state.first = value;
      state.last = value;
      state._accumulatedFirst = true;
    }
    if state.last > value then
      state.status = false;
    state.last = value;
  }

  proc accumulate(value: eltType) {
    accumulateOntoState(state, value);
  }

  proc accumulate(other: AccumState(eltType)) {
    this.state.status = this.state.status && state.status && this.state.last <= state.first;
    this.state.last = state.last;
  }

  // FIXME: the current reduction framework does not provide a way to enforce
  // ordering of combine() calls. Instead, it assumes that combine() calls are
  // commutative. This can result in a wrong answer computed by the reduction.
  proc combine(other: unmanaged isSorted(eltType)) {
    accumulate(other.state);
  }

  proc generate() {
    return state.status;
  }

  proc clone() {
    return new unmanaged isSorted(eltType=eltType);
  }
}
