module Other {
  /* This method has a variable number of arguments */
  proc variable(x ...) {

  }

  proc variableCommentless(c ...) {

  }

  /* This method requires a specific number of arguments, w/o limiting type */
  proc limited(x ...5) {

  }

  proc limitedCommentless(x ... 5) {

  }

  /* This methods can take any number of arguments, but that number must 
     be specified by the caller */
  proc specifyMe (q ...?k) {

  }

  proc specifyMeCommentless (q ... ?p) {

  }

}

/* This method has a variable number of arguments */
proc variable(x ...) {

}

proc variableCommentless(c ...) {

}

/* This method requires a specific number of arguments, w/o limiting type */
proc limited(x ...5) {

}

proc limitedCommentless(x ... 5) {

}

/* This methods can take any number of arguments, but that number must 
   be specified by the caller */
proc specifyMe (q ...?k) {
  
}

proc specifyMeCommentless (q ... ?p) {
  
}
