/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

CHPL_LOCATION_MAP(Dot, DotField)
CHPL_LOCATION_MAP(NamedDecl, DeclName)
// CHPL_LOCATION_MAP(AstNode, NamedActual)
// CHPL_LOCATION_MAP(AstNode, AttributeNamedActual)
// CHPL_LOCATION_MAP(Function, ReturnIntent)
// CHPL_LOCATION_MAP(Function, ReceiverIntent)
// CHPL_LOCATION_MAP(AstNode, FormalIntent)
