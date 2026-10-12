/*
  Records should use camelCase naming convention.
*/

// This record uses PascalCase which violates the rule
record MyRecord {
  var id: int;
  var name: string;
}

// This record uses camelCase which is correct
record myRecord {
  var id: int;
  var name: string;
}
