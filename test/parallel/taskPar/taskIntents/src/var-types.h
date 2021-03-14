lvar(b0,  bool,     true)
lvar(b8,  bool(8),  true)
lvar(b16, bool(16), true)
lvar(b32, bool(32), true)
lvar(b64, bool(64), true)

lvar(u8,  uint(8),  78)
lvar(u16, uint(16), 716)
lvar(u32, uint(32), 70032)
lvar(u64, uint(64), 70064)

lvar(i8,  int(8),  88)
lvar(i16, int(16), 816)
lvar(i32, int(32), 80032)
lvar(i64, int(64), 80064)

lvar(r32, real(32), (5.032:real(32)))
lvar(r64, real(64), 5.064)
lvar(m32, imag(32), (6.032i:imag(32)))
lvar(m64, imag(64), 6.064i)
lvar(z64,  complex(64),  ((-2+64i):complex(64)))
lvar(z128, complex(128), (-2+128i))

/* no strings for now */
lvar(enm, EnumType, EnumType.eLast)
/* no records for now */
/* no unions for now */
lvar(cls,  ClassType, cInstance)
/* no tuples for now */

lvar(dom1, DomType1, init1dom)
lvar(dom2, DomType2, init2dom)
lvar(arr1, ArrType1, init1arr)
lvar(arr2, ArrType2, init2arr)
