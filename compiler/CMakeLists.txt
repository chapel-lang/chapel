# Copyright 2021-2022 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.



# request C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# request C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_VERBOSE_MAKEFILE 1)

find_package(Python)


option(CHPL_LLVM_COMP_ARGS "Override compile args for LLVM")
option(CHPL_LLVM_LINK_ARGS "Override linker args for LLVM")
# remove whitespace or you'll get complaints about an error
# according to policy CMP0004
string(STRIP ${CHPL_LLVM_COMP_ARGS} CHPL_LLVM_COMP_ARGS)
string(STRIP ${CHPL_LLVM_LINK_ARGS} CHPL_LLVM_LINK_ARGS)

set(SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR})
# need many of the outputs from printchplenv for the following values
include_directories(
        ${SRC_DIR}/include/linux64 ${SRC_DIR}/include ${SRC_DIR}/../build/compiler/lin
        ux64/gnu/x86_64/hostmem-cstdlib/llvm-system/14/san-none
        ${SRC_DIR}/../runtime/include/encoding
        /home/arezaii/git/chapel/compiler/../third-party/utf8-decoder
        ${SRC_DIR}/dyno/include)

# set(DYNO_LIB_IMMEDIATES_SOURCES
#   dyno/lib/immediates/ifa_vars.cpp dyno/lib/immediates/hash_multipliers.cpp
#   dyno/lib/immediates/num.cpp dyno/lib/immediates/num.h
#   dyno/lib/immediates/prim_data.h)

# set(DYNO_LIB_RESOLUTION_SOURCES
#   dyno/lib/resolution/can-pass.cpp dyno/lib/resolution/disambiguation.cpp
#   dyno/lib/resolution/intents.cpp dyno/lib/resolution/resolution-queries.cpp
#   dyno/lib/resolution/resolution-types.cpp dyno/lib/resolution/scope-queries.cpp
#   dyno/lib/resolution/scope-types.cpp)

# set(DYNO_LIB_UTIL_SOURCES
#   dyno/lib/util/my_aligned_alloc.h dyno/lib/util/my_aligned_alloc.c
#   dyno/lib/util/my_strerror_r.h dyno/lib/util/my_strerror_r.c
#   dyno/lib/util/bitmap.cpp dyno/lib/util/break.cpp dyno/lib/util/filesystem.cpp
#   dyno/lib/util/filesystem_help.h dyno/lib/util/string-escapes.cpp
#   dyno/lib/util/subprocess.cpp)

# set(DYNO_LIB_TYPES_SOURCES
#   dyno/lib/types/AnyType.cpp dyno/lib/types/BasicClassType.cpp
#   dyno/lib/types/BoolType.cpp dyno/lib/types/BuiltinType.cpp
#    dyno/lib/types/CStringType.cpp
#   dyno/lib/types/ClassType.cpp dyno/lib/types/ClassTypeDecorator.cpp
#   dyno/lib/types/ComplexType.cpp dyno/lib/types/CompositeType.cpp
#   dyno/lib/types/ErroneousType.cpp dyno/lib/types/ImagType.cpp
#   dyno/lib/types/IntType.cpp dyno/lib/types/NilType.cpp
#   dyno/lib/types/NothingType.cpp dyno/lib/types/Param.cpp
#   dyno/lib/types/PrimitiveType.cpp dyno/lib/types/QualifiedType.cpp
#   dyno/lib/types/RealType.cpp dyno/lib/types/RecordType.cpp
#    dyno/lib/types/TupleType.cpp
#   dyno/lib/types/Type.cpp dyno/lib/types/TypeTag.cpp dyno/lib/types/UintType.cpp
#   dyno/lib/types/UnionType.cpp dyno/lib/types/UnknownType.cpp
#   dyno/lib/types/VoidType.cpp)

# set(DYNO_LIB_PARSING_SOURCES
#   dyno/lib/parsing/bison-chpl-lib.cpp dyno/lib/parsing/bison-chpl-lib.h
#   dyno/lib/parsing/flex-chpl-lib.cpp dyno/lib/parsing/flex-chpl-lib.h
#   dyno/lib/parsing/ParserContext.h dyno/lib/parsing/ParserContextImpl.h
#   dyno/lib/parsing/lexer-help.h dyno/lib/parsing/parser-dependencies.h
#   dyno/lib/parsing/parser-help.h dyno/lib/parsing/Parser.cpp
#   dyno/lib/parsing/parsing-queries.cpp)

# set(DYNO_LIB_UAST_SOURCES
#   dyno/lib/uast/AggregateDecl.cpp dyno/lib/uast/Array.cpp
#   dyno/lib/uast/AstList.cpp dyno/lib/uast/AstNode.cpp dyno/lib/uast/AstTag.cpp
#   dyno/lib/uast/As.cpp dyno/lib/uast/Attributes.cpp dyno/lib/uast/Begin.cpp
#   dyno/lib/uast/Block.cpp dyno/lib/uast/BoolLiteral.cpp
#   dyno/lib/uast/BracketLoop.cpp dyno/lib/uast/Break.cpp
#   dyno/lib/uast/Builder.cpp dyno/lib/uast/BuilderResult.cpp
#   dyno/lib/uast/BytesLiteral.cpp dyno/lib/uast/Call.cpp dyno/lib/uast/Catch.cpp
#   dyno/lib/uast/chpl-syntax-printer.cpp dyno/lib/uast/Class.cpp
#   dyno/lib/uast/Cobegin.cpp dyno/lib/uast/Coforall.cpp dyno/lib/uast/Comment.cpp
#   dyno/lib/uast/Conditional.cpp dyno/lib/uast/Continue.cpp
#   dyno/lib/uast/CStringLiteral.cpp dyno/lib/uast/Decl.cpp
#   dyno/lib/uast/Defer.cpp dyno/lib/uast/Delete.cpp dyno/lib/uast/Domain.cpp
#   dyno/lib/uast/Dot.cpp dyno/lib/uast/DoWhile.cpp dyno/lib/uast/Enum.cpp dyno/lib/uast/EmptyStmt.cpp
#   dyno/lib/uast/EnumElement.cpp dyno/lib/uast/ErroneousExpression.cpp
#   dyno/lib/uast/ExternBlock.cpp
#   dyno/lib/uast/FnCall.cpp dyno/lib/uast/For.cpp dyno/lib/uast/Forall.cpp
#   dyno/lib/uast/Foreach.cpp dyno/lib/uast/Formal.cpp
#   dyno/lib/uast/ForwardingDecl.cpp dyno/lib/uast/Function.cpp
#   dyno/lib/uast/Identifier.cpp dyno/lib/uast/ImagLiteral.cpp
#   dyno/lib/uast/Import.cpp dyno/lib/uast/Include.cpp
#   dyno/lib/uast/IndexableLoop.cpp dyno/lib/uast/IntLiteral.cpp
#   dyno/lib/uast/Label.cpp dyno/lib/uast/Let.cpp dyno/lib/uast/Literal.cpp
#   dyno/lib/uast/Local.cpp dyno/lib/uast/Loop.cpp dyno/lib/uast/Manage.cpp
#   dyno/lib/uast/Module.cpp dyno/lib/uast/MultiDecl.cpp
#   dyno/lib/uast/NamedDecl.cpp dyno/lib/uast/New.cpp
#   dyno/lib/uast/NumericLiteral.cpp dyno/lib/uast/On.cpp dyno/lib/uast/OpCall.cpp
#   dyno/lib/uast/Pragma.cpp dyno/lib/uast/PrimCall.cpp dyno/lib/uast/PrimOp.cpp
#   dyno/lib/uast/Range.cpp dyno/lib/uast/RealLiteral.cpp dyno/lib/uast/Record.cpp
#   dyno/lib/uast/Reduce.cpp dyno/lib/uast/Require.cpp dyno/lib/uast/Return.cpp
#   dyno/lib/uast/Scan.cpp dyno/lib/uast/Select.cpp dyno/lib/uast/Serial.cpp
#   dyno/lib/uast/SimpleBlockLike.cpp dyno/lib/uast/StringLikeLiteral.cpp
#   dyno/lib/uast/StringLiteral.cpp dyno/lib/uast/Sync.cpp
#   dyno/lib/uast/TaskVar.cpp dyno/lib/uast/Throw.cpp dyno/lib/uast/Try.cpp
#   dyno/lib/uast/Tuple.cpp dyno/lib/uast/TupleDecl.cpp dyno/lib/uast/TypeDecl.cpp
#   dyno/lib/uast/TypeQuery.cpp dyno/lib/uast/UintLiteral.cpp
#   dyno/lib/uast/Use.cpp dyno/lib/uast/Union.cpp dyno/lib/uast/VarArgFormal.cpp
#   dyno/lib/uast/Variable.cpp dyno/lib/uast/VarLikeDecl.cpp
#   dyno/lib/uast/VisibilityClause.cpp dyno/lib/uast/When.cpp
#   dyno/lib/uast/While.cpp dyno/lib/uast/WithClause.cpp dyno/lib/uast/Yield.cpp
#   dyno/lib/uast/Zip.cpp)

# set(DYNO_LIB_QUERIES_SOURCES
#   dyno/lib/framework/Context.cpp dyno/lib/framework/ErrorMessage.cpp
#   dyno/lib/framework/ID.cpp dyno/lib/framework/Location.cpp
#   dyno/lib/framework/UniqueString.cpp)

# set(AST_SOURCES
#   AST/AggregateType.cpp AST/alist.cpp AST/astutil.cpp AST/baseAST.cpp AST/bb.cpp
#   AST/build.cpp AST/CatchStmt.cpp AST/CallExpr.cpp AST/checkAST.cpp
#   AST/DecoratedClassType.cpp AST/DeferStmt.cpp AST/dominator.cpp AST/expr.cpp
#   AST/foralls.cpp AST/flags.cpp AST/FnSymbol.cpp AST/ForallStmt.cpp
#   AST/IfExpr.cpp AST/ImportStmt.cpp AST/interfaces.cpp AST/iterator.cpp
#   AST/LoopExpr.cpp AST/ModuleSymbol.cpp AST/PartialCopyData.cpp
#   AST/primitive.cpp AST/stmt.cpp AST/symbol.cpp AST/TryStmt.cpp AST/type.cpp
#   AST/UseStmt.cpp AST/view.cpp AST/wellknown.cpp AST/LoopStmt.cpp
#   AST/WhileStmt.cpp AST/WhileDoStmt.cpp AST/DoWhileStmt.cpp AST/CForLoop.cpp
#   AST/ForLoop.cpp AST/ParamForLoop.cpp AST/AstVisitor.cpp
#   AST/AstVisitorTraverse.cpp AST/AstLogger.cpp AST/AstDump.cpp
#   AST/AstDumpToHtml.cpp AST/AstDumpToNode.cpp AST/AstCount.cpp
#   AST/AstPrintDocs.cpp AST/AstToText.cpp AST/CollapseBlocks.cpp
#   AST/TransformLogicalShortCircuit.cpp)

# set(MAIN_SOURCES
#   main/arg.cpp main/checks.cpp main/commonFlags.cpp main/config.cpp
#   main/docsDriver.cpp main/driver.cpp main/log.cpp main/runpasses.cpp
#   main/version.cpp main/PhaseTracker.cpp)

# set(LLVM_SOURCES
#   llvm/clangUtil.cpp llvm/llvmAggregateGlobalOps.cpp llvm/llvmDumpIR.cpp
#   llvm/llvmExtractIR.cpp llvm/llvmGlobalToWide.cpp llvm/llvmUtil.cpp
#   llvm/llvmDebug.cpp)

# set(RESOLUTION_SOURCES
#   resolution/addAutoDestroyCalls.cpp resolution/AutoDestroyScope.cpp
#   resolution/caches.cpp resolution/callDestructors.cpp resolution/callInfo.cpp
#   resolution/cleanups.cpp resolution/cullOverReferences.cpp
#   resolution/expandVarArgs.cpp resolution/fixupExports.cpp
#   resolution/functionResolution.cpp resolution/generics.cpp
#   resolution/implementForallIntents.cpp resolution/initializerResolution.cpp
#   resolution/interfaceResolution.cpp resolution/lateConstCheck.cpp
#   resolution/lifetime.cpp resolution/loopDetails.cpp resolution/lowerForalls.cpp
#   resolution/lowerIterators.cpp resolution/nilChecking.cpp
#   resolution/postFold.cpp resolution/preFold.cpp
#   resolution/ResolutionCandidate.cpp resolution/resolveFunction.cpp
#   resolution/tuples.cpp resolution/typeSpecifier.cpp
#   resolution/virtualDispatch.cpp resolution/visibleFunctions.cpp
#   resolution/wrappers.cpp)

# set(UTIL_SOURCES
#   util/astlocs.cpp util/exprAnalysis.cpp util/files.cpp util/misc.cpp
#         util/stringutil.cpp util/timer.cpp )

# set(CODEGEN_SOURCES
#   codegen/codegen.cpp codegen/cg-expr.cpp codegen/cg-CForLoop.cpp
#   codegen/cg-DoWhileStmt.cpp codegen/cg-LoopStmt.cpp codegen/cg-WhileDoStmt.cpp
#   codegen/cg-alist.cpp codegen/library.cpp codegen/mli.cpp codegen/cg-stmt.cpp
#   codegen/cg-symbol.cpp codegen/cg-type.cpp)

# set(PARSER_SOURCES
#   parser/parser.cpp)

# set(OPTIMIZATIONS_SOURCES
#   optimizations/bulkCopyRecords.cpp optimizations/copyPropagation.cpp
#   optimizations/deadCodeElimination.cpp optimizations/forallOptimizations.cpp
#   optimizations/gpuTransforms.cpp optimizations/inlineFunctions.cpp
#   optimizations/inferConstRefs.cpp optimizations/liveVariableAnalysis.cpp
#   optimizations/localizeGlobals.cpp optimizations/loopInvariantCodeMotion.cpp
#   optimizations/noAliasSets.cpp optimizations/optimizeForallUnorderedOps.cpp
#   optimizations/optimizeOnClauses.cpp optimizations/propagateDomainConstness.cpp
#   optimizations/refPropagation.cpp optimizations/remoteValueForwarding.cpp
#   optimizations/removeEmptyRecords.cpp
#   optimizations/removeUnnecessaryAutoCopyCalls.cpp
#   optimizations/removeUnnecessaryGotos.cpp
#   optimizations/replaceArrayAccessesWithRefTemps.cpp
#   optimizations/scalarReplace.cpp)

# set(PASSES_SOURCES
#   passes/addInitGuards.cpp passes/buildDefaultFunctions.cpp
#   passes/checkNormalized.cpp passes/checkParsed.cpp passes/checkResolved.cpp
#   passes/cleanup.cpp passes/convert-uast.cpp passes/createTaskFunctions.cpp
#   passes/denormalize.cpp passes/docs.cpp passes/errorHandling.cpp
#   passes/expandExternArrayCalls.cpp passes/externCResolve.cpp
#   passes/flattenClasses.cpp passes/flattenFunctions.cpp
#   passes/initializerRules.cpp passes/InitNormalize.cpp
#   passes/insertLineNumbers.cpp passes/insertWideReferences.cpp
#   passes/normalize.cpp passes/normalizeErrors.cpp passes/parallel.cpp
#   passes/resolveIntents.cpp passes/ResolveScope.cpp
#   passes/returnStarTuplesByRefArgs.cpp passes/scopeResolve.cpp
#   passes/splitInit.cpp)

# set(BACKEND_SOURCES
#   backend/beautify.cpp)

# set(ADT_SOURCES
#   adt/bitVec.cpp adt/vec.cpp)

# set(ALL_SOURCES
  # ${AST_SOURCES}
  # ${MAIN_SOURCES}
  # ${LLVM_SOURCES}
  # ${RESOLUTION_SOURCES}
  # ${UTIL_SOURCES}
  # ${CODEGEN_SOURCES}
  # ${PARSER_SOURCES}
  # ${OPTIMIZATIONS_SOURCES}
  # ${PASSES_SOURCES}
  # ${BACKEND_SOURCES}
  # ${ADT_SOURCES})

  # ${DYNO_LIB_IMMEDIATES_SOURCES} ${DYNO_LIB_RESOLUTION_SOURCES}
  # ${DYNO_LIB_UTIL_SOURCES} ${DYNO_LIB_TYPES_SOURCES} ${DYNO_LIB_PARSING_SOURCES}
  # ${DYNO_LIB_UAST_SOURCES} ${DYNO_LIB_QUERIES_SOURCES}

add_executable(chpl)
add_subdirectory(AST)
add_subdirectory(adt)
add_subdirectory(backend)
add_subdirectory(codegen)
add_subdirectory(dyno)
add_subdirectory(include)
add_subdirectory(llvm)
add_subdirectory(main)
add_subdirectory(optimizations)
add_subdirectory(parser)
add_subdirectory(passes)
add_subdirectory(resolution)
add_subdirectory(util)
add_dependencies(chpl libdyno)
target_compile_options(chpl PUBLIC SHELL:$<$<COMPILE_LANGUAGE:CXX>:${CHPL_LLVM_COMP_ARGS}>)
target_link_libraries(chpl PUBLIC ${CHPL_LLVM_LINK_ARGS} libdyno)
