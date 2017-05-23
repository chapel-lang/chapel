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

#ifndef _CANDIDATE_DISPOSITION_H_
#define _CANDIDATE_DISPOSITION_H_

typedef enum {
  AcceptCandidate,
  RejectCandidateAlignmentMismatch,    // # args / named arg mismatch
  RejectCandidateThisTypeMismatch,    // this type mismatch
  RejectCandidateArgTypeMismatch,     // argument type mismatch
  RejectCandidateWhere,               // where clause didn't evaluate to true
  RejectCandidateTypeNonTypeMismatch, // one is a type, other is not 
  RejectCandidateParamFormalNotActual,// param formal, non-param actual
} CandidateDisposition;

const char* getDispositionDescription(CandidateDisposition d);

#endif
