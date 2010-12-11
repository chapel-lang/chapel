//-----------------------------------------------------
// The spatial dimension, 'dimension', can be set by a
// compiler flag at compile-time, "--set dimension=?".
//
// The domain 'dimensions' is used wherever iteration
// over the spatial dimensions is required.  It's a 
// domain rather than a range so that it can index an
// array if desired.
//-----------------------------------------------------

config param dimension = 2;
const dimensions = [1..dimension];
