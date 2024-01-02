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

#include "./TestClient.h"
#include <sstream>

void TestClient::breakpoint() {}

Server TestClient::createServerInstance() {
  Server::Configuration conf;
  conf.transport = nullptr;
  return { std::move(conf) };
}

chpl::Context TestClient::createChapelCompilerInstance() {
  chpl::Context::Configuration conf;
  return { std::move(conf) };
}

int64_t TestClient::bumpVersionForUri(const std::string& uri) {
  auto it = uriToVersion_.find(uri);
  if (it != uriToVersion_.end()) return it->second++;
  uriToVersion_[uri] = 0;
  return 0;
}

JsonValue TestClient::createUniqueMessageId() {
  return messageIdCounter_++;
}

opt<InitializeResult> TestClient::sendInitialize() {
  Initialize::Params p;
  auto msg = Initialize::create(createUniqueMessageId(), std::move(p));
  msg->handle(ctx_);
  if (auto r = msg->result()) return *r;
  return {};
}

void TestClient::sendInitialized() {
  Initialized::Params p;
  auto msg = Initialized::create(createUniqueMessageId(), std::move(p));
  msg->handle(ctx_);
}

void TestClient::sendExit() {
  CHPLDEF_TODO();
}

void TestClient::sendShutdown() {
  CHPLDEF_TODO();
}

void TestClient::advanceServerToReady() {
  if (ctx_->state() == Server::UNINITIALIZED) {
    std::ignore = sendInitialize();
    assert(ctx_->state() == Server::SETUP);
  }

  if (ctx_->state() == Server::SETUP) {
    sendInitialized();
    assert(ctx_->state() == Server::READY);
  }
}

void TestClient::sendDidOpen(const std::string& uri,
                             const std::string& text) {
  auto ver = bumpVersionForUri(uri);
  TextDocumentItem tdi { uri, "chapel", ver, text };
  DidOpen::Params p { std::move(tdi) };
  auto msg = DidOpen::create(nullptr, std::move(p));
  msg->handle(ctx_);
}

static std::string mentionSymbol(const chpl::uast::AstNode* ast) {
  if (auto x = ast->toIdentifier()) return x->name().c_str();
  if (auto x = ast->toDot()) return x->field().c_str();
  CHPLDEF_TODO();
  return {};
}

static Location
mentionSourceLocation(chpl::Context* chapel, const chpl::uast::AstNode* ast) {
  assert(ast);
  if (auto ident = ast->toIdentifier()) {
    auto loc = chpl::parsing::locateAst(chapel, ident);
    return Location(std::move(loc));
  } else if (auto dot = ast->toDot()) {
    auto loc = chpl::parsing::locateDotFieldWithAst(chapel, dot);
    assert(loc);
    return Location(std::move(loc));
  } else {
    CHPLDEF_TODO();
  }
  return {};
}

static Location
mentionTargetLocation(chpl::Context* chapel, const chpl::uast::AstNode* ast) {
  assert(ast);
  auto loc = chpl::parsing::locateAst(chapel, ast);
  return Location(std::move(loc));
}

static void
doCollectMentions(chpl::Context* chapel,
                  const chpl::uast::AstNode* ast,
                  std::vector<const chpl::uast::NamedDecl*>& decls,
                  std::vector<TestClient::Mention>& mentions) {
  if (auto decl = ast->toNamedDecl()) {
    decls.push_back(decl);
  } else if (ast->isIdentifier() || ast->isDot()) {
    TestClient::Mention m;
    m.tag = ast->tag();
    m.symbol = mentionSymbol(ast);
    bool isBaseExpr = parentCallIfBaseExpression(chapel, ast) != nullptr;
    m.isCallBaseExpression = isBaseExpr;
    m.source = mentionSourceLocation(chapel, ast);
    mentions.push_back(std::move(m));
  }

  for (auto child : ast->children()) {
    doCollectMentions(chapel, child, decls, mentions);
  }
}

static void
doFixupMentionTargets(chpl::Context* chapel,
                      std::vector<const chpl::uast::NamedDecl*>& decls,
                      std::vector<TestClient::Mention>& mentions) {
  std::map<std::string, std::vector<const chpl::uast::NamedDecl*>> nameToDecls;
  std::map<std::string, size_t> functionMentionIdx;

  for (auto& nd : decls) {
    auto name = nd->name().c_str();
    auto& v = nameToDecls[name];
    v.push_back(nd);
    if (!nd->isFunction()) assert(v.size() == 1);
  }

  for (size_t i = 0; i < mentions.size(); i++) {
    auto& m = mentions[i];
    auto& name = m.symbol;

    // Get the declaration(s) with the name 'name'.
    auto it = nameToDecls.find(name);
    if (it == nameToDecls.end()) {
      m.isValid = false;
      continue;
    }

    auto& v = it->second;
    const chpl::uast::NamedDecl* nd = nullptr;

    // Calls map linearly in order to simulate overloading.
    if (m.isCallBaseExpression) {
      auto& idx = functionMentionIdx[name];
      assert(v.size() >= 1);
      assert(idx < v.size());
      nd = v[idx++];

    // Everything else just maps by name.
    } else {
      assert(v.size() == 1);
      nd = v[0];
    }

    assert(nd);
    m.target = mentionTargetLocation(chapel, nd);
  }
}

std::vector<TestClient::Mention>
TestClient::collectMentions(const std::string& uri,
                            const std::string& text) {
  auto chapelContext = createChapelCompilerInstance();
  auto chapel = &chapelContext;
  std::vector<TestClient::Mention> ret;
  std::vector<const chpl::uast::NamedDecl*> decls;

  // Set the file text and parse the file.
  auto ustr = chpl::UniqueString::get(chapel, uri);
  chpl::parsing::setFileText(chapel, ustr, text);
  auto& br = parseFromUri(chapel, uri);

  for (auto ast : br.topLevelExpressions()) {
    doCollectMentions(chapel, ast, decls, ret);
  }

  // Fixing up the targets allows tests to write decls out of order.
  doFixupMentionTargets(chapel, decls, ret);

  return ret;
}

std::vector<uint64_t>
TestClient::collectLineLengthsInSource(const std::string& text) {
  std::vector<uint64_t> ret;
  uint64_t character = 0;
  for (auto c : text) {
    if (c == '\n') {
      ret.push_back(character);
      character = 0;
    } else {
      character += 1;
    }
  }
  return ret;
}

opt<Location>
TestClient::sendDeclaration(const std::string& uri, Position cursor) {
  TextDocumentIdentifier tdi(uri);
  Declaration::Params p;
  p.textDocument = std::move(tdi);
  p.position = std::move(cursor);

  // Create and handle the message directly rather than enqueue it for
  // processing by the server main loop.
  auto msg = Declaration::create(createUniqueMessageId(), std::move(p));
  msg->handle(ctx_);

  // TODO: This is absurd, we need to flatten 'DeclarationResult'.
  if (auto r = msg->result()) {
    if (auto& res = r->result) {
      if (auto p = std::get_if<LocationArray>(&(*res))) {
        auto& arr = *p;
        assert(arr.size() <= 1);
        if (arr.size() == 1) return arr[0];
        return {};
      }
    }
  }
  return {};
}

std::string TestClient::Mention::toString() const {
  std::stringstream ss;
  ss << chpl::uast::asttags::tagToString(tag) << " " << symbol << " (";
  ss << source.range.start.line << ":" << source.range.start.character;
  ss << "-";
  ss << source.range.end.line << ":" << source.range.end.character;
  ss << " -> ";
  ss << target.range.start.line << ":" << target.range.start.character;
  ss << "-";
  ss << target.range.end.line << ":" << target.range.end.character;
  ss << ")";
  return ss.str();
}
