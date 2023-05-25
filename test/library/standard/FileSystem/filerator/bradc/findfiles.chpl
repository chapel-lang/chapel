use FileSystem, Sort;

config const startdir = "subdir";
config const recur = false;
config const dotfiles = false;
config param sort = true;

if sort then
  for filename in sorted(findFiles(startdir, recur, dotfiles)) do
    writeln(filename);
else
  for filename in findFiles(startdir, recur, dotfiles) do
    writeln(filename);
