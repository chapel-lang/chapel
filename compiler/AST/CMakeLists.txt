# Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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
set(SRCS
    AggregateType.cpp
    alist.cpp
    metadata.cpp
    AstCount.cpp
    AstDump.cpp
    AstDumpToHtml.cpp
    AstLogger.cpp
    AstToText.cpp
    astutil.cpp
    AstVisitor.cpp
    AstVisitorTraverse.cpp
    baseAST.cpp
    bb.cpp
    build.cpp
    CallExpr.cpp
    CatchStmt.cpp
    CForLoop.cpp
    checkAST.cpp
    CollapseBlocks.cpp
    DecoratedClassType.cpp
    DeferStmt.cpp
    dominator.cpp
    DoWhileStmt.cpp
    expr.cpp
    flags.cpp
    FnSymbol.cpp
    foralls.cpp
    ForallStmt.cpp
    ForLoop.cpp
    IfExpr.cpp
    ImportStmt.cpp
    interfaces.cpp
    iterator.cpp
    LoopExpr.cpp
    LoopStmt.cpp
    ModuleSymbol.cpp
    ParamForLoop.cpp
    PartialCopyData.cpp
    primitive.cpp
    stmt.cpp
    symbol.cpp
    TemporaryConversionThunk.cpp
    thunks.cpp
    TransformLogicalShortCircuit.cpp
    TryStmt.cpp
    type.cpp
    UseStmt.cpp
    view.cpp
    wellknown.cpp
    WhileDoStmt.cpp
    WhileStmt.cpp
   )
add_compiler_sources("${SRCS}" "${CMAKE_CURRENT_SOURCE_DIR}")
