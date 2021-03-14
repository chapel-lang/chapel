/* This proc specifies the type of the next function's argument */
proc nada() type {

}

/* This proc takes an argument whose type is determined by
   another function */
proc needsAType(val: nada()) {

}