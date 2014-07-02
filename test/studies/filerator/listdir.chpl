iter listdir(path: string, recur = false, dotfiles=false): string {
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
  if (is_c_nil(dir) == 0) {
    ent = readdir(dir);
    while (is_c_nil(ent) == 0) {
      const filename = ent.d_name();
      if (dotfiles || filename.substring(1) != '.') {
        const fullpath = path + "/" + filename;
        yield fullpath;
        if recur && ent.isDir() && filename != "." && filename != ".." {
          //        writeln("^^^ it's a directory!");

          //
          // feature request: This is a nice place for a yieldall concept
          //
          for filename in listdir(fullpath, recur, dotfiles) do
            yield filename;
        }
      }
      ent = readdir(dir);
    }
    closedir(dir);
  } else {
    extern proc perror(s: string);
    perror("error in listdir(): ");
  }
}

config const startdir = "subdir";
config const recur = false;
config const dotfiles = false;

for filename in listdir(startdir, recur, dotfiles) do
  writeln(filename);
