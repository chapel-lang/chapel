/^-DNDEBUG$/ { next }
/^-O[0-4s]?$/ { next }
/^-pedantic$/ { next }
/^-W.,/ { print }
/^-Wno-/ { print }
/^-W/ { next }
{ print }
