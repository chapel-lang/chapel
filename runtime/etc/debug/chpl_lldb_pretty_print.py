#
# Copyright 2025-2025 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from functools import cache
import lldb
import re


class Ptr:
    def __init__(self, target, ptr, owned=True):
        self.target = target
        assert self.target is not None
        self.ptr = ptr
        self.owned = owned

    def get(self):
        if self.ptr is not None:
            return self.ptr
        else:
            # return an invalid SBValue
            return lldb.SBValue()


def debugp(*args, **kwargs):
    print("[chpl_lldb_pretty_print]", *args, **kwargs)


def errorp(*args, **kwargs):
    print("[chpl_lldb_pretty_print][ERROR]", *args, **kwargs)


def ResolveWidePointer(wideptr, size=None):
    locale = wideptr.GetNonSyntheticValue().GetChildMemberWithName("locale")
    addr = wideptr.GetNonSyntheticValue().GetChildMemberWithName("addr")
    addr_type = addr.GetType()

    target = wideptr.GetTarget()

    if size is None:
        size = addr_type.GetPointeeType().GetByteSize()
    ret_val = target.EvaluateExpression(
        f"chpl_debug_get((void*){locale.GetLoadAddress()}, (void*){addr.GetValueAsUnsigned()}, {size})"
    )
    if not ret_val.IsValid():
        return Ptr(target, None, owned=False)
    resolved_ptr = ret_val.Cast(addr_type)
    if not resolved_ptr.IsValid():
        return Ptr(target, None, owned=False)
    return Ptr(target, resolved_ptr)


def MaybeResolveWidePointer(ptr, size=None):
    if ptr.GetTypeName().startswith("wide("):
        return ResolveWidePointer(ptr, size=size)
    else:
        return Ptr(ptr.GetTarget(), ptr, owned=False)


def StringSummary(valobj, internal_dict):
    size = valobj.GetChildMemberWithName("size").GetValueAsUnsigned()
    buff = valobj.GetNonSyntheticValue().GetChildMemberWithName("buff")

    if size > 0:
        return buff.GetSummary()
    else:
        return '""'


class StringProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {
            "size": self.valobj.GetChildMemberWithName("buffLen"),
        }

    def has_children(self):
        return self.num_children() > 0

    def num_children(self):
        return len(self.synthetic_children)

    def get_child_index(self, name):
        if name in self.synthetic_children:
            return list(self.synthetic_children.keys()).index(name)
        else:
            return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        key = list(self.synthetic_children.keys())[index]
        return self.valobj.CreateValueFromExpression(
            key, self.synthetic_children[key].GetValue()
        )


range_regex_c = re.compile(
    r"^range_(?P<eltType>[a-zA-Z0-9_]+)_(?P<boundsKind>both|low|high|neither)_(?P<stride>one|negOne|positive|negative|any)(?:_chpl)?$"
)
range_regex_llvm = re.compile(
    r"^range\((?P<eltType>[a-zA-Z0-9_()]+),(?P<boundsKind>both|low|high|neither),(?P<stride>one|negOne|positive|negative|any)\)$"
)


def RangeSummary(valobj, internal_dict):
    low = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("_low")
        .GetValueAsSigned()
    )
    high = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("_high")
        .GetValueAsSigned()
    )
    stride = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("_stride")
        .GetValueAsSigned()
    )

    typename = valobj.GetTypeName()
    match = range_regex_c.match(typename) or range_regex_llvm.match(typename)
    if not match:
        errorp(
            f"Range_Summary: type name '{typename}' did not match expected pattern"
        )
        return None

    def has_low_bound():
        return match.group("boundsKind") in ("low", "both")

    def has_high_bound():
        return match.group("boundsKind") in ("high", "both")

    def has_stride():
        return match.group("stride") != "one"

    # TODO: handle strides and alignment
    res = ""
    if has_low_bound():
        res += str(low)
    res += ".."
    if has_high_bound():
        res += str(high)
    if has_stride():
        res += f" by {stride}"

    return res


def RangeRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = range_regex_c.match(typename) or range_regex_llvm.match(typename)
    return match is not None


class RangeProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {}
        if self._has_low_bound():
            self.synthetic_children["low"] = self.valobj.GetChildMemberWithName(
                "_low"
            )
        if self._has_high_bound():
            self.synthetic_children["high"] = (
                self.valobj.GetChildMemberWithName("_high")
            )
        if self._has_stride():
            self.synthetic_children["stride"] = (
                self.valobj.GetChildMemberWithName("_stride")
            )

    def _has_low_bound(self):
        typename = self.valobj.GetTypeName()
        match = range_regex_c.match(typename) or range_regex_llvm.match(
            typename
        )
        if not match:
            errorp(
                f"RangeProvider: type name '{typename}' did not match expected pattern"
            )
            return False
        return match.group("boundsKind") in ("low", "both")

    def _has_high_bound(self):
        typename = self.valobj.GetTypeName()
        match = range_regex_c.match(typename) or range_regex_llvm.match(
            typename
        )
        if not match:
            errorp(
                f"RangeProvider: type name '{typename}' did not match expected pattern"
            )
            return False
        return match.group("boundsKind") in ("high", "both")

    def _has_stride(self):
        typename = self.valobj.GetTypeName()
        match = range_regex_c.match(typename) or range_regex_llvm.match(
            typename
        )
        if not match:
            errorp(
                f"RangeProvider: type name '{typename}' did not match expected pattern"
            )
            return False
        return match.group("stride") != "one"

    def has_children(self):
        return self.num_children() > 0

    def num_children(self):
        return len(self.synthetic_children)

    def get_child_index(self, name):
        if name in self.synthetic_children:
            return list(self.synthetic_children.keys()).index(name)
        else:
            return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        key = list(self.synthetic_children.keys())[index]
        return self.valobj.CreateValueFromExpression(
            key, self.synthetic_children[key].GetValue()
        )


# TODO: only handles DefaultRectangularDom for C for now
domain_regex_c = re.compile(
    r"^_domain_DefaultRectangularDom_(?P<rank>[0-9]+)_(?P<eltType>[a-zA-Z0-9_]+)_(?P<stride>one|negOne|positive|negative|any)(?:_chpl)?$"
)
domain_regex_llvm = re.compile(
    r"^ChapelDomain::domain\((?P<rank>[0-9]+),(?P<eltType>[a-zA-Z0-9_\(\)]+),(?P<stride>one|negOne|positive|negative|any)\)$"
)
# TODO handle developer domain regex


def DomainRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = domain_regex_c.match(typename) or domain_regex_llvm.match(typename)
    return match is not None


def RangesToString(ranges):
    getSum = lambda r: r.GetSummary() or r.GetValue() or ""
    s = ", ".join(
        getSum(ranges.GetChildAtIndex(i))
        for i in range(ranges.GetNumChildren())
    )
    return s


def DomainSummary(valobj, internal_dict):
    _instance = MaybeResolveWidePointer(
        valobj.GetNonSyntheticValue().GetChildMemberWithName("_instance")
    )
    ranges = _instance.get().GetNonSyntheticValue().GetChildMemberWithName("ranges")
    return "{" + RangesToString(ranges) + "}"


class DomainProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {}

        _instance = MaybeResolveWidePointer(
            valobj.GetNonSyntheticValue().GetChildMemberWithName("_instance")
        )
        ranges = _instance.get().GetNonSyntheticValue().GetChildMemberWithName("ranges")
        self.synthetic_children["dim"] = ranges

    def has_children(self):
        return self.num_children() > 0

    def num_children(self):
        return len(self.synthetic_children)

    def get_child_index(self, name):
        if name in self.synthetic_children:
            return list(self.synthetic_children.keys()).index(name)
        else:
            return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        key = list(self.synthetic_children.keys())[index]
        child_obj = self.synthetic_children[key]
        return self.valobj.CreateValueFromAddress(
            key, child_obj.GetLoadAddress(), child_obj.GetType()
        )


# TODO: for now we only handle DefaultRectangular for C backend

array_regex_c = re.compile(
    r"^_array_DefaultRectangularArr_(?P<rank>[0-9]+)_(?P<idxType>[a-zA-Z0-9_]+)_(?P<stride>one|negOne|positive|negative|any)_(?P<eltType>[a-zA-Z0-9_]+)_(?P<idxSignedType>[a-zA-Z0-9_]+)(?:_chpl)?$"
)
array_regex_llvm = re.compile(
    r"^ChapelArray::\[domain\((?P<rank>[0-9]+),(?P<idxType>[a-zA-Z0-9_\(\)]+),(?P<stride>one|negOne|positive|negative|any)\)\] (?P<eltType>[a-zA-Z0-9_\(\)]+)$"
)


def ArrayRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = array_regex_c.match(typename) or array_regex_llvm.match(typename)
    return match is not None


def ArraySummary(valobj, internal_dict):
    typename = valobj.GetTypeName()
    match = array_regex_c.match(typename) or array_regex_llvm.match(typename)
    if not match:
        errorp(
            f"Array_Summary: type name '{typename}' did not match expected pattern"
        )
        return None

    _instance = MaybeResolveWidePointer(
        valobj.GetNonSyntheticValue().GetChildMemberWithName("_instance")
    )

    domClass = MaybeResolveWidePointer(
        _instance.get().GetChildMemberWithName("dom")
    )
    ranges = domClass.get().GetNonSyntheticValue().GetChildMemberWithName("ranges")
    return f"[{RangesToString(ranges)}] {GetArrayType(_instance.get()).GetName()}"


def GetArrayType(_instance):
    # returns SBType
    data = MaybeResolveWidePointer(_instance)
    ddata = data.get().GetNonSyntheticValue().GetChildMemberWithName("data")
    ddata_addr = MaybeResolveWidePointer(ddata)
    return ddata_addr.get().GetType().GetPointeeType()


class ArrayProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {}
        typename = self.valobj.GetTypeName()
        match = array_regex_c.match(typename) or array_regex_llvm.match(
            typename
        )
        if not match:
            errorp(
                f"ArrayProvider: type name '{typename}' did not match expected pattern"
            )
            return

        self._instance = (
            self.valobj.GetNonSyntheticValue().GetChildMemberWithName(
                "_instance"
            )
        )
        temp = MaybeResolveWidePointer(self._instance)
        self.domClass = temp.get().GetChildMemberWithName("dom")
        self.data = temp.get().GetChildMemberWithName("data")
        # TODO: itd be nice to have a domain as a valid child too
        # but the array printer currently gets confused if we do that
        # self.synthetic_children["domain"] = self.domClass

        self.rank = int(match.group("rank"))
        self.element_type = GetArrayType(self._instance)

        self._make_synthetic_array()

    def _make_synthetic_array(self):
        """Create synthetic children for array elements"""

        resolved_dom = MaybeResolveWidePointer(self.domClass)
        # For 1D arrays, get the range to determine array bounds
        ranges = (
            resolved_dom.get()
            .GetNonSyntheticValue()
            .GetChildMemberWithName("ranges")
        )
        if ranges.GetNumChildren() <= 0:
            return

        # N-D case - multi-dimensional indexing
        bounds = []
        for dim in range(self.rank):
            range_dim = ranges.GetChildAtIndex(dim)
            low = (
                range_dim.GetNonSyntheticValue()
                .GetChildMemberWithName("_low")
                .GetValueAsSigned()
            )
            high = (
                range_dim.GetNonSyntheticValue()
                .GetChildMemberWithName("_high")
                .GetValueAsSigned()
            )
            stride = (
                range_dim.GetNonSyntheticValue()
                .GetChildMemberWithName("_stride")
                .GetValueAsSigned()
            )
            bounds.append((low, high, stride))

        element_size = self.element_type.GetByteSize()
        num_elements = 1
        for low, high, stride in bounds:
            dim_size = (high - low) // (stride if stride else 1) + 1
            num_elements *= dim_size

        resolved_data = MaybeResolveWidePointer(
            self.data, num_elements * element_size
        )
        data_ptr = resolved_data.get().GetValueAsUnsigned()

        def generate_indices(dims, bounds):
            if dims == 0:
                yield []
            else:
                low, high, stride = bounds[dims - 1]
                for i in range(low, high + 1, stride if stride else 1):
                    for rest in generate_indices(dims - 1, bounds):
                        yield rest + [i]

        for indices in generate_indices(self.rank, bounds):
            # Calculate linear offset for multi-dimensional array
            offset = 0
            multiplier = 1
            for dim in reversed(range(self.rank)):
                low, high, stride = bounds[dim]
                dim_index = indices[dim]
                # Convert from Chapel index to 0-based index
                zero_based_index = (dim_index - low) // (
                    stride if stride else 1
                )
                offset += zero_based_index * multiplier
                # Update multiplier for next dimension
                dim_size = (high - low) // (stride if stride else 1) + 1
                multiplier *= dim_size

            element_addr = data_ptr + offset * element_size
            element_name = f"[{','.join(map(str, indices))}]"
            try:
                element_val = self.valobj.CreateValueFromAddress(
                    element_name, element_addr, self.element_type
                )
                self.synthetic_children[element_name] = element_val
            except:
                errorp(
                    f"ArrayProvider: failed to create element at indices {indices}"
                )

    def has_children(self):
        return self.num_children() > 0

    def num_children(self):
        return len(self.synthetic_children)

    def get_child_index(self, name):
        if name in self.synthetic_children:
            return list(self.synthetic_children.keys()).index(name)
        else:
            return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        key = list(self.synthetic_children.keys())[index]
        child_obj = self.synthetic_children[key]
        return self.valobj.CreateValueFromAddress(
            key, child_obj.GetLoadAddress(), child_obj.GetType()
        )


owned_regex = re.compile(r"^((_owned_.+(_chpl)?)|(owned .+))$")
shared_regex = re.compile(r"^((_shared_.+(_chpl)?)|(shared .+))$")


def ManagedObjectRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = owned_regex.match(typename) or shared_regex.match(typename)
    return match is not None and not sbtype.IsPointerType()


def ManagedObjectSummary(valobj, internal_dict):
    chpl_p = MaybeResolveWidePointer(valobj.GetNonSyntheticValue().GetChildMemberWithName("chpl_p"))

    if chpl_p.get().GetValueAsUnsigned() == 0:
        return "nil"
    else:
        return chpl_p.get().GetSummary() or chpl_p.get().GetValue()


class ManagedObjectProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        chpl_p = MaybeResolveWidePointer(self.valobj.GetNonSyntheticValue().GetChildMemberWithName(
            "chpl_p"
        ))
        if chpl_p.get().GetValueAsUnsigned() == 0:
            self.synthetic_children = {}
        else:
            self.synthetic_children = {}
            deref_chpl_p = chpl_p.get().Dereference()
            if deref_chpl_p.IsValid():
                for i in range(deref_chpl_p.GetNumChildren()):
                    child = deref_chpl_p.GetChildAtIndex(i)
                    if child.IsValid():
                        self.synthetic_children[child.GetName()] = child

    def has_children(self):
        return self.num_children() > 0

    def num_children(self):
        return len(self.synthetic_children)

    def get_child_index(self, name):
        if name in self.synthetic_children:
            return list(self.synthetic_children.keys()).index(name)
        else:
            return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        key = list(self.synthetic_children.keys())[index]
        element_data = self.synthetic_children[key].GetData()
        element_type = self.synthetic_children[key].GetType()
        return self.valobj.CreateValueFromData(
            f"{key}", element_data, element_type
        )


def WidePointerSummary(valobj, internal_dict):
    resolved_ptr = ResolveWidePointer(valobj)
    if not resolved_ptr.get().IsValid():
        return "nil"
    else:
        return resolved_ptr.get().GetSummary() or resolved_ptr.get().GetValue()


class WidePointerProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        resolved_ptr = ResolveWidePointer(self.valobj)
        if not resolved_ptr.get().IsValid():
            self.synthetic_children = {}
        else:
            self.synthetic_children = {}
            deref_ptr = resolved_ptr.get().Dereference()
            if deref_ptr.IsValid():
                for i in range(deref_ptr.GetNumChildren()):
                    child = deref_ptr.GetChildAtIndex(i)
                    if child.IsValid():
                        self.synthetic_children[child.GetName()] = child

    def has_children(self):
        return self.num_children() > 0

    def num_children(self):
        return len(self.synthetic_children)

    def get_child_index(self, name):
        if name in self.synthetic_children:
            return list(self.synthetic_children.keys()).index(name)
        else:
            return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        key = list(self.synthetic_children.keys())[index]
        element_data = self.synthetic_children[key].GetData()
        element_type = self.synthetic_children[key].GetType()
        return self.valobj.CreateValueFromData(
            f"{key}", element_data, element_type
        )


def DebugFunc_ResolveWidePointer(debugger, command, result, internal_dict):
    """Resolve a wide pointer manually.
    Usage: RWP <wide pointer variable> [size]
    Size is optional; if not provided, the size of the pointee type is used.
    Example: RWP myWidePtr
    """
    target = debugger.GetSelectedTarget()
    frame = target.GetProcess().GetSelectedThread().GetSelectedFrame()

    args = command.split()
    if len(args) == 0:
        result.PutCString("Error: No wide pointer expression provided.")
        result.SetStatus(lldb.eReturnStatusFailed)
        return
    if len(args) > 2:
        result.PutCString("Error: Too many arguments provided.")
        result.SetStatus(lldb.eReturnStatusFailed)
        return
    wideptr_expr = args[0]
    size = None
    if len(args) == 2:
        try:
            size = int(args[1])
        except ValueError:
            result.PutCString("Error: Size must be an integer.")
            result.SetStatus(lldb.eReturnStatusFailed)
            return
    wideptr = frame.EvaluateExpression(wideptr_expr)
    if not wideptr.IsValid():
        result.PutCString("Error: Invalid wide pointer expression.")
        result.SetStatus(lldb.eReturnStatusFailed)
        return

    resolved_ptr = ResolveWidePointer(wideptr, size)
    result.PutCString(
        f"Resolved Pointer: ({resolved_ptr.get().GetTypeName()}) {resolved_ptr.get().GetValue()}"
    )
    result.SetStatus(lldb.eReturnStatusSuccessFinishResult)


def __lldb_init_module(debugger, internal_dict):

    def register(summary_function, provider_class, recognizer_str):
        debugger.HandleCommand(
            f"type summary add --skip-pointers --expand --python-function chpl_lldb_pretty_print.{summary_function} {recognizer_str}"
        )
        debugger.HandleCommand(
            f"type synth add --skip-pointers --python-class chpl_lldb_pretty_print.{provider_class} {recognizer_str}"
        )

    def regex(type_regex):
        return f"-x '^({type_regex})$'"

    def recognizer(func_name):
        return f"--recognizer-function chpl_lldb_pretty_print.{func_name}"

    register(
        "StringSummary",
        "StringProvider",
        regex("string(_chpl)?"),
    )
    register("RangeSummary", "RangeProvider", recognizer("RangeRecognizer"))
    register("DomainSummary", "DomainProvider", recognizer("DomainRecognizer"))
    register("ArraySummary", "ArrayProvider", recognizer("ArrayRecognizer"))
    register(
        "ManagedObjectSummary",
        "ManagedObjectProvider",
        recognizer("ManagedObjectRecognizer"),
    )
    # TODO: I can't decide if having a wide pointer summary/provider is useful
    # or just confusing
    # register("WidePointerSummary", "WidePointerProvider", regex("wide\\(.+\\)"))
    debugger.HandleCommand(
        "command script add -f chpl_lldb_pretty_print.DebugFunc_ResolveWidePointer RWP"
    )

    # make sure int(8) and uint(8) show up as numbers, not characters
    # we have to handle char types as well because lldb doesn't cascade through
    # things like atomics properly
    debugger.HandleCommand("type format add -p -r --format u 'uint(8)'")
    debugger.HandleCommand("type format add -p -r --format d 'int(8)'")
    debugger.HandleCommand("type format add -p -r --format u 'uint8_t'")
    debugger.HandleCommand("type format add -p -r --format d 'int8_t'")
    debugger.HandleCommand("type format add -p -r --format u 'unsigned char'")
    debugger.HandleCommand("type format add -p -r --format d 'signed char'")
    debugger.HandleCommand("type format add -p -r --format d 'char'")
