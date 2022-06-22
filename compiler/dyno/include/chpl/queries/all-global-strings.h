/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

/*
   This uses the X macro pattern. The X macro is not yet defined here.
   Look in global-strings.h for uses of #include "all-global-strings.h" and
   you'll find a `#define X(field, str) ...` just before it.
 */

/* X(identifier, chapel string name) */
/* the identifier must be a legal C++ identifier */

X(align          , "align")
X(atomic         , "atomic")
X(bool_          , "bool")
X(borrowed       , "borrowed")
X(by             , "by")
X(bytes          , "bytes")
X(complex_       , "complex")
X(deinit         , "deinit")
X(dmapped        , "dmapped")
X(domain         , "domain")
X(imag_          , "imag")
X(index          , "index")
X(init           , "init")
X(initequals     , "init=")
X(int_           , "int")
X(isCoercible    , "isCoercible")
X(locale         , "locale")
X(nil            , "nil")
X(noinit         , "noinit")
X(object_        , "object")
X(owned          , "owned")
X(real_          , "real")
X(reduce         , "reduce")
X(reduceAssign   , "reduce=")
X(scan           , "scan")
X(shared         , "shared")
X(single         , "single")
X(sparse         , "sparse")
X(string         , "string")
X(subdomain      , "subdomain")
X(sync           , "sync")
X(this_          , "this")
X(these_         , "these")
X(type           , "type")
X(tuple_         , "_tuple")
X(uint_          , "uint")
X(unmanaged      , "unmanaged")
X(void_          , "void")

X(equals         , "=")
X(question       , "?")
X(tripleDot      , "...")

X(andEquals      , "&=")
X(divEquals      , "/=")
X(epxEquals      , "**=")
X(lshiftEquals   , "<<=")
X(minusEquals    , "-=")
X(modEquals      , "%=")
X(orEquals       , "|=")
X(plusEquals     , "+=")
X(rshiftEquals   , ">>=")
X(timesEquals    , "*=")
X(xorEquals      , "^=")

X(logicAndAssign , "&&=")
X(logicOrAssign  , "||=")

X(equalsGreater  , "=>")

X(bitwiseAnd     , "&")
X(bitwiseOr      , "|")
X(bitwiseXor     , "^")
X(logicalAnd     , "&&")
X(logicalOr      , "||")
X(plus           , "+")
X(minus          , "-")
X(times          , "*")
X(compareEqual   , "==")
X(notEqual       , "!=")
X(greaterOrEqual , ">=")
X(greaterThan    , ">")
X(lessThan       , "<")
X(lessOrEqual    , "<=")
X(underscore     , "_")
X(postfixBang    , "postfix!")
X(logicalNot     , "!")
X(colon          , ":")
X(exponentiation , "**")
X(bitwiseNot     , "~")
X(divide         , "/")
X(modulus        , "%")
X(leftShift      , "<<")
X(rightShift     , ">>")
X(rangeCount     , "#")

/* A string too long is checked at compile time */
/* X(somethingtoolong      , "somethingtoolongforthemacro") */
