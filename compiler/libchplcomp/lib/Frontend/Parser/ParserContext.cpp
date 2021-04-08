/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

std::vector<ParserComment>* ParserContext::gatherComments(YYLTYPE location) {
  std::vector<ParserComment>* ret = this->comments;
  this->comments = new std::vector<ParserComment>();

  // Drop any comments from 'ret' that occur after the passed location
  // so that things like the docs comment below are ignored
  //    proc f /* docs comment */ (arg) 
  while (ret->size() > 0) {
    ParserComment comment = ret->back();
    if (comment.location.first_line <= location.first_line &&
        comment.location.first_column <= location.first_column) {
      // OK, we can keep this comment (and any earlier ones)
      break;
    } else {
      // remove this comment
      delete comment.comment.allocatedData;
      ret->pop_back();
    }
  }

  return ret;
}

void ParserContext::noteComment(YYLTYPE loc, const char* data, long size) {
  ParserComment c;
  c.location = loc;
  c.comment.allocatedData = data;
  c.comment.size = size;
  this->comments->push_back(c);
}

void ParserContext::clearComments() {
  for (ParserComment parserComment : *this->comments) {
    delete parserComment.comment.allocatedData;
  }
  this->comments->clear();
}

ExprList* ParserContext::makeList() {
  return new ExprList();
}
ExprList* ParserContext::makeList(ExprList* lst) {
  return lst;
}
ExprList* ParserContext::makeList(Expr* e) {
  ExprList* ret = new ExprList();
  ret->push_back(e);
  return ret;
}
ExprList* ParserContext::makeList(CommentsAndStmt cs) {
  ExprList* ret = new ExprList();
  this->appendList(ret, cs);
  return ret;
}

void ParserContext::appendList(ExprList* dst, ExprList* lst) {
  for (Expr* elt : *lst) {
    dst->push_back(elt);
  }
  delete lst;
}

void ParserContext::appendList(ExprList* dst, Expr* e) {
  dst->push_back(e);
}

void ParserContext::appendList(ExprList* dst, CommentsAndStmt cs) {
  if (cs.comments != nullptr) {
    for (ParserComment parserComment : *cs.comments) {
      Comment* c = Comment::build(builder,
                                  parserComment.comment.allocatedData,
                                  parserComment.comment.size);
      dst->push_back(c);
    }
  }
  if (cs.stmt != nullptr) {
    dst->push_back(cs.stmt);
  }
  if (cs.comments != nullptr) {
    delete cs.comments;
  }
}

void ParserContext::appendComments(CommentsAndStmt*cs,
                                   std::vector<ParserComment>* comments) {
  if (cs->comments == nullptr) {
    cs->comments = comments;
    return;
  }

  // otherwise, append them and then delete comments
  for (ParserComment parserComment : *comments) {
    cs->comments->push_back(parserComment);
  }

  delete comments;
}

CommentsAndStmt ParserContext::finishStmt(CommentsAndStmt cs) {
  this->clearComments();
  return cs;
}
CommentsAndStmt ParserContext::finishStmt(Expr* e) {
  this->clearComments();
  return makeCommentsAndStmt(NULL, e);
}
