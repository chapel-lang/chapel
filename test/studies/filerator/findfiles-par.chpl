use Filerator, Sort;

config const startdir = "subdir";
config const recur = false;
config const dotfiles = false;

forall filename in findfiles(startdir, recur, dotfiles) do
  writeln(filename);
