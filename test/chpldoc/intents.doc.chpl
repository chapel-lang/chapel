module Inner {
  /* This function has an argument with the in intent */
  proc one (in val) {

  }

  /* This function has an argument with the out intent */
  proc two (out val) {

  }

  /* This function has an argument with the inout intent */
  proc three (inout val) {

  }

  /* This function has an argument with the param intent */
  proc four (param val) {

  }

  /* This function has an argument with the type intent */
  proc five (type val) {

  }

  /* This function has an argument with the const intent */
  proc fiveA (const val) {

  }

  /* This function has an argument with the ref intent */
  proc fiveB (ref val) {

  }

  /* This function has an argument with an intent and a type */
  proc six (in val: int) {

  }

  /* This function has a return intent of ref */
  proc seven () ref {
    return "I suppose if I must have a return type here . . . ";
  }

  /* This function has a return intent of const */
  proc eight () const {

  }

  /* This function has a return intent of param */
  proc nine () param {

  }

  /* This function has a return intent of type */
  proc ten () type {

  }

  /* This function has both an argument and a return intent */
  proc twelve (val: int) param {

  }

  /* This function has both an argument intent and a return intent */
  proc thirteen (inout val) ref {

  }

  /* This function has a return intent (other than ref) and a body */
  proc fourteen () const {
    return true;
  }
}
/* This function has an argument with the in intent */
proc one (in val) {

}

/* This function has an argument with the out intent */
proc two (out val) {

}

/* This function has an argument with the inout intent */
proc three (inout val) {

}

/* This function has an argument with the param intent */
proc four (param val) {

}

/* This function has an argument with the type intent */
proc five (type val) {

}

/* This function has an argument with the const intent */
proc fiveA (const val) {

}

/* This function has an argument with the ref intent */
proc fiveB (ref val) {

}

/* This function has an argument with an intent and a type */
proc six (in val: int) {

}

/* This function has a return intent of ref */
proc seven () ref {
  return "I suppose if I must have a return type here . . . ";
}

/* This function has a return intent of const */
proc eight () const {

}

/* This function has a return intent of param */
proc nine () param {

}

/* This function has a return intent of type */
proc ten () type {

}

/* This function has both an argument and a return intent */
proc twelve (val: int) param {

}

/* This function has both an argument intent and a return intent */
proc thirteen (inout val) ref {

}

/* This function has a return intent (other than ref) and a body */
proc fourteen () const {
  return true;
}


record foo {
  /* this method has a "const" "this" intent */
  proc const contains(x: eltType): bool {
    return true;
  }

  /*
  This method has a "const ref" "this" intent
  */
  proc const ref eighteen {
    return false;
  }
}

/* I am test fifteen, a test for return intent and type */
proc fifteen(type t) param : int {
  return 1;
}

/* I am test sixteen, a test for return intent and type */
proc sixteen(v: int ) param : bool {
  return true;
}

/* I am test seventeen, cousin of sixteen,
   also a test for return intent and type */
proc seventeen(param q: bool) param : string {
  return "yes";
}
