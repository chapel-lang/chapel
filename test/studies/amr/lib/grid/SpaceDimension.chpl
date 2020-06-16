//-----------------------------------------------------
// The spatial dimension, 'dimension', can be set by a
// compiler flag at compile-time, "--set dimension=?".
//
// The range 'dimensions' is used wherever iteration
// over the spatial dimensions is required.
//-----------------------------------------------------

config param dimension = 2;
const dimensions = 0..#dimension;
