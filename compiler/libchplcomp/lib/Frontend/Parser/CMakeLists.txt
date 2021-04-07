# cmake has some support for finding flex/bison,
# but it wasn't clear to me how to get that working with
# our model of checking in the generated parser.
#FIND_PACKAGE(BISON)
#FIND_PACKAGE(FLEX)
#IF(BISON_FOUND AND FLEX_FOUND)
#  BISON_TARGET(MyParser chapel.ypp ${CMAKE_CURRENT_SOURCE_DIR}/bison-chapel.cpp
#               DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/bison-chapel.h)
#  FLEX_TARGET(MyScanner chapel.lex ${CMAKE_CURRENT_SOURCE_DIR}/flex-chapel.cpp)
#  ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)
#ENDIF(BISON_FOUND AND FLEX_FOUND)

target_sources(libchplcomp
               PRIVATE

               bison-chapel.cpp
               bison-chapel.h
               flex-chapel.cpp

              )

add_custom_target(parser
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  COMMAND bison chapel.ypp
                  COMMAND flex chapel.lex
                  COMMAND rm -f bison-chapel.output
                  COMMENT "generating flex/bison parser")
