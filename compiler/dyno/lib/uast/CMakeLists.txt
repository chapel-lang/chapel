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

target_sources(libdyno-obj
               PRIVATE

               AggregateDecl.cpp
               Array.cpp
               AstList.cpp
               AstNode.cpp
               AstTag.cpp
               As.cpp
               Attributes.cpp
               Begin.cpp
               Block.cpp
               BoolLiteral.cpp
               BracketLoop.cpp
               Break.cpp
               Builder.cpp
               BuilderResult.cpp
               BytesLiteral.cpp
               Call.cpp
               Catch.cpp
               chpl-syntax-printer.cpp
               Class.cpp
               Cobegin.cpp
               Coforall.cpp
               Comment.cpp
               Conditional.cpp
               Continue.cpp
               CStringLiteral.cpp
               Decl.cpp
               Defer.cpp
               Delete.cpp
               Domain.cpp
               Dot.cpp
               DoWhile.cpp
               EmptyStmt.cpp
               Enum.cpp
               EnumElement.cpp
               ErroneousExpression.cpp
               ExternBlock.cpp
               FnCall.cpp
               For.cpp
               Forall.cpp
               Foreach.cpp
               Formal.cpp
               ForwardingDecl.cpp
               Function.cpp
               Identifier.cpp
               ImagLiteral.cpp
               Import.cpp
               Include.cpp
               IndexableLoop.cpp
               IntLiteral.cpp
               Label.cpp
               Let.cpp
               Literal.cpp
               Local.cpp
               Loop.cpp
               Manage.cpp
               Module.cpp
               MultiDecl.cpp
               NamedDecl.cpp
               New.cpp
               NumericLiteral.cpp
               On.cpp
               OpCall.cpp
               Pragma.cpp
               PrimCall.cpp
               PrimOp.cpp
               Range.cpp
               RealLiteral.cpp
               Record.cpp
               Reduce.cpp
               Require.cpp
               Return.cpp
               Scan.cpp
               Select.cpp
               Serial.cpp
               SimpleBlockLike.cpp
               StringLikeLiteral.cpp
               StringLiteral.cpp
               Sync.cpp
               TaskVar.cpp
               Throw.cpp
               Try.cpp
               Tuple.cpp
               TupleDecl.cpp
               TypeDecl.cpp
               TypeQuery.cpp
               UintLiteral.cpp
               Use.cpp
               Union.cpp
               VarArgFormal.cpp
               Variable.cpp
               VarLikeDecl.cpp
               VisibilityClause.cpp
               When.cpp
               While.cpp
               WithClause.cpp
               Yield.cpp
               Zip.cpp

              )
