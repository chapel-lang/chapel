BEGIN { RS=" " }
/^$/ { next }
/^-DNDEBUG$/ { next }
/^-fPIC$/ { next }
/^-gsplit-dwarf$/ { next }
/^-O[0-4s]?$/ { next }
/^-pedantic$/ { next }
/^-W.,/ { printf " %s",$0 }
/^-Wno-class-memaccess/ { next }
/^-Wno-/ { printf " %s",$0 }
/^-W/ { next }
{ printf " %s",$0 }
END { printf "\n" }
