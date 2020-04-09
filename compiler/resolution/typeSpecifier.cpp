/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "typeSpecifier.h"

#include "expr.h"
#include "symbol.h"

static int typeSize(VarSymbol* var);

Type* typeForTypeSpecifier(Expr* expr, bool fatalOK) {
  Type* retval = NULL;

  if (SymExpr* symExpr = toSymExpr(expr)) {
    if (TypeSymbol* sym = toTypeSymbol(symExpr->symbol())) {
      retval = sym->type;
    }

  } else if (CallExpr* call = toCallExpr(expr)) {
    if (SymExpr* symExpr = toSymExpr(call->baseExpr)) {
      if (TypeSymbol* type = toTypeSymbol(symExpr->symbol())) {
        if (call->numActuals() == 1) {
          if (SymExpr* arg = toSymExpr(call->get(1))) {
            if (VarSymbol* var = toVarSymbol(arg->symbol())) {
              if (var->immediate) {
                if (NUM_KIND_INT  == var->immediate->const_kind ||
                    NUM_KIND_UINT == var->immediate->const_kind) {
                  if (type == dtBools[BOOL_SIZE_SYS]->symbol) {
                    int size = typeSize(var);

                    switch (size) {
                    case BOOL_SYS_WIDTH:
                      retval = dtBools[BOOL_SIZE_SYS];
                      break;

                    case 8:
                      retval = dtBools[BOOL_SIZE_8];
                      break;

                    case 16:
                      retval = dtBools[BOOL_SIZE_16];
                      break;

                    case 32:
                      retval = dtBools[BOOL_SIZE_32];
                      break;

                    case 64:
                      retval = dtBools[BOOL_SIZE_64];
                      break;

                    default:
                      if (fatalOK == true) {
                        USR_FATAL_CONT(call, "illegal size %d for bool", size);
                      }
                      break;
                    }


                  } else if (type == dtInt[INT_SIZE_DEFAULT]->symbol) {
                    int size = typeSize(var);

                    switch (size) {
                    case 8:
                      retval = dtInt[INT_SIZE_8];
                      break;

                    case 16:
                      retval = dtInt[INT_SIZE_16];
                      break;

                    case 32:
                      retval = dtInt[INT_SIZE_32];
                      break;

                    case 64:
                      retval = dtInt[INT_SIZE_64];
                      break;

                    default:
                      if (fatalOK == true) {
                        USR_FATAL_CONT(call, "illegal size %d for int", size);
                      }
                      break;
                    }


                  } else if (type == dtUInt[INT_SIZE_DEFAULT]->symbol) {
                    int size = typeSize(var);

                    switch (size) {
                    case  8:
                      retval = dtUInt[INT_SIZE_8];
                      break;

                    case 16:
                      retval = dtUInt[INT_SIZE_16];
                      break;

                    case 32:
                      retval = dtUInt[INT_SIZE_32];
                      break;

                    case 64:
                      retval = dtUInt[INT_SIZE_64];
                      break;

                    default:
                      if (fatalOK == true) {
                        USR_FATAL_CONT(call, "illegal size %d for uint", size);
                      }
                      break;
                    }


                  } else if (type == dtReal[FLOAT_SIZE_64]->symbol) {
                    int size = typeSize(var);

                    switch (size) {
                    case 32:
                      retval = dtReal[FLOAT_SIZE_32];
                      break;

                    case 64:
                      retval = dtReal[FLOAT_SIZE_64];
                      break;

                    default:
                      if (fatalOK == true) {
                        USR_FATAL_CONT(call, "illegal size %d for real", size);
                      }
                      break;
                    }


                  } else if (type == dtImag[FLOAT_SIZE_64]->symbol) {
                    int size = typeSize(var);

                    switch (size) {
                    case 32:
                      retval = dtImag[FLOAT_SIZE_32];
                      break;

                    case 64:
                      retval = dtImag[FLOAT_SIZE_64];
                      break;

                    default:
                      if (fatalOK == true) {
                        USR_FATAL_CONT(call, "illegal size %d for imag", size);
                      }
                      break;
                    }


                  } else if (type == dtComplex[COMPLEX_SIZE_128]->symbol) {
                    int size = typeSize(var);

                    switch (size) {
                    case 64:
                      retval = dtComplex[COMPLEX_SIZE_64];
                      break;

                    case 128:
                      retval = dtComplex[COMPLEX_SIZE_128];
                      break;

                    default:
                      if (fatalOK == true) {
                        USR_FATAL_CONT(call,
                                       "illegal size %d for complex",
                                       size);
                      }
                      break;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return retval;
}


static int typeSize(VarSymbol* var) {
  int retval = 0;

  if (NUM_KIND_INT == var->immediate->const_kind) {
    retval = var->immediate->int_value();

  } else {
    retval = var->immediate->uint_value();
  }

  return retval;
}
