use FileSystem;

config const startdir = ".";
config const topdown = true;
config const depth = max(int);
config const dotfiles = false;
config const followlinks = false;

config const sort = true;
config const defaults = false;

if defaults then
  for filename in walkdirs(sort=sort) do
    writeln(filename);
else
  for filename in walkdirs(startdir, topdown, depth, dotfiles, followlinks, sort=sort) do
    writeln(filename);
