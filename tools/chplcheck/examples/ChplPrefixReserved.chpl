/*
  User-defined names should not start with a reserved prefix.
*/

// This variable uses a reserved prefix which violates the rule.
var chpl_myVar = 10;

// This variable uses a regular name which is correct.
var myVar = 10;
