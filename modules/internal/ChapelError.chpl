module ChapelError {
  class Error {
    var msg: string;

    proc Error(_msg: string) {
      msg = _msg;
    }
  }

  /* This is a work-around to prevent the compiler from
     removing or not properly resolving the Error type. */
  proc ensureErrorTypeRemains() {
    var x = new Error();
    delete x;
  }
  ensureErrorTypeRemains();

}
