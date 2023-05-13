/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

//
// (name__, outbound__, notification__, rpc__)
//

//
// LIFECYCLE
//

CHPLDEF_MESSAGE(Initialize, 0, 0, initialize)
CHPLDEF_MESSAGE(Initialized, 0, 1, initialized)

/*
CHPLDEF_MESSAGE(RegisterCapability, client/registerCapability)
CHPLDEF_MESSAGE(UnregisterCapability, client/unregisterCapability)
CHPLDEF_MESSAGE(SetTrace, $/setTrace)
CHPLDEF_MESSAGE(LogTrace, $/logTrace)
CHPLDEF_MESSAGE(Shutdown, 0, 0, shutdown)
CHPLDEF_MESSAGE(Exit, exit)
*/

//
// MISC
//

/*
CHPLDEF_MESSAGE(CancelRequest, $/cancelRequest)
CHPLDEF_MESSAGE(Progress, $/progress)
*/

//
// SYNCHRONIZATION
//

/*
CHPLDEF_MESSAGE(DidOpen, textDocument/didOpen)
CHPLDEF_MESSAGE(DidChange, textDocument/didChange)
CHPLDEF_MESSAGE(WillSave, textDocument/willSave)
CHPLDEF_MESSAGE(WillSaveWaitUntil, textDocument/willSaveWaitUntil)
CHPLDEF_MESSAGE(DidSave, textDocument/didSave)
CHPLDEF_MESSAGE(DidClose, textDocument/didClose)
*/

//
// NOTEBOOK SYNCHRONIZATION
//

/*
CHPLDEF_MESSAGE(NotebookDidOpen, notebookDocument/didOpen)
CHPLDEF_MESSAGE(NotebookDidChange, notebookDocument/didChange)
CHPLDEF_MESSAGE(NotebookDidSave, notebookDocument/didSave)
CHPLDEF_MESSAGE(NotebookDidClose, notebookDocument/didClose)
*/

//
// LANGUAGE FEATURES
//

/*
CHPLDEF_MESSAGE(GotoDeclaration, textDocument/declaration)
CHPLDEF_MESSAGE(GotoDefinition, textDocument/definition)
CHPLDEF_MESSAGE(GotoTypeDefinition, textDocument/typeDefinition)
CHPLDEF_MESSAGE(GotoImplementation, textDocument/implementation)
CHPLDEF_MESSAGE(FindReferences, textDocument/references)
CHPLDEF_MESSAGE(PrepareCallHierarchy, textDocument/prepareCallHierarchy)
CHPLDEF_MESSAGE(CallHierarchyIncoming, callHierarchy/incomingCalls)
CHPLDEF_MESSAGE(CallHierarchyOutgoing, callHierarchy/outgoingCalls)
*/

//
// TODO...
// https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#languageFeatures
//
