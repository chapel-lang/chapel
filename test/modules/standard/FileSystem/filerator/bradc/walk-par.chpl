use FileSystem;

config const startdir = ".";
config const topdown = true;
config const depth = max(int);
config const dotfiles = false;
config const followlinks = true;

config const sort = true;
config const defaults = false;

forall filename in walkdirs(startdir, topdown, depth, dotfiles, followlinks, sort=sort) do
  writeln(filename);
