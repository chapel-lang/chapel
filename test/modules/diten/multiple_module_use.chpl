// Test using multiple modules in a single use statement
use mod1, mod2, mod3;
use mod4;

proc main() {
  mod1foo();
  mod2foo();
  mod3foo();
  mod4foo();
}
