BEGIN { RS=" " }
/^-DNDEBUG$/ { next }
/^-O[0-4s]?$/ { next }
/^-pedantic$/ { next }
/^-W.,/ { printf " %s",$0 }
/^-Wno-/ { printf " %s",$0 }
/^-W/ { next }
{ printf " %s",$0 }
END { printf "\n" }
