module BoolLitInCondExpr {
  var a = for i in 1..10 do if true then i else 0;

  var b = if false then 1 else 3;
}
