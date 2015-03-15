use Filerator, Sort;

config const startdir = "subdir";
config const recur = true;
config const dotfiles = true;

forall filename in findfiles(startdir, recur, dotfiles) do
  writeln(filename);
