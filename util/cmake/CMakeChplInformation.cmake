
# TODO: this is a hack, how do we tell cmake that we can't compile to object files?
set(CMAKE_CHPL_COMPILE_OBJECT "test -f <OBJECT> || ln -s <SOURCE> <OBJECT>")
set(CMAKE_CHPL_LINK_EXECUTABLE "<CMAKE_CHPL_COMPILER> -o <TARGET> <OBJECTS> <FLAGS> <LINK_FLAGS>")
# <COMPILE_FLAGS> doesn't work on link step

# TODO: support --library directly to build Chapel shared libs for interoperability

set(CMAKE_CHPL_SOURCE_FILE_EXTENSIONS chpl)

set(CMAKE_CHPL_OUTPUT_EXTENSION ".chpl")


set(CMAKE_CHPL_FLAGS_RELEASE "--fast")
set(CMAKE_CHPL_FLAGS_DEBUG "-g")
