add_subdirectory(Parser)

target_sources(libchplcomp-obj
               PRIVATE

               FrontendQueries.cpp
               Parser.cpp

              )
