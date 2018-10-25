record R {
  var x : int;
}
var r : R;

// Intentionally-incorrect AST that should trigger an internal error
__primitive(".v", r, "nonExistentField");
