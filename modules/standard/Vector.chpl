class Vector {
    type eltType;
    var capacity : int,
        lastIdx  : int,
        dom      : domain(1),
        elements : [dom] eltType;
    //param MIN_SIZE : uint = 4;

    
    def Vector(type eltType, cap=4, offset=0) {
        capacity = cap;
        lastIdx  = offset-1;
        dom = [offset..offset+capacity-1];
    }

    def push(val : eltType) {
        const firstIdx = this.low;

        lastIdx += 1;
        if lastIdx >= firstIdx + capacity {
            capacity = max(1, capacity*2);
            dom = [firstIdx..#capacity];
        }
        elements[lastIdx] = val;
    }

    def low const {
        return dom.dim(1).low;
    }

    def high const {
        return lastIdx;
    }

    def pop() {
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
            dom = [firstIdx..#capacity];
        }
        return val;
    }

    def top var {
        return elements[lastIdx];
    }

    def this(idx) var {
        const firstIdx = this.low;
        
        assert(firstIdx <= idx && idx <= lastIdx,
            "Vector index out-of-bounds");
        return elements[idx];
    }

    def these {
        for v in elements {
            yield v;
        }
    }

    def size const {
        const firstIdx = this.low;
        return lastIdx - firstIdx + 1;
    }

    def empty const {
        const firstIdx = this.low;
        return lastIdx < firstIdx;
    }
    
    // other functions we might want to add:
    //      def append(rhs);
    //      def clear()
}

