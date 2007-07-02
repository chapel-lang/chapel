switch (to->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1: 
switch (to->num_index) {
case 0:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_bool = (bool)!!from->v_bool; break;
case 1:
to->v_bool = (bool)!!from->v_uint8; break;
case 2:
to->v_bool = (bool)!!from->v_uint16; break;
case 3:
to->v_bool = (bool)!!from->v_uint32; break;
case 4:
to->v_bool = (bool)!!from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_bool = (bool)!!from->v_bool; break;
case 1:
to->v_bool = (bool)!!from->v_int8; break;
case 2:
to->v_bool = (bool)!!from->v_int16; break;
case 3:
to->v_bool = (bool)!!from->v_int32; break;
case 4:
to->v_bool = (bool)!!from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_bool = (bool)!!from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_bool = (bool)!!from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_bool = (bool)!!from->v_float128; break;
} break;
} break;
case 1:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_uint8 = (uint8)from->v_bool; break;
case 1:
to->v_uint8 = (uint8)from->v_uint8; break;
case 2:
to->v_uint8 = (uint8)from->v_uint16; break;
case 3:
to->v_uint8 = (uint8)from->v_uint32; break;
case 4:
to->v_uint8 = (uint8)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_uint8 = (uint8)from->v_bool; break;
case 1:
to->v_uint8 = (uint8)from->v_int8; break;
case 2:
to->v_uint8 = (uint8)from->v_int16; break;
case 3:
to->v_uint8 = (uint8)from->v_int32; break;
case 4:
to->v_uint8 = (uint8)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_uint8 = (uint8)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_uint8 = (uint8)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_uint8 = (uint8)from->v_float128; break;
} break;
} break;
case 2:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_uint16 = (uint16)from->v_bool; break;
case 1:
to->v_uint16 = (uint16)from->v_uint8; break;
case 2:
to->v_uint16 = (uint16)from->v_uint16; break;
case 3:
to->v_uint16 = (uint16)from->v_uint32; break;
case 4:
to->v_uint16 = (uint16)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_uint16 = (uint16)from->v_bool; break;
case 1:
to->v_uint16 = (uint16)from->v_int8; break;
case 2:
to->v_uint16 = (uint16)from->v_int16; break;
case 3:
to->v_uint16 = (uint16)from->v_int32; break;
case 4:
to->v_uint16 = (uint16)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_uint16 = (uint16)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_uint16 = (uint16)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_uint16 = (uint16)from->v_float128; break;
} break;
} break;
case 3:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_uint32 = (uint32)from->v_bool; break;
case 1:
to->v_uint32 = (uint32)from->v_uint8; break;
case 2:
to->v_uint32 = (uint32)from->v_uint16; break;
case 3:
to->v_uint32 = (uint32)from->v_uint32; break;
case 4:
to->v_uint32 = (uint32)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_uint32 = (uint32)from->v_bool; break;
case 1:
to->v_uint32 = (uint32)from->v_int8; break;
case 2:
to->v_uint32 = (uint32)from->v_int16; break;
case 3:
to->v_uint32 = (uint32)from->v_int32; break;
case 4:
to->v_uint32 = (uint32)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_uint32 = (uint32)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_uint32 = (uint32)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_uint32 = (uint32)from->v_float128; break;
} break;
} break;
case 4:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_uint64 = (uint64)from->v_bool; break;
case 1:
to->v_uint64 = (uint64)from->v_uint8; break;
case 2:
to->v_uint64 = (uint64)from->v_uint16; break;
case 3:
to->v_uint64 = (uint64)from->v_uint32; break;
case 4:
to->v_uint64 = (uint64)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_uint64 = (uint64)from->v_bool; break;
case 1:
to->v_uint64 = (uint64)from->v_int8; break;
case 2:
to->v_uint64 = (uint64)from->v_int16; break;
case 3:
to->v_uint64 = (uint64)from->v_int32; break;
case 4:
to->v_uint64 = (uint64)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_uint64 = (uint64)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_uint64 = (uint64)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_uint64 = (uint64)from->v_float128; break;
} break;
} break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2: 
switch (to->num_index) {
case 0:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_bool = (bool)from->v_bool; break;
case 1:
to->v_bool = (bool)from->v_uint8; break;
case 2:
to->v_bool = (bool)from->v_uint16; break;
case 3:
to->v_bool = (bool)from->v_uint32; break;
case 4:
to->v_bool = (bool)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_bool = (bool)from->v_bool; break;
case 1:
to->v_bool = (bool)from->v_int8; break;
case 2:
to->v_bool = (bool)from->v_int16; break;
case 3:
to->v_bool = (bool)from->v_int32; break;
case 4:
to->v_bool = (bool)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_bool = (bool)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_bool = (bool)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_bool = (bool)from->v_float128; break;
} break;
} break;
case 1:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_int8 = (int8)from->v_bool; break;
case 1:
to->v_int8 = (int8)from->v_uint8; break;
case 2:
to->v_int8 = (int8)from->v_uint16; break;
case 3:
to->v_int8 = (int8)from->v_uint32; break;
case 4:
to->v_int8 = (int8)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_int8 = (int8)from->v_bool; break;
case 1:
to->v_int8 = (int8)from->v_int8; break;
case 2:
to->v_int8 = (int8)from->v_int16; break;
case 3:
to->v_int8 = (int8)from->v_int32; break;
case 4:
to->v_int8 = (int8)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_int8 = (int8)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_int8 = (int8)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_int8 = (int8)from->v_float128; break;
} break;
} break;
case 2:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_int16 = (int16)from->v_bool; break;
case 1:
to->v_int16 = (int16)from->v_uint8; break;
case 2:
to->v_int16 = (int16)from->v_uint16; break;
case 3:
to->v_int16 = (int16)from->v_uint32; break;
case 4:
to->v_int16 = (int16)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_int16 = (int16)from->v_bool; break;
case 1:
to->v_int16 = (int16)from->v_int8; break;
case 2:
to->v_int16 = (int16)from->v_int16; break;
case 3:
to->v_int16 = (int16)from->v_int32; break;
case 4:
to->v_int16 = (int16)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_int16 = (int16)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_int16 = (int16)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_int16 = (int16)from->v_float128; break;
} break;
} break;
case 3:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_int32 = (int32)from->v_bool; break;
case 1:
to->v_int32 = (int32)from->v_uint8; break;
case 2:
to->v_int32 = (int32)from->v_uint16; break;
case 3:
to->v_int32 = (int32)from->v_uint32; break;
case 4:
to->v_int32 = (int32)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_int32 = (int32)from->v_bool; break;
case 1:
to->v_int32 = (int32)from->v_int8; break;
case 2:
to->v_int32 = (int32)from->v_int16; break;
case 3:
to->v_int32 = (int32)from->v_int32; break;
case 4:
to->v_int32 = (int32)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_int32 = (int32)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_int32 = (int32)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_int32 = (int32)from->v_float128; break;
} break;
} break;
case 4:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_int64 = (int64)from->v_bool; break;
case 1:
to->v_int64 = (int64)from->v_uint8; break;
case 2:
to->v_int64 = (int64)from->v_uint16; break;
case 3:
to->v_int64 = (int64)from->v_uint32; break;
case 4:
to->v_int64 = (int64)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_int64 = (int64)from->v_bool; break;
case 1:
to->v_int64 = (int64)from->v_int8; break;
case 2:
to->v_int64 = (int64)from->v_int16; break;
case 3:
to->v_int64 = (int64)from->v_int32; break;
case 4:
to->v_int64 = (int64)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_int64 = (int64)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_int64 = (int64)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_int64 = (int64)from->v_float128; break;
} break;
} break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3: 
switch (to->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_float32 = (float32)from->v_bool; break;
case 1:
to->v_float32 = (float32)from->v_uint8; break;
case 2:
to->v_float32 = (float32)from->v_uint16; break;
case 3:
to->v_float32 = (float32)from->v_uint32; break;
case 4:
to->v_float32 = (float32)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_float32 = (float32)from->v_bool; break;
case 1:
to->v_float32 = (float32)from->v_int8; break;
case 2:
to->v_float32 = (float32)from->v_int16; break;
case 3:
to->v_float32 = (float32)from->v_int32; break;
case 4:
to->v_float32 = (float32)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_float32 = (float32)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_float32 = (float32)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_float32 = (float32)from->v_float128; break;
} break;
} break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_float64 = (float64)from->v_bool; break;
case 1:
to->v_float64 = (float64)from->v_uint8; break;
case 2:
to->v_float64 = (float64)from->v_uint16; break;
case 3:
to->v_float64 = (float64)from->v_uint32; break;
case 4:
to->v_float64 = (float64)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_float64 = (float64)from->v_bool; break;
case 1:
to->v_float64 = (float64)from->v_int8; break;
case 2:
to->v_float64 = (float64)from->v_int16; break;
case 3:
to->v_float64 = (float64)from->v_int32; break;
case 4:
to->v_float64 = (float64)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_float64 = (float64)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_float64 = (float64)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_float64 = (float64)from->v_float128; break;
} break;
} break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
switch (from->const_kind) {
default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
switch (from->num_index) {
case 0:
to->v_float128 = (float128)from->v_bool; break;
case 1:
to->v_float128 = (float128)from->v_uint8; break;
case 2:
to->v_float128 = (float128)from->v_uint16; break;
case 3:
to->v_float128 = (float128)from->v_uint32; break;
case 4:
to->v_float128 = (float128)from->uint_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 2:
switch (from->num_index) {
case 0:
to->v_float128 = (float128)from->v_bool; break;
case 1:
to->v_float128 = (float128)from->v_int8; break;
case 2:
to->v_float128 = (float128)from->v_int16; break;
case 3:
to->v_float128 = (float128)from->v_int32; break;
case 4:
to->v_float128 = (float128)from->int_value(); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
} break;
case 3:
switch (from->num_index) {
case 0:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 1:
to->v_float128 = (float128)from->v_float32; break;
case 2:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 3:
to->v_float128 = (float128)from->v_float64; break;
case 4:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 5:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 6:
INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
case 7:
to->v_float128 = (float128)from->v_float128; break;
} break;
} break;
} break;
}
