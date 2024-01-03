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

#include "Message.h"
#include "Server.h"
#include "chpl/parsing/parsing-queries.h"

namespace chpldef {

template<>
DidOpen::ComputeResult DidOpen::compute(Server* ctx, ComputeParams p) {
  auto& tdi = p.textDocument;
  auto& e = ctx->mutableTextRegistry()[tdi.uri];

  if (e.isOpen) {
    CHPLDEF_TODO();
    return {};
  }

  CHPL_ASSERT(tdi.version > e.version);

  // NOTE: I think we always have to bump the revision here. This is
  // because this file may have been implicitly parsed from disk as
  // as result of resolving a use/import. The contents are considered
  // to have changed and the "truth of the file's contents" are determined
  // by the client as long as it has the file open. Cannot implicitly
  // read from disk, so have to bump the revision to ensure correctness.
  ctx->withChapel(Server::CHPL_BUMP_REVISION, [&](auto chapel) {
    chpl::parsing::setFileText(chapel, tdi.uri, tdi.text);
    auto& fc = chpl::parsing::fileText(chapel, tdi.uri);
    CHPL_ASSERT(!fc.error());
    CHPL_ASSERT(fc.text() == tdi.text);
    e.version = tdi.version;
    e.lastRevisionContentsUpdated = ctx->revision();
    e.isOpen = true;
  });

  return {};
}

template<>
DidChange::ComputeResult DidChange::compute(Server* ctx, ComputeParams p) {
  CHPLDEF_TODO();
  return {};
}


template<>
DidSave::ComputeResult DidSave::compute(Server* ctx, ComputeParams p) {
  CHPLDEF_TODO();
  return {};
}

template<>
DidClose::ComputeResult DidClose::compute(Server* ctx, ComputeParams p) {
  CHPLDEF_TODO();
  return {};
}

} // end namespace 'chpldef'
