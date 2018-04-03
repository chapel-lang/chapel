/*
 * Copyright 2004-2018 Cray Inc.
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

module Dataframes {
  class Index {
    type idxType;
    var set: domain(idxType);
    var map: [set] int;

    proc init(idx: [?D] int) {
      super.init();
      idxType = D.idxType;
      set = D;
      map = idx;
    }
  }

  class Series {
  }

  class TypedSeries : Series {
    type eltType;
    type idxType;

    var ordSet: domain(int);
    var data: [ordSet] eltType;
    var idx: Index(idxType);

    proc init(data: [?D] ?T) {
      super.init();
      eltType = T;
      idxType = int;

      ordSet = D;
      this.data = data;
    }

    proc init(data: [?D] ?T, idx: [?I] int) {
      // TODO: verify (i in idx) iff (i in D)
      super.init();
      eltType = T;
      idxType = I.idxType;

      ordSet = D;
      this.data = data;
      this.idx = new Index(idx);
    }

    proc writeThis(f) {
      if idx {
        f <~> "index: ";
        idx.writeThis(f);
        f <~> " | ";
      }
      f <~> "data: ";
      data.writeThis(f);
    }
  }
}

