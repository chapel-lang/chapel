/*
   Testing the warning when .chplconfig file is not found

   The actual testings takes place in warnings.precomp, this can be
   considered the .good file

 */

writeln("\
Warning: $CHPL_CONFIG is defined, but no $CHPL_CONFIG/.chplconfig file is found\
");
