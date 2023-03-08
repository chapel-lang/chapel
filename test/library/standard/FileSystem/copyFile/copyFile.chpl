use FileSystem, Subprocess;

proc main(args: [] string) {
  const base = args[1];
  const infile = base + ".in";
  const outfile = base + ".out";

  copy(infile, outfile, permissions=false);

  var sub = spawn(["diff", infile, outfile]);
  sub.communicate();
}
