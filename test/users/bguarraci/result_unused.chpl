for i in 1..10 {
  var c = i / 10;
}

/* At some point, with some C compiler, this code
   produced a warning. It doesn't in current testing,
   so I'm putting the .future message here in case it comes
   up again.

feature: the error message for this unused variable c is not easily correlated with the Chapel source code

In file included from /var/folders/sv/8rdk1swj2kn4jsbjm108r2b80000gn/T//chpl-bguarraci-92825.deleteme/_main.c:30:
/var/folders/sv/8rdk1swj2kn4jsbjm108r2b80000gn/T//chpl-bguarraci-92825.deleteme/result_unused.c:19:9: warning: expression result unused [-Wunused-value]
(i_chpl / INT64(10));
 ~~~~~~ ^ ~~~~~~~~~
1 warning generated.

*/
