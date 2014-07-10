iter listdir(path: string, recur = false, dotfiles=false, nosvn=true): string {
  extern type DIRptr;
  extern type direntptr;
  extern proc opendir(name: c_string): DIRptr;
  extern proc readdir(dirp: DIRptr): direntptr;
  extern proc closedir(dirp: DIRptr): c_int;

  proc direntptr.d_name(): c_string {
    extern proc chpl_rt_direntptr_getname(d: direntptr): c_string;

    return chpl_rt_direntptr_getname(this);
  }

  proc direntptr.isDir() {
    extern proc chpl_rt_direntptr_isDir(d: direntptr): bool;

    return chpl_rt_direntptr_isDir(this);
  }

  var dir: DIRptr;
  var ent: direntptr;
  //  writeln("***Trying ", path);
  dir = opendir(path:c_string);
  if (!is_c_nil(dir)) {
    ent = readdir(dir);
    while (!is_c_nil(ent)) {
      const filename = ent.d_name();
      if (dotfiles || filename.substring(1) != '.') {
        if (!nosvn || filename != '.svn') {
          const fullpath = path + "/" + filename;
          if (!ent.isDir()) {
            yield fullpath;
          } else {
            //        writeln("^^^ it's a directory!");
            if recur && filename != "." && filename != ".." {
              //
              // feature request: This is a nice place for a yieldall concept
              //
              for filename in listdir(fullpath, recur, dotfiles) do
                yield filename;
            }
          }
        }
      }
      ent = readdir(dir);
    }
    closedir(dir);
  } else {
    extern proc perror(s: c_string);
    perror("error in listdir(): ");
  }
}

iter sorted(x) {
  use Sort;

  var y = x;
  QuickSort(y);
  for i in y do
    yield i;
}

config const startdir = "subdir";
config const recur = false;
config const dotfiles = false;
config param sort = true;

if sort then
  for filename in sorted(listdir(startdir, recur, dotfiles, nosvn=true)) do
    writeln(filename);
else
  for filename in listdir(startdir, recur, dotfiles, nosvn=true) do
    writeln(filename);
