/*
  The recommended Chapel style is for opening curly braces to be attached to
  the preceding statement, rather than being on a separate line.
*/

// this uses unattached curly braces
record myRecord1
{
  proc myProc()
  {
    writeln("Hello, world!");
  }
}

// this uses attached curly braces
record myRecord2 {
  proc myProc() {
    writeln("Hello, world!");
  }
}
