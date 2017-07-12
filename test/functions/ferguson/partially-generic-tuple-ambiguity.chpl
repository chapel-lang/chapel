
// expecting an ambiguity error
proc f(tuple: (?,real)) {
}
proc f(tuple: (?,?)) {
}
f( (1.0, 2.0) );
