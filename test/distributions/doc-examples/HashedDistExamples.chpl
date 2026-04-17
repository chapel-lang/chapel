//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
record CustomMapper {
  proc this(ind:string, targetLocs: [?D] locale) : D.idxType {
    const numlocs = targetLocs.domain.size;
    // use the first digit of the string to choose the destination locale
    var byte: int = ind.byte(1);
    return byte % numlocs;
  }
}
/* STOP_EXAMPLE_0 */

/* START_EXAMPLE_1 */
  use HashedDist;

  var D: domain(string) dmapped new hashedDist(mapper=new CustomMapper(),
                                               idxType=string);

  // Now D is a distributed associative domain (set) of strings
  D += "one";
  D += "two";
  D += "three";
  D += "four";
  D += "five";
  D += "six";

  var A: [D] int;
  // Now A is a distributed associative array (map) from string to int
  forall a in A do
    a = a.locale.id;

  for (key, value) in zip(D, A) {
    writeln(key, " -> ", value);
  }
/* STOP_EXAMPLE_1 */
