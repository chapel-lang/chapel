/*
  Modules should use PascalCase naming convention.
*/

// This module uses camelCase which violates the rule.
module modX {
  var x: string = "Module MX";
  proc printX() {
    writeln(x);
  }
}

// This module uses PascalCase which is correct.
module ModX {
  var x: string = "Module MX";
  proc printX() {
    writeln(x);
  }
}
