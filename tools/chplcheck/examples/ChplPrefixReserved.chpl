/*
  User-defined names should not start with the reserved 'chpl_' prefix.
*/

// This variable uses the reserved prefix which violates the rule.
var chplMyVar = 10;

// This variable uses a regular name which is correct.
var myVar = 10;
