record thingy {

}

class whatchamacallit {
  /* This field contains a thingy */
  var aThingy: thingy;
}

class recursion {
  /* This field contains a whatchamacallit */
  var aWhatchamacallit: whatchamacallit;
  /* This field is recursive */
  var recursive: recursion;
}