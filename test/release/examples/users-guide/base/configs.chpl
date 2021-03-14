config var n = 10;
config const epsilon = 0.01,
             debug = false;

writeln(n);
writeln(epsilon);
writeln(debug);


//
// I put extra strings on the execopts lines for the sake of the users
// guide text.  Gobble them up here.
//
proc main(args: [] string) {
  if args.size > 1 then
    assert(args[1] == './configs');
}
