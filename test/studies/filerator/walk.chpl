use Filerator, Sort;

config const startdir = "subdir";
config const topdown = true;
config const depth = max(int);
config const dotfiles = false;
config const followlinks = false;

config const sort = true;
config const defaults = false;

if defaults then
  if sort then
    for filename in sorter(walkdirs()) do
      writeln(filename);
 else
   for filename in walkdirs() do
     writeln(filename);
else
  if sort then
    for filename in sorter(walkdirs(startdir, topdown, depth, dotfiles, followlinks)) do
      writeln(filename);
 else
   for filename in walkdirs(startdir, topdown, depth, dotfiles, followlinks) do
     writeln(filename);
