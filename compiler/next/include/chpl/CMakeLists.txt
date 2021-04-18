file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h")
target_sources(libchplcomp-obj
               PUBLIC

               ${headers})
