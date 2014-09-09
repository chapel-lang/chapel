#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "chplisdir.h"
#include "chplrt.h"

int chpl_rt_isDir(const char* pathname, chpl_bool followLinks) {
  struct stat fileinfo;
  if (followLinks) {
    stat(pathname, &fileinfo);
    return S_ISDIR(fileinfo.st_mode);
    /*  return fileinfo.st_mode & S_IFDIR;*/
  } else {
    lstat(pathname, &fileinfo);
    if (fileinfo.st_mode & S_IFLNK) {
      return false;
    } else {
      return S_ISDIR(fileinfo.st_mode);
    }
  }
}
