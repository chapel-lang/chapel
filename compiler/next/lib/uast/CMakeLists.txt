# Copyright 2021 Hewlett Packard Enterprise Development LP
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

target_sources(libchplcomp-obj
               PRIVATE

               ASTList.cpp
               ASTNode.cpp
               ASTTag.cpp
               Block.cpp
               BracketLoop.cpp
               Builder.cpp
               Call.cpp
               Coforall.cpp
               Comment.cpp
               Conditional.cpp
               Decl.cpp
               Defer.cpp
               Delete.cpp
               Dot.cpp
               DoWhile.cpp
               Enum.cpp
               EnumElement.cpp
               ErroneousExpression.cpp
               Expression.cpp
               FnCall.cpp
               For.cpp
               Forall.cpp
               Foreach.cpp
               Formal.cpp
               Function.cpp
               Identifier.cpp
               IndexableLoop.cpp
               Local.cpp
               Loop.cpp
               Module.cpp
               MultiDecl.cpp
               NamedDecl.cpp
               New.cpp
               OpCall.cpp
               Return.cpp
               Serial.cpp
               SimpleBlockLike.cpp
               TaskVar.cpp
               TupleDecl.cpp
               TypeDecl.cpp
               Variable.cpp
               VarLikeDecl.cpp
               Visitor.cpp
               While.cpp
               WithClause.cpp
               Yield.cpp
               Zip.cpp

              )
