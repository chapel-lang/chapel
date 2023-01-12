use OS.POSIX;

var structStat: struct_stat;
var err: int;

// This test is going to be robust, gorramn it!
var permission: int;
for ur in [true, false] {
  permission = if ur then 256 else 0;
  for uw in [true, false] {
    permission += if uw then 128 else 0;
    for ux in [true, false] {
      permission += if ux then 64 else 0;
      for gr in [true, false] {
        permission += if gr then 32 else 0;
        for gw in [true, false] {
          permission += if gw then 16 else 0;
          for gx in [true, false] {
            permission += if gx then 8 else 0;
            for or in [true, false] {
              permission += if or then 4 else 0;
              for ow in [true, false] {
                permission += if ow then 2 else 0;
                for ox in [true, false] {
                  permission += if ox then 1 else 0;
                  var perm = permission:c_int;
                  err = chmod(c"blah", perm: mode_t);
                  if err != 0 then halt("error in chmod");
                  err = stat(c"blah", c_ptrTo(structStat));
                  if err != 0 then halt("error in stat");
                  var result = structStat.st_mode: c_int & 0x1ff;//bottom 9 bits
                  if (result != perm) {
                    writeln("Expected ", permission, " on file blah, got ", result);
                  }
                  permission -= if ox then 1 else 0;
                }
                permission -= if ow then 2 else 0;
              }
              permission -= if or then 4 else 0;
            }
            permission -= if gx then 8 else 0;
          }
          permission -= if gw then 16 else 0;
        }
        permission -= if gr then 32 else 0;
      }
      permission -= if ux then 64 else 0;
    }
    permission -= if uw then 128 else 0;
  }
  permission = 0;
}

permission = 448;
var perm = permission:c_int;
err = chmod(c"blah", perm: mode_t); // reset blah's permissions to only include user permissions
if err != 0 then halt("error in chmod");
