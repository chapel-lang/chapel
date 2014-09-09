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


iter walkdirs(path: string=".", topdown=true, depth=max(int), dotfiles=false, followlinks=false, sort = false): string {
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

  if (topdown) then
    yield path;

  if (depth) {
    var dir: DIRptr;
    var ent: direntptr;
    if debug then
      writeln("***Trying ", path);
    dir = opendir(path:c_string);
    if (!is_c_nil(dir)) {
      ent = readdir(dir);
      while (!is_c_nil(ent)) {
        const filename = ent.d_name();
        if (filename != "." && filename != "..") {
          const fullpath = path + "/" + filename;
          
          if (chpl_rt_isDir(fullpath:c_string)!=0) {
            if debug then
              writeln("***It's a dir!");
            if (dotfiles || filename.substring(1) != '.') {
              //
              // feature request: This is a nice place for a yieldall concept
              //
              var subdirs = walkdirs(fullpath, topdown, depth-1, dotfiles, followlinks);
              if (sort) then
                QuickSort(subdirs);
              for subdir in subdirs do
                yield subdir;
            }
          }
        }
        ent = readdir(dir);
      }
    } else {
      extern proc perror(s: c_string);
      perror("error in walkdirs(): ");
    }
    closedir(dir);
  }

  if (!topdown) then
    yield path;
}

