/* This module uses another module in a separate file */
module first {
  use second;

  /* This function calls the other module's function */
  proc main() {
    writeln(method());
  }


}