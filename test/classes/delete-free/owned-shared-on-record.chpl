record C {}
// just one of these is sufficient to trigger the internal error
type T = owned C;
type T2 = shared C;
