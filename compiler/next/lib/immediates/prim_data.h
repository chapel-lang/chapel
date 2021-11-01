/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef PRIM_DATA_H
#define PRIM_DATA_H

#include "chpl/uast/PrimOp.h"

#define P_prim_pow            chpl::uast::primtags::PRIM_POW
#define P_prim_mult           chpl::uast::primtags::PRIM_MULT
#define P_prim_div            chpl::uast::primtags::PRIM_DIV
#define P_prim_mod            chpl::uast::primtags::PRIM_MOD
#define P_prim_add            chpl::uast::primtags::PRIM_ADD
#define P_prim_subtract       chpl::uast::primtags::PRIM_SUBTRACT
#define P_prim_lsh            chpl::uast::primtags::PRIM_LSH
#define P_prim_rsh            chpl::uast::primtags::PRIM_RSH
#define P_prim_less           chpl::uast::primtags::PRIM_LESS
#define P_prim_lessorequal    chpl::uast::primtags::PRIM_LESSOREQUAL
#define P_prim_greater        chpl::uast::primtags::PRIM_GREATER
#define P_prim_greaterorequal chpl::uast::primtags::PRIM_GREATEROREQUAL
#define P_prim_equal          chpl::uast::primtags::PRIM_EQUAL
#define P_prim_notequal       chpl::uast::primtags::PRIM_NOTEQUAL
#define P_prim_and            chpl::uast::primtags::PRIM_AND
#define P_prim_xor            chpl::uast::primtags::PRIM_XOR
#define P_prim_or             chpl::uast::primtags::PRIM_OR
#define P_prim_land           chpl::uast::primtags::PRIM_LOGICAL_AND
#define P_prim_lor            chpl::uast::primtags::PRIM_LOGICAL_OR
#define P_prim_plus           chpl::uast::primtags::PRIM_UNARY_PLUS
#define P_prim_minus          chpl::uast::primtags::PRIM_UNARY_MINUS
#define P_prim_not            chpl::uast::primtags::PRIM_UNARY_NOT
#define P_prim_lnot           chpl::uast::primtags::PRIM_UNARY_LNOT

#endif
