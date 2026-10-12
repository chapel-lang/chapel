//
// These definitions for walkDirs used to be in FileSystem.chpl, but they
// were removed in favor of a more robust iterative iterator implementation.
// These are here to continue to stress-test recursive iterators in general
//


use FileSystem;

iter myWalkDirs(path: string = ".", topdown: bool = true, depth: int = max(int),
              hidden: bool = false, followlinks: bool = false,
              sort: bool = false): string {

  if (topdown) then
    yield path;

  if (depth) {
    var subdirs = listDir(path, hidden=hidden, files=false, listlinks=followlinks);
    if (sort) {
      use Sort only sort as sortList;
      sortList(subdirs);
    }

    for subdir in subdirs {
      const fullpath = path + "/" + subdir;
      for subdir in myWalkDirs(fullpath, topdown, depth-1, hidden,
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
@chpldoc.nodoc
iter myWalkDirs(path: string = ".", topdown: bool = true, depth: int =max(int),
              hidden: bool = false, followlinks: bool = false,
              sort: bool = false, param tag: iterKind): string
       where tag == iterKind.standalone {

  if (sort) then
    warning("sorting has no effect for parallel invocations of walkdirs()");

  if (topdown) then
    yield path;

  if (depth) {
    var subdirs = listDir(path, hidden=hidden, files=false, listlinks=followlinks);
    forall subdir in subdirs {
      const fullpath = path + "/" + subdir;
      //
      // Call standalone walkdirs() iterator recursively; set sort=false since it is
      // not useful and we've already printed the warning
      //
      for subdir in myWalkDirs(fullpath, topdown, depth-1, hidden, followlinks, sort=false, iterKind.standalone) do
        yield subdir;
    }
  }

  if (!topdown) then
    yield path;
}
