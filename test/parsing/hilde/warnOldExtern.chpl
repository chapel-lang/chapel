// Make sure that we issue a warning when the old-style _extern (with leading underscore)
// appears.  _extern can be attached to types, vars and procs.
// Note that extern iterators are not currently supported.

extern type oneType;
extern proc oneProc(i:int) : oneType;
// extern iter oneIter(o:oneType) : void;
extern var oneVar : oneType;

_extern type twoType;
_extern proc twoProc(i:int) : twoType;
// _extern iter twoIter(o:twoType) : void;
_extern var twoVar : twoType;

