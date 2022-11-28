use FileSystem, Sort;

config const startdir = "subdir";
config const recur = true;
config const dotfiles = true;

forall filename in findFiles(startdir, recur, dotfiles) do
  writeln(filename);
