proc main()
{
   //do stuff
}

// <2011-07-19 THH> Note: The terminating semicolon used to be a syntax error, 
// but with the introduction of separate compilation, a bodyless signature
// introduces the function name into the current compilation unit.
// The following is parsed as a forward proc declaration, followed by a 
// compound statement which is always executed (just as the comment declares).
proc something();
{
   //this code is always executed
}
