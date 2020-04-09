/*
   Testing the warning when .chplconfig file is not found

   The actual testings takes place in warnings.precomp, this can be
   considered the .good file

 */

writeln("\
Warning: No chplconfig or .chplconfig file is found in the defined $CHPL_CONFIG\
");
