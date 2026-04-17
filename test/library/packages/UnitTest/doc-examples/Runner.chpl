use MasonTest;

proc main(args: [] string) {
  const localArgs = ["test"];
  var allArgs: [1..#(localArgs.size + args.size - 1)] string;
  allArgs[1..#localArgs.size] = localArgs;
  allArgs[(localArgs.size + 1)..] = args[1..];
  masonTest(allArgs);
}
