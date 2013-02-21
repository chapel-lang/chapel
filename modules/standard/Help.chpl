/* If the user declares main to take an argument, then Chapel's normal
 * processing of --help and -h arguments is suppressed.  As such,
 * printUsage allows the user to generate Chapel's normal help
 * messages, allowing the user to add information on any additional supported
 * arguments.
 */
module Help {
  proc printUsage() {
    extern proc printHelpTable();
    extern proc printConfigVarTable();

    printHelpTable();
    printConfigVarTable();
  }
}
