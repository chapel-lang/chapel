//
// the message to be printed out, defined as a configuration
// constant named "message".  The type is inferred to be a string
// due to the initializing expression.  This message can be
// over-ridden by using --message="new message" on the executable's
// command line
//
config const message = "Hello, world!";

writeln(message);
