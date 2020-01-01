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

#ifndef _prim_data_H
#define _prim_data_H

class Prim;

extern Prim *prim_operator;
#define P_prim_operator 0
extern Prim *prim_period;
#define P_prim_period 1
extern Prim *prim_setter;
#define P_prim_setter 2
extern Prim *prim_pow;
#define P_prim_pow 3
extern Prim *prim_mult;
#define P_prim_mult 4
extern Prim *prim_div;
#define P_prim_div 5
extern Prim *prim_mod;
#define P_prim_mod 6
extern Prim *prim_add;
#define P_prim_add 7
extern Prim *prim_subtract;
#define P_prim_subtract 8
extern Prim *prim_lsh;
#define P_prim_lsh 9
extern Prim *prim_rsh;
#define P_prim_rsh 10
extern Prim *prim_less;
#define P_prim_less 11
extern Prim *prim_lessorequal;
#define P_prim_lessorequal 12
extern Prim *prim_greater;
#define P_prim_greater 13
extern Prim *prim_greaterorequal;
#define P_prim_greaterorequal 14
extern Prim *prim_equal;
#define P_prim_equal 15
extern Prim *prim_notequal;
#define P_prim_notequal 16
extern Prim *prim_and;
#define P_prim_and 17
extern Prim *prim_xor;
#define P_prim_xor 18
extern Prim *prim_or;
#define P_prim_or 19
extern Prim *prim_land;
#define P_prim_land 20
extern Prim *prim_lor;
#define P_prim_lor 21
extern Prim *prim_assign;
#define P_prim_assign 22
extern Prim *prim_apply;
#define P_prim_apply 23
extern Prim *prim_by;
#define P_prim_by 24
extern Prim *prim_seqcat;
#define P_prim_seqcat 25
extern Prim *prim_plus;
#define P_prim_plus 26
extern Prim *prim_minus;
#define P_prim_minus 27
extern Prim *prim_not;
#define P_prim_not 28
extern Prim *prim_lnot;
#define P_prim_lnot 29
extern Prim *prim_deref;
#define P_prim_deref 30
extern Prim *prim_ref;
#define P_prim_ref 31
extern Prim *prim_cast;
#define P_prim_cast 32
extern Prim *prim_reply;
#define P_prim_reply 33
extern Prim *prim_tuple;
#define P_prim_tuple 34
extern Prim *prim_vector;
#define P_prim_vector 35
extern Prim *prim_continuation;
#define P_prim_continuation 36
extern Prim *prim_new;
#define P_prim_new 37
extern Prim *prim_index_object;
#define P_prim_index_object 38
extern Prim *prim_set_index_object;
#define P_prim_set_index_object 39
extern Prim *prim_meta_apply;
#define P_prim_meta_apply 40
extern Prim *prim_destruct;
#define P_prim_destruct 41
extern Prim *prim_set;
#define P_prim_set 42
extern Prim *prim_list;
#define P_prim_list 43
extern Prim *prim_coerce;
#define P_prim_coerce 44
extern Prim *prim_primitive;
#define P_prim_primitive 45
#endif
