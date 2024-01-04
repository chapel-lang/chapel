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

#include "events.h"
#include "Server.h"
#include "chpl/parsing/parsing-queries.h"
#include <sstream>

namespace chpldef {

void Format::writeit(const chpl::uast::AstNode& t) {
  ctx_->withChapel([&](auto chapel) {
    ss_ << chpl::uast::asttags::tagToString(t.tag()) << " ";
    if (auto nd = t.toNamedDecl()) ss_ << "'" << nd->name().c_str() << "' ";
    auto loc = chpl::parsing::locateAst(chapel, &t);
    ss_ << "[" << loc.path().c_str() << ":" << loc.firstLine();
    ss_ << ":" << loc.firstColumn() << "]";
  });
}

void Format::writeit(const chpl::Location& t) {
  ss_ << t.path().c_str() << ":" << t.firstLine() << ":" << t.firstColumn();
  ss_ << "-" << t.lastLine() << ":" << t.lastColumn();
}

void Format::writeit(const chpldef::Message& msg) {
  ss_ << Message::tagToString(msg.tag());
  if (!msg.isNotification()) ss_ << " (" << msg.idToString() << ")";
}

} // end namespace 'chpldef'
