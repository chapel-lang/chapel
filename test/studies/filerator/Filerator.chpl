config const debug = false;

use Sort;

module FileratorHelp {
  extern type DIRptr;
  extern type direntptr;
  extern proc opendir(name: c_string): DIRptr;
  extern proc readdir(dirp: DIRptr): direntptr;
  extern proc closedir(dirp: DIRptr): c_int;
  extern proc chpl_rt_isDir(pathname: c_string): c_int;

  proc direntptr.d_name(): c_string {
    extern proc chpl_rt_direntptr_getname(d: direntptr): c_string;

    return chpl_rt_direntptr_getname(this);
  }

  /* Not portable (for example to Debian)
  proc direntptr.isDir() {
    extern proc chpl_rt_direntptr_isDir(d: direntptr): bool;

    return chpl_rt_direntptr_isDir(this);
  }
  */
}

iter listdir(path: string, recur = false, dotfiles=false, nosvn=true): string {
  // This does not work:
  //  use FileratorHelp;

  //
  // So inline its contents directly instead:
  //
  extern type DIRptr;
  extern type direntptr;
  extern proc opendir(name: c_string): DIRptr;
  extern proc readdir(dirp: DIRptr): direntptr;
  extern proc closedir(dirp: DIRptr): c_int;
  extern proc chpl_rt_isDir(pathname: c_string): c_int;

  proc direntptr.d_name(): c_string {
    extern proc chpl_rt_direntptr_getname(d: direntptr): c_string;

    return chpl_rt_direntptr_getname(this);
  }
  // End inlining

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
          if (!chpl_rt_isDir(fullpath:c_string)) {
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


iter listdir2(path: string, dotfiles=false, dirs=true, files=false): string {
  // This does not work:
  //  use FileratorHelp;

  //  writeln("listdir2() called with ", path);

  //
  // So inline its contents directly instead:
  //
  extern type DIRptr;
  extern type direntptr;
  extern proc opendir(name: c_string): DIRptr;
  extern proc readdir(dirp: DIRptr): direntptr;
  extern proc closedir(dirp: DIRptr): c_int;
  extern proc chpl_rt_isDir(pathname: c_string): c_int;

  proc direntptr.d_name(): c_string {
    extern proc chpl_rt_direntptr_getname(d: direntptr): c_string;

    return chpl_rt_direntptr_getname(this);
  }
  // End inlining

  var dir: DIRptr;
  var ent: direntptr;
  //  writeln("***Trying ", path);
  dir = opendir(path:c_string);
  if (!is_c_nil(dir)) {
    ent = readdir(dir);
    while (!is_c_nil(ent)) {
      const filename = ent.d_name();
      if (dotfiles || filename.substring(1) != '.') {
        if (filename != "." && filename != "..") {
          const fullpath = path + "/" + filename;
          if (chpl_rt_isDir(fullpath:c_string)) {
            if (dirs) then
              yield filename;
          } else {
            if (files) then
              yield filename;
          }
        }
      }
      ent = readdir(dir);
    }
    closedir(dir);
  } else {
    extern proc perror(s: c_string);
    perror("error in listdir2(): ");
  }
}


iter walkdirs(path: string=".", topdown=true, depth=max(int), dotfiles=false, followlinks=false, sort = false): string {
  //  writeln("topdown = ", topdown);
  //  writeln("walkdirs() called with ", path, " depth=",depth);

  if (topdown) then
    yield path;

  if (depth) {
    var subdirs = listdir2(path, dotfiles=dotfiles, dirs=true);
    if (sort) then
      QuickSort(subdirs);
    for subdir in subdirs {
      const fullpath = path + "/" + subdir;
      //      yield fullpath;
      //      writeln("Calling walkdirs with ", fullpath);
      for subdir in walkdirs(fullpath, topdown, depth-1, dotfiles, followlinks) do
        yield subdir;
    }
  }

  if (!topdown) then
    yield path;
}

