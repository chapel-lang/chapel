record InitEqOnly { }
proc InitEqOnly.init=(other:InitEqOnly) { }

record AssignOnly { }
proc =(ref lhs:AssignOnly, rhs:AssignOnly) { }

record Both { }
proc Both.init=(other:Both) { }
proc =(ref lhs:Both, rhs:Both) { }

record Neither { }

proc tryInitAndAssign(type t) {
  var x: t;
  var y: t = x;
  x = y;
}

proc main() {
  tryInitAndAssign(InitEqOnly); 
  tryInitAndAssign(AssignOnly); 
  tryInitAndAssign(Both); 
  tryInitAndAssign(Neither); 
}
