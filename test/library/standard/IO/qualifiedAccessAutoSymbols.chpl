// This test is to ensure that you can call auto-included IO symbols with the
// IO module name as a prefix.  We expect this support to be stable, even when
// we determine a better name for ChapelIO
{
  use IO;

  IO.writeln("that works");
  IO.write("that works, too\n");
  IO.writef("%s", "and this!\n");
}
{
  import IO;

  IO.writeln("that works");
  IO.write("that works, too\n");
  IO.writef("%s", "and this!\n");
}
