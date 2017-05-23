/*
 * Copyright 2004-2017 Cray Inc.
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

#include "candidateDisposition.h"

const char* getDispositionDescription(CandidateDisposition d)
{
  switch (d) {
    case AcceptCandidate: return "Accept";
    case RejectCandidateAlignmentMismatch:
      return "mismatch in number of arguments or named argument not found";
    case RejectCandidateThisTypeMismatch:
      return "receiver type did not match";
    case RejectCandidateArgTypeMismatch:
      return "argument type did not match";
    case RejectCandidateWhere:
      return "where clause did not evaluate to true";
    case RejectCandidateTypeNonTypeMismatch:
      return "type formal but not a type actual or vice versa";
    case RejectCandidateParamFormalNotActual:
      return "param formal but not a param actual";
  }
  return "<unknown>";
}

