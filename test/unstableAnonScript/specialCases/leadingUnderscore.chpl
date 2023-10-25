// Copied from test/unstable/leadingUnderscore.chpl

module _MyHiddenModule {
  config const _t: int;

  proc _myPrivateFn() {
    var _doNotLookAtMe: int;
    type _nope = real;
    const _____a: int;
    param _____b: int;
  }

  class _Underscore_ {
  }

  // don't warn for use of special _ (as in dropping a value in de-tupling)
  var (x, _) = (1, 2);
}
