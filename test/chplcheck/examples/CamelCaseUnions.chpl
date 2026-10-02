/*
  Unions should use camelCase naming convention.
*/

// This union uses PascalCase which violates the rule
union MyUnion {
  var field1: int;
  var field2: real;
}

// This union uses camelCase which is correct
union myUnion {
  var field1: int;
  var field2: real;
}
