use FileSystem, Spawn;

proc main(args: [] string) {
  const base = args[1];
  const infile = base + ".in";
  const outfile = base + ".out";

  copyFile(infile, outfile);

  var sub = spawn(["diff", infile, outfile]);
  sub.communicate();
}
