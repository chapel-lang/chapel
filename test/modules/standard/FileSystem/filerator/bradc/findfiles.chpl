use FileSystem, Sort;

config const startdir = "subdir";
config const recur = false;
config const dotfiles = false;
config param sort = true;

if sort then
  for filename in sorted(findfiles(startdir, recur, dotfiles)) do
    writeln(filename);
else
  for filename in findfiles(startdir, recur, dotfiles) do
    writeln(filename);
