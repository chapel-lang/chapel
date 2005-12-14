var one: integer = 1;

if (!one) {
   writeln("This shouldn't have printed.");
}

if (one != 1) {
   writeln("And this shouldn't print either.");
}