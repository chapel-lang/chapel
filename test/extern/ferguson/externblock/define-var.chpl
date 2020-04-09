extern {
  #define MYVAR myvar_impl
  int myvar_impl = 20;
}

writeln(MYVAR);

