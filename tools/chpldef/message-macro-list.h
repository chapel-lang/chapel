/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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
CHPLDEF_MESSAGE(Shutdown, 0, 0, shutdown)
CHPLDEF_MESSAGE(Exit, 0, 1, exit)

/** The document's content is now managed by the client and the server
    MUST NOT try to read the document's content using the document's
    URI. Open in this sense means that it is managed by the client. An
    open notification must not be sent more than once without a close
    notification having been sent before. */
CHPLDEF_MESSAGE(DidOpen, 0, 1, textDocument/didOpen)
CHPLDEF_MESSAGE(DidChange, 0, 1, textDocument/didChange)
CHPLDEF_MESSAGE(DidSave, 0, 1, textDocument/didSave)
CHPLDEF_MESSAGE(DidClose, 0, 1, textDocument/didClose)

CHPLDEF_MESSAGE(Declaration, 0, 0, textDocument/declaration)
CHPLDEF_MESSAGE(Definition, 0, 0, textDocument/definition)

/*
CHPLDEF_MESSAGE(RegisterCapability, client/registerCapability)
CHPLDEF_MESSAGE(UnregisterCapability, client/unregisterCapability)
CHPLDEF_MESSAGE(SetTrace, $/setTrace)
CHPLDEF_MESSAGE(LogTrace, $/logTrace)
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
CHPLDEF_MESSAGE(WillSaveWaitUntil, textDocument/willSaveWaitUntil)
CHPLDEF_MESSAGE(WillSave, textDocument/willSave)
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
