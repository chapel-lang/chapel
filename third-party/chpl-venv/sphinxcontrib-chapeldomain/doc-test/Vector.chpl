/*
 * Copyright 2004-2015 Cray Inc.
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

class Vector {
    type eltType;
    var capacity : int,
        lastIdx  : int,
        dom      : domain(1),
        elements : [dom] eltType;
//    param MIN_SIZE : uint = 4;

    
    proc Vector(type eltType, cap=4, offset=0) {
        capacity = cap;
        lastIdx  = offset-1;
        dom = {offset..offset+capacity-1};
    }

    proc push(val : eltType) {
        const firstIdx = this.low;

        lastIdx += 1;
        if lastIdx >= firstIdx + capacity {
            capacity = max(1, capacity*2);
            dom = {firstIdx..#capacity};
        }
        elements[lastIdx] = val;
    }

    proc low const {
        return dom.dim(1).low;
    }

    proc high const {
        return lastIdx;
    }

    proc pop() {
        const firstIdx = this.low;
        var val = elements[lastIdx];

        // set element to default value so if its an object it can be
        // garbage collected
        var default:eltType;
        elements[lastIdx] = default;
        lastIdx -= 1;

        // if size is less than quarter of capacity cut the capacity down
        if(size < capacity/4) {
            capacity = max(4, capacity/2);
            dom = {firstIdx..#capacity};
        }
        return val;
    }

    proc top ref {
        return elements[lastIdx];
    }

    proc this(idx) ref {
        const firstIdx = this.low;
        
        assert(firstIdx <= idx && idx <= lastIdx,
            "Vector index out-of-bounds");
        return elements[idx];
    }

    iter these() ref {
        const firstIdx = this.low;

        for idx in firstIdx..lastIdx {
            yield elements[idx];
        }
    }

    proc size const {
        const firstIdx = this.low;
        return lastIdx - firstIdx + 1;
    }

    proc empty const {
        const firstIdx = this.low;
        return lastIdx < firstIdx;
    }
    
    // other functions we might want to add:
    //      proc append(rhs);
    //      proc clear()
}

