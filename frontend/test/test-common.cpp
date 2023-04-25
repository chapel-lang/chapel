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

#include "test-common.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/uast/post-parse-checks.h"

using namespace chpl;

const uast::BuilderResult&
parseAndReportErrors(Context* context, UniqueString path) {
  auto& ret = parsing::parseFileToBuilderResultAndCheck(context, path, {});
  return ret;
}

uast::BuilderResult
parseStringAndReportErrors(parsing::Parser* parser, const char* filename,
                           const char* content) {
  auto path = UniqueString::get(parser->context(), filename);
  auto result = parser->parseString(filename, content);
  uast::checkBuilderResult(parser->context(), path, result);
  return result;
}

struct NamedCollector {
  std::vector<const uast::AstNode*> nodes;
  std::string name_;
  public:
    NamedCollector(std::string name) : name_(name) { }

    bool enter(const uast::AstNode* node) {
      return true;
    }
    void exit(const uast::AstNode* node) { }

    bool enter(const uast::NamedDecl* node) {
      if (node->name() == name_) {
        nodes.push_back(node);
      }
      return true;
    }
    void exit(const uast::NamedDecl* node) {
    }

    const uast::AstNode* only() {
      if (nodes.size() == 1) {
        return nodes.front();
      } else {
        return nullptr;
      }
    }
};

const uast::AstNode* findOnlyNamed(const uast::Module* mod, std::string name) {
  NamedCollector col(name);
  mod->traverse(col);
  return col.only();
}
