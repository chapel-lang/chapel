use Reflection;

var bye = chpl__coerceMove("hi", false);

pragma "coerce fn"
proc chpl__coerceMove(hi:string, b: bool) {
  // Curiously, if we call asdf() directly, its compilerError()
  // aborts compilation directly, without getting postponed
  // or reporting "invalid copy-initialization":
  //asdf(5);

  // This results in a postponed error and "invalid copy-initialization":
  canResolve("asdf", 5);

  return "bye";
}

proc asdf(x: int) do
  compilerError("fatal error in asdf", 0);
