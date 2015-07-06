/* feature request: Query on size of tuple. */

proc p(pt : ?ndim * int) {
  writef("number dimensions ", ndim);
}

p((3,4,5,6,7));
