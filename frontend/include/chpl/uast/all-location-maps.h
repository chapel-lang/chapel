/*
 * Copyright 2021-2026 Hewlett Packard Enterprise Development LP
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

/** (AST, LOCATION) **/
// The first actual is the AST type the location is associated with.
// The second actual is a string naming the location.
// The second actual must have a unique name.
//

LOCATION_MAP(Dot, DotField)
LOCATION_MAP(NamedDecl, DeclName)
LOCATION_MAP(Include, IncludeName)
LOCATION_MAP(Loop, LoopHeader)
LOCATION_MAP(AstNode, BlockHeader)
LOCATION_MAP(NamedDecl, DeclHeader)
LOCATION_MAP(AstNode, ExprParen)
LOCATION_MAP(AstNode, CurlyBraces)
LOCATION_MAP(Conditional, ThenKeyword)
LOCATION_MAP(Conditional, ElseKeyword)
// LOCATION_MAP(AstNode, NamedActual)
// LOCATION_MAP(AstNode, AttributeNamedActual)
// LOCATION_MAP(Function, ReturnIntent)
// LOCATION_MAP(Function, ReceiverIntent)
// LOCATION_MAP(AstNode, FormalIntent)
