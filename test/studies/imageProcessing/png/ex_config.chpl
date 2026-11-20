
/*****
      ex_config.chpl -
      Example program for passing arguments on the command line.  See text
      for description.

      Call:
        ex_config

      c 2015-2018 Primordial Machine Vision Systems
*****/

config param cfgparam : int = 1;
config const cfgconst : int = 2;
config var cfgvar : real = 2.178;

writeln("cfgparam = ", cfgparam, "   cfgconst = ", cfgconst, "   cfgvar = ",
        cfgvar);
