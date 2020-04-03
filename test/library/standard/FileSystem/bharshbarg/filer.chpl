use Sort;

//
// This module defines several iterators that are designed to help
// support reasoning about directory and file contents.  At present,
// all are serial and single-locale, though the ultimate intention
// is to also support parallel and multi-locale options.  
//
// These interfaces should be considered experimental and subject to
// change for the 1.10 release.  For that reason, send in feedback if
// you would like to see changes or additional arguments/features.
// And, as always, send in bug reports if you think we got something
// wrong.
//

/* iter listdir(path: string, hidden=false, dirs=true, files=true, 
   listlinks=true): string

   listdir() lists the contents of a directory, similar to 'ls'
 * path: the directory whose contents should be listed
 * hidden: should hidden files/directories be listed?
 * dirs: should dirs be listed?
 * files: should files be listed?
 * listlinks: should symbolic links be listed?

 By default this routine lists all files and directories in the
 current directory, including symbolic links, as long as they don't
 start with '.'
 */

iter listdir(path: string, hidden=false, dirs=true, files=true, 
    listlinks=true): string {
  use SysCTypes;

  extern type DIRptr;
  extern type direntptr;
  extern proc opendir(name: c_string): DIRptr;
  extern proc readdir(dirp: DIRptr): direntptr;
  extern proc closedir(dirp: DIRptr): c_int;

  proc direntptr.d_name(): c_string {
    extern proc chpl_rt_direntptr_getname(d: direntptr): c_string;

    return chpl_rt_direntptr_getname(this);
  }

  var dir: DIRptr;
  var ent: direntptr;
  dir = opendir(path.c_str());
  if (!is_c_nil(dir)) {
    ent = readdir(dir);
    while (!is_c_nil(ent)) {
      const filename = createStringWithNewBuffer(ent.d_name());
      if (hidden || filename[0] != '.') {
        if (filename != "." && filename != "..") {
          //
          // use FileSystem;  // Doesn't work, see comment below
          //
          const fullpath = path + "/" + filename;
          {
            //
            // The use of this compound statement to restrict the
            // impact of the 'use' of FileSystem is unfortunate
            // (compared to placing it in the more logical place
            // above), yet seemingly required at present; otherwise
            // the 'path' argument gets shadowed by a
            // (compiler-introduced?) method coming from one of the
            // standard or internal modules.  See
            // test/modules/bradc/useFileSystemShadowsPath.chpl for
            // a smaller standalone test exhibiting the issue (or
            // uncomment the 'use' above to see it here).
            //
            use FileSystem;

            if (listlinks || !isLink(fullpath)) {
              if (dirs && isDir(fullpath)) then
                yield filename;
              else if (files && isFile(fullpath)) then
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


/* iter walkdirs(path: string=".", topdown=true, depth=max(int), 
   hidden=false, followlinks=false, sort=false): string

   walkdirs() recursively walks a directory structure, yielding
   directory names.  The strings that are generated will be rooted
   from 'path'.

 * path: the directory to start from
 * topdown: indicates whether to yield the directories using a
 preorder (vs. postorder) traversal
 * depth: indicates the maximal depth of recursion to use
 * hidden: indicates whether to enter hidden directories
 * followlinks: indicates whether to follow symbolic links or not
 * sort: indicates whether to consider subdirectories in sorted
 order or not

 by default, walkdirs() will start in the current directory, process
 directories in preorder; recursively traverse subdirectories; and
 neither follow dotfile directories nor symbolic links.  It will not
 sort the directories by default.
 */

iter walkdirs(path: string=".", topdown=true, depth=max(int), hidden=false, 
    followlinks=false, sort=false): string {

  if (topdown) then
    yield path;

  if (depth) {
    var subdirs = listdir(path, hidden=hidden, files=false, listlinks=followlinks);
    if (sort) then
      QuickSort.quickSort(subdirs);
    for subdir in subdirs {
      const fullpath = path + "/" + subdir;
      for subdir in walkdirs(fullpath, topdown, depth-1, hidden, 
          followlinks, sort) do
        yield subdir;
    }
  }

  if (!topdown) then
    yield path;
}


//
// Here's a parallel version
//
iter walkdirs(path: string=".", topdown=true, depth=max(int), hidden=false, 
    followlinks=false, sort=false, param tag: iterKind): string 
                                                         where tag == iterKind.standalone {
  if (sort) then
    warning("sorting has no effect for parallel invocations of walkdirs()");

  if (topdown) then
    yield path;

  if (depth) {
    var subdirs = listdir(path, hidden=hidden, files=false, listlinks=followlinks);
    forall subdir in subdirs {
      const fullpath = path + "/" + subdir;
      //
      // Call standalone walkdirs() iterator recursively
      //
      for subdir in walkdirs(fullpath, topdown, depth-1, hidden, followlinks, sort=false, iterKind.standalone) do
        yield subdir;
    }
  }

  if (!topdown) then
    yield path;
}

config const startdir = ".";
config const topdown = true;
config const depth = max(int);
config const dotfiles = false;
config const followlinks = true;

config const sort = true;
config const defaults = false;

forall filename in walkdirs(startdir, topdown, depth, dotfiles, followlinks, sort=sort) do
writeln(filename);
