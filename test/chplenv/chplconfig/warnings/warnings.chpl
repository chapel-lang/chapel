/*
   Testing the warnings of the chplconfig file functionality

   The actual testings takes place in warnings.precomp, this can be
   considered the .good file

 */



writeln("\n\
Warning: Syntax Error: $CHPL_CONFIG/chplconfig:line 8\
              > CHPL_TASKS == fifo\
              Expected format is:\
              > CHPL_VAR = VALUE\
Warning: Syntax Error: $CHPL_CONFIG/chplconfig:line 9\
              > CHPL_TASKS = = fifo\
              Expected format is:\
              > CHPL_VAR = VALUE\
Warning: $CHPL_CONFIG/chplconfig:line 16: \"CHPL_COMMS\" is not an acceptable variable\
Warning: $CHPL_CONFIG/chplconfig:line 20: Duplicate entry of \"CHPL_COMM\"");
