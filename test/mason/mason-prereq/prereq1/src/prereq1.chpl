/* Documentation for prereq1 */
module prereq1 {
  extern proc hello();
  proc main() {
    writeln("New mason package: prereq1");
    hello();
  }
}
