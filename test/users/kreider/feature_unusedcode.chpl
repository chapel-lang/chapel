/* bug: Chapel ignores unused code when compiling.

   The type mismatch does not cause a compile fail if the procedure isn't
   called.  If you uncomment the call, then the compile fails.
*/

proc unused(i : int) : bool {
  return i;
}

writeln("ignoring unused");
/*
unused(1);
*/
