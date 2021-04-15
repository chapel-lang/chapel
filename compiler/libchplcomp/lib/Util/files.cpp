#include "files.h"

#include "./mystrerror.h"

#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Location.h"

#include <cerrno>

namespace chpl {

FILE* openfile(const char* path, const char* mode, ast::ErrorMessage& errorOut) {
  FILE* fp = fopen(path, mode);
  if (fp == nullptr) {
    std::string strerr = my_strerror(errno);
    auto emptyLocation = ast::Location();
    // set errorOut. NULL will be returned.
    errorOut = ast::ErrorMessage::build(emptyLocation, "opening %s: %s",
                                   path, strerr.c_str());
  }

  return fp;
}

bool closefile(FILE* fp, const char* path, ast::ErrorMessage& errorOut) {
  int rc = fclose(fp);
  if (rc != 0) {
    std::string strerr = my_strerror(errno);
    auto emptyLocation = ast::Location();
    errorOut = ast::ErrorMessage::build(emptyLocation, "closing %s: %s",
                                   path, strerr.c_str());
    return false;
  }
  return true;
}

bool readfile(const char* path,
              std::string& strOut,
              ast::ErrorMessage& errorOut) {

  FILE* fp = openfile(path, "r", errorOut);
  if (fp == nullptr) {
    return false;
  }

  // Try to get the file length in order to optimize string allocation
  long len = 0;
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  if (len > 0) strOut.reserve(len);

  char buf[256];
  while (true) {
    size_t got = fread(buf, 1, sizeof(buf), fp);
    if (got > 0) {
      strOut.append(buf, got);
    } else {
      if (ferror(fp)) {
        auto emptyLocation = ast::Location();
        errorOut = ast::ErrorMessage::build(emptyLocation, "reading %s", path);
        ast::ErrorMessage ignored;
        closefile(fp, path, ignored);
        return false;
      }
      // otherwise, end of file reached
      break;
    }
  }
  bool ok = closefile(fp, path, errorOut);
  if (!ok) {
    return false;
  }

  return true;
}

}
