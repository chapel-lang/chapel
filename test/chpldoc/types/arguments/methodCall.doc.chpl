/* This proc specifies the type of the next method's argument */
proc nada() type {

}

/* This proc takes an argument whose type is determined by
   another method */
proc needsAType(val: nada()) {

}