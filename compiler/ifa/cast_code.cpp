/*
 * Copyright 2004-2020 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

switch (to->const_kind) {
 default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
 case NUM_KIND_BOOL:
   switch (from->const_kind) {
   default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   case NUM_KIND_BOOL:
     to->v_bool = (bool)!!from->bool_value(); break;
   case NUM_KIND_UINT:
     switch (from->num_index) {
     case INT_SIZE_8:
       to->v_bool = (bool)!!from->v_uint8; break;
     case INT_SIZE_16:
       to->v_bool = (bool)!!from->v_uint16; break;
     case INT_SIZE_32:
       to->v_bool = (bool)!!from->v_uint32; break;
     case INT_SIZE_64:
       to->v_bool = (bool)!!from->v_uint64; break;
     default:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   case NUM_KIND_INT:
     switch (from->num_index) {
     case INT_SIZE_8:
       to->v_bool = (bool)!!from->v_int8; break;
     case INT_SIZE_16:
       to->v_bool = (bool)!!from->v_int16; break;
     case INT_SIZE_32:
       to->v_bool = (bool)!!from->v_int32; break;
     case INT_SIZE_64:
       to->v_bool = (bool)!!from->v_int64; break;
     default:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   case NUM_KIND_REAL:
   case NUM_KIND_IMAG:
     switch (from->num_index) {
     case FLOAT_SIZE_32:
       to->v_bool = (bool)!!from->v_float32; break;
     case FLOAT_SIZE_64:
       to->v_bool = (bool)!!from->v_float64; break;
     default:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   case NUM_KIND_COMPLEX:
     switch (from->num_index) {
       case COMPLEX_SIZE_64:
         to->v_bool = ((bool)!!from->v_complex64.r) ||
                      ((bool)!!from->v_complex64.i);
         break;
       case COMPLEX_SIZE_128:
         to->v_bool = ((bool)!!from->v_complex128.r) ||
                      ((bool)!!from->v_complex128.i);
         break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   } break; // end to NUM_KIND_BOOL
 case NUM_KIND_UINT:
   switch (to->num_index) {
   case INT_SIZE_8:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_uint8 = (uint8_t)from->bool_value(); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_uint8 = (uint8_t)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_uint8 = (uint8_t)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_uint8 = (uint8_t)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_uint8 = (uint8_t)from->v_uint64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_uint8 = (uint8_t)from->v_int8; break;
       case INT_SIZE_16:
         to->v_uint8 = (uint8_t)from->v_int16; break;
       case INT_SIZE_32:
         to->v_uint8 = (uint8_t)from->v_int32; break;
       case INT_SIZE_64:
         to->v_uint8 = (uint8_t)from->v_int64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_uint8 = (uint8_t)from->v_float32; break;
       case FLOAT_SIZE_64:
         to->v_uint8 = (uint8_t)from->v_float64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       switch (from->num_index) {
       case COMPLEX_SIZE_64:
         to->v_uint8 = (uint8_t)from->v_complex64.r; break;
       case COMPLEX_SIZE_128:
         to->v_uint8 = (uint8_t)from->v_complex128.r; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_16:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_uint16 = (uint16_t)from->bool_value(); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_uint16 = (uint16_t)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_uint16 = (uint16_t)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_uint16 = (uint16_t)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_uint16 = (uint16_t)from->v_uint64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_uint16 = (uint16_t)from->v_int8; break;
       case INT_SIZE_16:
         to->v_uint16 = (uint16_t)from->v_int16; break;
       case INT_SIZE_32:
         to->v_uint16 = (uint16_t)from->v_int32; break;
       case INT_SIZE_64:
         to->v_uint16 = (uint16_t)from->v_int64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_uint16 = (uint16_t)from->v_float32; break;
       case FLOAT_SIZE_64:
         to->v_uint16 = (uint16_t)from->v_float64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       switch (from->num_index) {
       case COMPLEX_SIZE_64:
         to->v_uint16 = (uint16_t)from->v_complex64.r; break;
       case COMPLEX_SIZE_128:
         to->v_uint16 = (uint16_t)from->v_complex128.r; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_32:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_uint32 = (uint32_t)from->bool_value(); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_uint32 = (uint32_t)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_uint32 = (uint32_t)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_uint32 = (uint32_t)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_uint32 = (uint32_t)from->v_uint64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_uint32 = (uint32_t)from->v_int8; break;
       case INT_SIZE_16:
         to->v_uint32 = (uint32_t)from->v_int16; break;
       case INT_SIZE_32:
         to->v_uint32 = (uint32_t)from->v_int32; break;
       case INT_SIZE_64:
         to->v_uint32 = (uint32_t)from->v_int64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_uint32 = (uint32_t)from->v_float32; break;
       case FLOAT_SIZE_64:
         to->v_uint32 = (uint32_t)from->v_float64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       switch (from->num_index) {
       case COMPLEX_SIZE_64:
         to->v_uint32 = (uint32_t)from->v_complex64.r; break;
       case COMPLEX_SIZE_128:
         to->v_uint32 = (uint32_t)from->v_complex128.r; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_64:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_uint64 = (uint64_t)from->bool_value(); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_uint64 = (uint64_t)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_uint64 = (uint64_t)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_uint64 = (uint64_t)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_uint64 = (uint64_t)from->v_uint64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_uint64 = (uint64_t)from->v_int8; break;
       case INT_SIZE_16:
         to->v_uint64 = (uint64_t)from->v_int16; break;
       case INT_SIZE_32:
         to->v_uint64 = (uint64_t)from->v_int32; break;
       case INT_SIZE_64:
         to->v_uint64 = (uint64_t)from->v_int64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_uint64 = (uint64_t)from->v_float32; break;
       case FLOAT_SIZE_64:
         to->v_uint64 = (uint64_t)from->v_float64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       switch (from->num_index) {
       case COMPLEX_SIZE_64:
         to->v_uint64 = (uint64_t)from->v_complex64.r; break;
       case COMPLEX_SIZE_128:
         to->v_uint64 = (uint64_t)from->v_complex128.r; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   default:
     INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   } break; // end to NUM_KIND_UINT
 case NUM_KIND_INT:
   switch (to->num_index) {
   case INT_SIZE_8:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_int8 = (int8_t)from->bool_value(); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_int8 = (int8_t)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_int8 = (int8_t)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_int8 = (int8_t)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_int8 = (int8_t)from->v_uint64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_int8 = (int8_t)from->v_int8; break;
       case INT_SIZE_16:
         to->v_int8 = (int8_t)from->v_int16; break;
       case INT_SIZE_32:
         to->v_int8 = (int8_t)from->v_int32; break;
       case INT_SIZE_64:
         to->v_int8 = (int8_t)from->v_int64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_int8 = (int8_t)from->v_float32; break;
       case FLOAT_SIZE_64:
         to->v_int8 = (int8_t)from->v_float64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       switch (from->num_index) {
       case COMPLEX_SIZE_64:
         to->v_int8 = (int8_t)from->v_complex64.r; break;
       case COMPLEX_SIZE_128:
         to->v_int8 = (int8_t)from->v_complex128.r; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_16:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_int16 = (int16_t)from->bool_value(); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_int16 = (int16_t)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_int16 = (int16_t)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_int16 = (int16_t)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_int16 = (int16_t)from->v_uint64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_int16 = (int16_t)from->v_int8; break;
       case INT_SIZE_16:
         to->v_int16 = (int16_t)from->v_int16; break;
       case INT_SIZE_32:
         to->v_int16 = (int16_t)from->v_int32; break;
       case INT_SIZE_64:
         to->v_int16 = (int16_t)from->v_int64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_int16 = (int16_t)from->v_float32; break;
       case FLOAT_SIZE_64:
         to->v_int16 = (int16_t)from->v_float64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       switch (from->num_index) {
       case COMPLEX_SIZE_64:
         to->v_int16 = (int16_t)from->v_complex64.r; break;
       case COMPLEX_SIZE_128:
         to->v_int16 = (int16_t)from->v_complex128.r; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;

     } break;
   case INT_SIZE_32:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_int32 = (int32_t)from->bool_value(); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_int32 = (int32_t)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_int32 = (int32_t)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_int32 = (int32_t)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_int32 = (int32_t)from->v_uint64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_int32 = (int32_t)from->v_int8; break;
       case INT_SIZE_16:
         to->v_int32 = (int32_t)from->v_int16; break;
       case INT_SIZE_32:
         to->v_int32 = (int32_t)from->v_int32; break;
       case INT_SIZE_64:
         to->v_int32 = (int32_t)from->v_int64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_int32 = (int32_t)from->v_float32; break;
       case FLOAT_SIZE_64:
         to->v_int32 = (int32_t)from->v_float64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       switch (from->num_index) {
       case COMPLEX_SIZE_64:
         to->v_int32 = (int32_t)from->v_complex64.r; break;
       case COMPLEX_SIZE_128:
         to->v_int32 = (int32_t)from->v_complex128.r; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_64:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_int64 = (int64_t)from->bool_value(); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_int64 = (int64_t)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_int64 = (int64_t)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_int64 = (int64_t)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_int64 = (int64_t)from->v_uint64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_int64 = (int64_t)from->v_int8; break;
       case INT_SIZE_16:
         to->v_int64 = (int64_t)from->v_int16; break;
       case INT_SIZE_32:
         to->v_int64 = (int64_t)from->v_int32; break;
       case INT_SIZE_64:
         to->v_int64 = (int64_t)from->v_int64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_int64 = (int64_t)from->v_float32; break;
       case FLOAT_SIZE_64:
         to->v_int64 = (int64_t)from->v_float64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       switch (from->num_index) {
       case COMPLEX_SIZE_64:
         to->v_int64 = (int64_t)from->v_complex64.r; break;
       case COMPLEX_SIZE_128:
         to->v_int64 = (int64_t)from->v_complex128.r; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   default:
     INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   } break; // end to NUM_KIND_INT
 case NUM_KIND_REAL:
 case NUM_KIND_IMAG:
   switch (to->num_index) {
   case FLOAT_SIZE_32:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_float32 = (float)from->bool_value(); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_float32 = (float)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_float32 = (float)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_float32 = (float)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_float32 = (float)from->v_uint64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_float32 = (float)from->v_int8; break;
       case INT_SIZE_16:
         to->v_float32 = (float)from->v_int16; break;
       case INT_SIZE_32:
         to->v_float32 = (float)from->v_int32; break;
       case INT_SIZE_64:
         to->v_float32 = (float)from->v_int64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_float32 = (float)from->v_float32; break;
       case FLOAT_SIZE_64:
         to->v_float32 = (float)from->v_float64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       if (to->const_kind == NUM_KIND_REAL) {
         switch (from->num_index) {
         case COMPLEX_SIZE_64:
           to->v_float32 = (float)from->v_complex64.r; break;
         case COMPLEX_SIZE_128:
           to->v_float32 = (float)from->v_complex128.r; break;
         default:
           INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
         }
       } else if (to->const_kind == NUM_KIND_IMAG) {
         switch (from->num_index) {
         case COMPLEX_SIZE_64:
           to->v_float32 = (float)from->v_complex64.i; break;
         case COMPLEX_SIZE_128:
           to->v_float32 = (float)from->v_complex128.i; break;
         default:
           INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
         }
       } else {
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case FLOAT_SIZE_64:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_float64 = (double)from->bool_value(); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_float64 = (double)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_float64 = (double)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_float64 = (double)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_float64 = (double)from->v_uint64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_float64 = (double)from->v_int8; break;
       case INT_SIZE_16:
         to->v_float64 = (double)from->v_int16; break;
       case INT_SIZE_32:
         to->v_float64 = (double)from->v_int32; break;
       case INT_SIZE_64:
         to->v_float64 = (double)from->v_int64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_float64 = (double)from->v_float32; break;
       case FLOAT_SIZE_64:
         to->v_float64 = (double)from->v_float64; break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       if (to->const_kind == NUM_KIND_REAL) {
         switch (from->num_index) {
         case COMPLEX_SIZE_64:
           to->v_float64 = (double)from->v_complex64.r; break;
         case COMPLEX_SIZE_128:
           to->v_float64 = (double)from->v_complex128.r; break;
         default:
           INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
         }
       } else if (to->const_kind == NUM_KIND_IMAG) {
         switch (from->num_index) {
         case COMPLEX_SIZE_64:
           to->v_float64 = (double)from->v_complex64.i; break;
         case COMPLEX_SIZE_128:
           to->v_float64 = (double)from->v_complex128.i; break;
         default:
           INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
         }
       } else {
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   default:
     INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   } break; // end to NUM_KIND_REAL / NUM_KIND_IMAG
 case CONST_KIND_STRING:
   switch (from->const_kind) {
   default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   case NUM_KIND_BOOL:
     to->v_string = from->bool_value() ? astr("true") : astr("false"); break;
   case NUM_KIND_UINT:
     switch (from->num_index) {
     case INT_SIZE_8:
       to->v_string = istrFromUserUint(from->v_uint8); break;
     case INT_SIZE_16:
       to->v_string = istrFromUserUint(from->v_uint16); break;
     case INT_SIZE_32:
       to->v_string = istrFromUserUint(from->v_uint32); break;
     case INT_SIZE_64:
       to->v_string = istrFromUserUint(from->v_uint64); break;
     default:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   case NUM_KIND_INT:
     switch (from->num_index) {
     case INT_SIZE_8:
       to->v_string = istrFromUserInt(from->v_int8); break;
     case INT_SIZE_16:
       to->v_string = istrFromUserInt(from->v_int16); break;
     case INT_SIZE_32:
       to->v_string = istrFromUserInt(from->v_int32); break;
     case INT_SIZE_64:
       to->v_string = istrFromUserInt(from->v_int64); break;
     default:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   case NUM_KIND_REAL:
     switch (from->num_index) {
     case FLOAT_SIZE_32:
       to->v_string = istrFromUserDouble(from->v_float32); break;
     case FLOAT_SIZE_64:
       to->v_string = istrFromUserDouble(from->v_float64); break;
     default:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   case NUM_KIND_IMAG:
     switch (from->num_index) {
     case FLOAT_SIZE_32:
       to->v_string = istrFromUserImag(from->v_float32); break;
     case FLOAT_SIZE_64:
       to->v_string = istrFromUserImag(from->v_float64); break;
     default:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   case NUM_KIND_COMPLEX:
     switch (from->num_index) {
     case COMPLEX_SIZE_64:
       to->v_string = istrFromUserComplex(from->v_complex64.r,
                                          from->v_complex64.i);
       break;
     case COMPLEX_SIZE_128:
       to->v_string = istrFromUserComplex(from->v_complex128.r,
                                          from->v_complex128.i);
       break;
     default:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   } break;
 case NUM_KIND_COMPLEX:
   switch (to->num_index) {
   case COMPLEX_SIZE_64:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_complex64.r = (float)from->bool_value();
       to->v_complex64.i = (float)0;
       break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_complex64.r = (float)from->v_uint8;
         to->v_complex64.i = (float)0;
         break;
       case INT_SIZE_16:
         to->v_complex64.r = (float)from->v_uint16;
         to->v_complex64.i = (float)0;
         break;
       case INT_SIZE_32:
         to->v_complex64.r = (float)from->v_uint32;
         to->v_complex64.i = (float)0;
         break;
       case INT_SIZE_64:
         to->v_complex64.r = (float)from->v_uint64;
         to->v_complex64.i = (float)0;
         break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_complex64.r = (float)from->v_int8;
         to->v_complex64.i = (float)0;
         break;
       case INT_SIZE_16:
         to->v_complex64.r = (float)from->v_int16;
         to->v_complex64.i = (float)0;
         break;
       case INT_SIZE_32:
         to->v_complex64.r = (float)from->v_int32;
         to->v_complex64.i = (float)0;
         break;
       case INT_SIZE_64:
         to->v_complex64.r = (float)from->v_int64;
         to->v_complex64.i = (float)0;
         break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_complex64.r = (float)from->v_float32;
         to->v_complex64.i = (float)0;
         break;
       case FLOAT_SIZE_64:
         to->v_complex64.r = (float)from->v_float64;
         to->v_complex64.i = (float)0;
         break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_complex64.r = (float)0;
         to->v_complex64.i = (float)from->v_float32;
         break;
       case FLOAT_SIZE_64:
         to->v_complex64.r = (float)0;
         to->v_complex64.i = (float)from->v_float64;
         break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       switch (from->num_index) {
       case COMPLEX_SIZE_64:
         to->v_complex64.r = (float)from->v_complex64.r;
         to->v_complex64.i = (float)from->v_complex64.i;
         break;
       case COMPLEX_SIZE_128:
         to->v_complex64.r = (float)from->v_complex128.r;
         to->v_complex64.i = (float)from->v_complex128.i;
         break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break; // end to complex 64
   case COMPLEX_SIZE_128:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_BOOL:
       to->v_complex128.r = (double)from->bool_value();
       to->v_complex128.i = (double)0;
       break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_complex128.r = (double)from->v_uint8;
         to->v_complex128.i = (double)0;
         break;
       case INT_SIZE_16:
         to->v_complex128.r = (double)from->v_uint16;
         to->v_complex128.i = (double)0;
         break;
       case INT_SIZE_32:
         to->v_complex128.r = (double)from->v_uint32;
         to->v_complex128.i = (double)0;
         break;
       case INT_SIZE_64:
         to->v_complex128.r = (double)from->v_uint64;
         to->v_complex128.i = (double)0;
         break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_8:
         to->v_complex128.r = (double)from->v_int8;
         to->v_complex128.i = (double)0;
         break;
       case INT_SIZE_16:
         to->v_complex128.r = (double)from->v_int16;
         to->v_complex128.i = (double)0;
         break;
       case INT_SIZE_32:
         to->v_complex128.r = (double)from->v_int32;
         to->v_complex128.i = (double)0;
         break;
       case INT_SIZE_64:
         to->v_complex128.r = (double)from->v_int64;
         to->v_complex128.i = (double)0;
         break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_REAL:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_complex128.r = (double)from->v_float32;
         to->v_complex128.i = (double)0;
         break;
       case FLOAT_SIZE_64:
         to->v_complex128.r = (double)from->v_float64;
         to->v_complex128.i = (double)0;
         break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_IMAG:
       switch (from->num_index) {
       case FLOAT_SIZE_32:
         to->v_complex128.r = (double)0;
         to->v_complex128.i = (double)from->v_float32;
         break;
       case FLOAT_SIZE_64:
         to->v_complex128.r = (double)0;
         to->v_complex128.i = (double)from->v_float64;
         break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_COMPLEX:
       switch (from->num_index) {
       case COMPLEX_SIZE_64:
         to->v_complex128.r = (double)from->v_complex64.r;
         to->v_complex128.i = (double)from->v_complex64.i;
         break;
       case COMPLEX_SIZE_128:
         to->v_complex128.r = (double)from->v_complex128.r;
         to->v_complex128.i = (double)from->v_complex128.i;
         break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break; // end to complex 128
   } break;
 }
