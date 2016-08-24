/*
   Testing the warnings of the chplconfig file functionality

   The actual testings takes place in warnings.precomp, this can be
   considered the .good file

 */



writeln("\
Warning: $CHPL_CONFIG/chplconfig:line 8: Received incorrect format:\
         > CHPL_TASKS == fifo\
         Expected format is:\
         > CHPL_VAR = VALUE\
Warning: $CHPL_CONFIG/chplconfig:line 11: \"CHPL_COMMS\" is not an acceptable variable\
Warning: $CHPL_CONFIG/chplconfig:line 15: Duplicate entry of \"CHPL_COMM\"\n");
