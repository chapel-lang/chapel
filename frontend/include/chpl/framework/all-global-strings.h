/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

X(tombstone           , "<tombstone>")
X(empty               , "<empty>")
X(align               , "align")
X(assertOnGpu         , "assertOnGpu")
X(atomic              , "atomic")
X(bool_               , "bool")
X(borrow              , "borrow")
X(borrowed            , "borrowed")
X(by                  , "by")
X(bytes               , "bytes")
X(coforall            , "coforall")
X(complex_            , "complex")
X(complete            , "complete")
X(c_ptr               , "c_ptr")
X(c_ptrConst          , "c_ptrConst")
X(c_char              , "c_char")
X(class_              , "class")
X(deinit              , "deinit")
X(deserialize         , "deserialize")
X(dmapped             , "dmapped")
X(domain              , "domain")
X(enterContext        , "enterContext")
X(exitContext         , "exitContext")
X(false_              , "false")
X(follower            , "follower")
X(followThis          , "followThis")
X(for_                , "for")
X(forall              , "forall")
X(foreach             , "foreach")
X(functionStatic      , "functionStatic")
X(generate            , "generate")
X(gpuBlockSize        , "gpu.blockSize")
X(hash_               , "hash")
X(imag_               , "imag")
X(index               , "index")
X(init                , "init")
X(initequals          , "init=")
X(int_                , "int")
X(isCoercible         , "isCoercible")
X(leader              , "leader")
X(locale              , "locale")
X(main                , "main")
X(max                 , "max")
X(maxloc              , "maxloc")
X(min                 , "min")
X(minloc              , "minloc")
X(minmax              , "minmax")
X(nil                 , "nil")
X(nilable             , "nilable")
X(noinit              , "noinit")
X(none                , "none")
X(nothing             , "nothing")
X(object_             , "object")
X(owned               , "owned")
X(postinit            , "postinit")
X(real_               , "real")
X(reduce              , "reduce")
X(reduceAssign        , "reduce=")
X(RootClass           , "RootClass")
X(scan                , "scan")
X(serialize           , "serialize")
X(shared              , "shared")
X(single              , "single")
X(sparse              , "sparse")
X(stable              , "stable")
X(standalone          , "standalone")
X(string              , "string")
X(subdomain           , "subdomain")
X(super_              , "super")
X(sync                , "sync")
X(tag                 , "tag")
X(this_               , "this")
X(these_              , "these")
X(true_               , "true")
X(type                , "type")
X(tuple_              , "_tuple")
X(uint_               , "uint")
X(unmanaged           , "unmanaged")
X(void_               , "void")
X(range               , "range")

X(atMark              , "@")
X(equals              , "=")
X(question            , "?")
X(tripleDot           , "...")

X(andEquals           , "&=")
X(divEquals           , "/=")
X(epxEquals           , "**=")
X(lshiftEquals        , "<<=")
X(minusEquals         , "-=")
X(modEquals           , "%=")
X(orEquals            , "|=")
X(plusEquals          , "+=")
X(rshiftEquals        , ">>=")
X(timesEquals         , "*=")
X(xorEquals           , "^=")

X(logicAndAssign      , "&&=")
X(logicOrAssign       , "||=")

X(equalsGreater       , "=>")

X(bitwiseAnd          , "&")
X(bitwiseOr           , "|")
X(bitwiseXor          , "^")
X(logicalAnd          , "&&")
X(logicalOr           , "||")
X(plus                , "+")
X(minus               , "-")
X(times               , "*")
X(compareEqual        , "==")
X(notEqual            , "!=")
X(greaterOrEqual      , ">=")
X(greaterThan         , ">")
X(lessThan            , "<")
X(lessOrEqual         , "<=")
X(underscore          , "_")
X(postfixBang         , "postfix!")
X(logicalNot          , "!")
X(colon               , ":")
X(exponentiation      , "**")
X(bitwiseNot          , "~")
X(divide              , "/")
X(modulus             , "%")
X(leftShift           , "<<")
X(rightShift          , ">>")
X(rangeCount          , "#")

X(swap                , "<=>")
X(chplBy              , "chpl_by")
X(chplAlign           , "chpl_align")
X(chpldocDot          , "chpldoc.")
X(chplcheckDot        , "chplcheck.")
X(llvmDot             , "llvm.")
X(llvmMetadata        , "llvm.metadata")
X(llvmAssertVectorized, "llvm.assertVectorized")
X(deprecated          , "deprecated")
X(unstable            , "unstable")

/* A string too long is checked at compile time */
/* X(somethingtoolong      , "somethingtoolongforthemacro") */
