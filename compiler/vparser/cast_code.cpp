switch (t->num_kind) {
default: assert(!"case"); break;
case 1: 
switch (t->num_index) {
case 0:
switch (s->type->num_kind) {
default: assert(!"case"); break;
case 1:
switch (s->type->num_index) {
case 0:
im->v_bool = (bool)s->imm.v_bool; break;
case 1:
im->v_bool = (bool)s->imm.v_uint8; break;
case 2:
im->v_bool = (bool)s->imm.v_uint16; break;
case 3:
im->v_bool = (bool)s->imm.v_uint32; break;
case 4:
im->v_bool = (bool)s->imm.v_uint64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_bool = (bool)s->imm.v_int8; break;
case 2:
im->v_bool = (bool)s->imm.v_int16; break;
case 3:
im->v_bool = (bool)s->imm.v_int32; break;
case 4:
im->v_bool = (bool)s->imm.v_int64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_bool = (bool)s->imm.v_float32; break;
case 2:
assert(!"case"); break;
case 3:
im->v_bool = (bool)s->imm.v_float64; break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
} break;
case 1:
switch (s->type->num_kind) {
default: assert(!"case"); break;
case 1:
switch (s->type->num_index) {
case 0:
im->v_uint8 = (uint8)s->imm.v_bool; break;
case 1:
im->v_uint8 = (uint8)s->imm.v_uint8; break;
case 2:
im->v_uint8 = (uint8)s->imm.v_uint16; break;
case 3:
im->v_uint8 = (uint8)s->imm.v_uint32; break;
case 4:
im->v_uint8 = (uint8)s->imm.v_uint64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_uint8 = (uint8)s->imm.v_int8; break;
case 2:
im->v_uint8 = (uint8)s->imm.v_int16; break;
case 3:
im->v_uint8 = (uint8)s->imm.v_int32; break;
case 4:
im->v_uint8 = (uint8)s->imm.v_int64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_uint8 = (uint8)s->imm.v_float32; break;
case 2:
assert(!"case"); break;
case 3:
im->v_uint8 = (uint8)s->imm.v_float64; break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
} break;
case 2:
switch (s->type->num_kind) {
default: assert(!"case"); break;
case 1:
switch (s->type->num_index) {
case 0:
im->v_uint16 = (uint16)s->imm.v_bool; break;
case 1:
im->v_uint16 = (uint16)s->imm.v_uint8; break;
case 2:
im->v_uint16 = (uint16)s->imm.v_uint16; break;
case 3:
im->v_uint16 = (uint16)s->imm.v_uint32; break;
case 4:
im->v_uint16 = (uint16)s->imm.v_uint64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_uint16 = (uint16)s->imm.v_int8; break;
case 2:
im->v_uint16 = (uint16)s->imm.v_int16; break;
case 3:
im->v_uint16 = (uint16)s->imm.v_int32; break;
case 4:
im->v_uint16 = (uint16)s->imm.v_int64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_uint16 = (uint16)s->imm.v_float32; break;
case 2:
assert(!"case"); break;
case 3:
im->v_uint16 = (uint16)s->imm.v_float64; break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
} break;
case 3:
switch (s->type->num_kind) {
default: assert(!"case"); break;
case 1:
switch (s->type->num_index) {
case 0:
im->v_uint32 = (uint32)s->imm.v_bool; break;
case 1:
im->v_uint32 = (uint32)s->imm.v_uint8; break;
case 2:
im->v_uint32 = (uint32)s->imm.v_uint16; break;
case 3:
im->v_uint32 = (uint32)s->imm.v_uint32; break;
case 4:
im->v_uint32 = (uint32)s->imm.v_uint64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_uint32 = (uint32)s->imm.v_int8; break;
case 2:
im->v_uint32 = (uint32)s->imm.v_int16; break;
case 3:
im->v_uint32 = (uint32)s->imm.v_int32; break;
case 4:
im->v_uint32 = (uint32)s->imm.v_int64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_uint32 = (uint32)s->imm.v_float32; break;
case 2:
assert(!"case"); break;
case 3:
im->v_uint32 = (uint32)s->imm.v_float64; break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
} break;
case 4:
switch (s->type->num_kind) {
default: assert(!"case"); break;
case 1:
switch (s->type->num_index) {
case 0:
im->v_uint64 = (uint64)s->imm.v_bool; break;
case 1:
im->v_uint64 = (uint64)s->imm.v_uint8; break;
case 2:
im->v_uint64 = (uint64)s->imm.v_uint16; break;
case 3:
im->v_uint64 = (uint64)s->imm.v_uint32; break;
case 4:
im->v_uint64 = (uint64)s->imm.v_uint64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_uint64 = (uint64)s->imm.v_int8; break;
case 2:
im->v_uint64 = (uint64)s->imm.v_int16; break;
case 3:
im->v_uint64 = (uint64)s->imm.v_int32; break;
case 4:
im->v_uint64 = (uint64)s->imm.v_int64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_uint64 = (uint64)s->imm.v_float32; break;
case 2:
assert(!"case"); break;
case 3:
im->v_uint64 = (uint64)s->imm.v_float64; break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
} break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2: 
switch (t->num_index) {
case 0:
assert(!"case"); break;
case 1:
switch (s->type->num_kind) {
default: assert(!"case"); break;
case 1:
switch (s->type->num_index) {
case 0:
im->v_int8 = (int8)s->imm.v_bool; break;
case 1:
im->v_int8 = (int8)s->imm.v_uint8; break;
case 2:
im->v_int8 = (int8)s->imm.v_uint16; break;
case 3:
im->v_int8 = (int8)s->imm.v_uint32; break;
case 4:
im->v_int8 = (int8)s->imm.v_uint64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_int8 = (int8)s->imm.v_int8; break;
case 2:
im->v_int8 = (int8)s->imm.v_int16; break;
case 3:
im->v_int8 = (int8)s->imm.v_int32; break;
case 4:
im->v_int8 = (int8)s->imm.v_int64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_int8 = (int8)s->imm.v_float32; break;
case 2:
assert(!"case"); break;
case 3:
im->v_int8 = (int8)s->imm.v_float64; break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
} break;
case 2:
switch (s->type->num_kind) {
default: assert(!"case"); break;
case 1:
switch (s->type->num_index) {
case 0:
im->v_int16 = (int16)s->imm.v_bool; break;
case 1:
im->v_int16 = (int16)s->imm.v_uint8; break;
case 2:
im->v_int16 = (int16)s->imm.v_uint16; break;
case 3:
im->v_int16 = (int16)s->imm.v_uint32; break;
case 4:
im->v_int16 = (int16)s->imm.v_uint64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_int16 = (int16)s->imm.v_int8; break;
case 2:
im->v_int16 = (int16)s->imm.v_int16; break;
case 3:
im->v_int16 = (int16)s->imm.v_int32; break;
case 4:
im->v_int16 = (int16)s->imm.v_int64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_int16 = (int16)s->imm.v_float32; break;
case 2:
assert(!"case"); break;
case 3:
im->v_int16 = (int16)s->imm.v_float64; break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
} break;
case 3:
switch (s->type->num_kind) {
default: assert(!"case"); break;
case 1:
switch (s->type->num_index) {
case 0:
im->v_int32 = (int32)s->imm.v_bool; break;
case 1:
im->v_int32 = (int32)s->imm.v_uint8; break;
case 2:
im->v_int32 = (int32)s->imm.v_uint16; break;
case 3:
im->v_int32 = (int32)s->imm.v_uint32; break;
case 4:
im->v_int32 = (int32)s->imm.v_uint64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_int32 = (int32)s->imm.v_int8; break;
case 2:
im->v_int32 = (int32)s->imm.v_int16; break;
case 3:
im->v_int32 = (int32)s->imm.v_int32; break;
case 4:
im->v_int32 = (int32)s->imm.v_int64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_int32 = (int32)s->imm.v_float32; break;
case 2:
assert(!"case"); break;
case 3:
im->v_int32 = (int32)s->imm.v_float64; break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
} break;
case 4:
switch (s->type->num_kind) {
default: assert(!"case"); break;
case 1:
switch (s->type->num_index) {
case 0:
im->v_int64 = (int64)s->imm.v_bool; break;
case 1:
im->v_int64 = (int64)s->imm.v_uint8; break;
case 2:
im->v_int64 = (int64)s->imm.v_uint16; break;
case 3:
im->v_int64 = (int64)s->imm.v_uint32; break;
case 4:
im->v_int64 = (int64)s->imm.v_uint64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_int64 = (int64)s->imm.v_int8; break;
case 2:
im->v_int64 = (int64)s->imm.v_int16; break;
case 3:
im->v_int64 = (int64)s->imm.v_int32; break;
case 4:
im->v_int64 = (int64)s->imm.v_int64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_int64 = (int64)s->imm.v_float32; break;
case 2:
assert(!"case"); break;
case 3:
im->v_int64 = (int64)s->imm.v_float64; break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
} break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3: 
switch (t->num_index) {
case 0:
assert(!"case"); break;
case 1:
switch (s->type->num_kind) {
default: assert(!"case"); break;
case 1:
switch (s->type->num_index) {
case 0:
im->v_float32 = (float32)s->imm.v_bool; break;
case 1:
im->v_float32 = (float32)s->imm.v_uint8; break;
case 2:
im->v_float32 = (float32)s->imm.v_uint16; break;
case 3:
im->v_float32 = (float32)s->imm.v_uint32; break;
case 4:
im->v_float32 = (float32)s->imm.v_uint64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_float32 = (float32)s->imm.v_int8; break;
case 2:
im->v_float32 = (float32)s->imm.v_int16; break;
case 3:
im->v_float32 = (float32)s->imm.v_int32; break;
case 4:
im->v_float32 = (float32)s->imm.v_int64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_float32 = (float32)s->imm.v_float32; break;
case 2:
assert(!"case"); break;
case 3:
im->v_float32 = (float32)s->imm.v_float64; break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
} break;
case 2:
assert(!"case"); break;
case 3:
switch (s->type->num_kind) {
default: assert(!"case"); break;
case 1:
switch (s->type->num_index) {
case 0:
im->v_float64 = (float64)s->imm.v_bool; break;
case 1:
im->v_float64 = (float64)s->imm.v_uint8; break;
case 2:
im->v_float64 = (float64)s->imm.v_uint16; break;
case 3:
im->v_float64 = (float64)s->imm.v_uint32; break;
case 4:
im->v_float64 = (float64)s->imm.v_uint64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 2:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_float64 = (float64)s->imm.v_int8; break;
case 2:
im->v_float64 = (float64)s->imm.v_int16; break;
case 3:
im->v_float64 = (float64)s->imm.v_int32; break;
case 4:
im->v_float64 = (float64)s->imm.v_int64; break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
case 3:
switch (s->type->num_index) {
case 0:
assert(!"case"); break;
case 1:
im->v_float64 = (float64)s->imm.v_float32; break;
case 2:
assert(!"case"); break;
case 3:
im->v_float64 = (float64)s->imm.v_float64; break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
} break;
case 4:
assert(!"case"); break;
case 5:
assert(!"case"); break;
case 6:
assert(!"case"); break;
case 7:
assert(!"case"); break;
} break;
}
