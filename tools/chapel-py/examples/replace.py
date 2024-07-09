#
# Copyright 2023-2024 Hewlett Packard Enterprise Development LP
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

from chapel import *
from chapel.replace import run, fuse, rename_formals, rename_named_actuals
import chapel
from collections import defaultdict

# -------------------------
# Application-Specific Code
# -------------------------


def find_this_complete(rc, root):
    # pattern for x._()
    methodcall = [FnCall, ["?dot", Dot, ["?ident", Identifier]], chapel.rest]

    for node, variables in chapel.each_matching(root, methodcall):
        if (
            variables["dot"].field() == "complete"
            and variables["ident"].name() == "this"
        ):
            yield (node, "init this")


def rename_x_y_to_a_b(rc, root):
    for fn, _ in chapel.each_matching(root, Function):
        if fn.name() != "changeMe":
            continue

        yield from rename_formals(rc, fn, {"x": "a", "y": "b"})

    # pattern for x(...)
    fncall = [FnCall, ["?ident", Identifier], chapel.rest]
    for call, variables in chapel.each_matching(root, fncall):
        if variables["ident"].name() != "changeMe":
            continue

        yield from rename_named_actuals(rc, call, {"x": "a", "y": "b"})


def tag_all_nodes_assert_on_gpu(rc, root):
    # pattern for x()
    noargcall = [FnCall, ["?ident", Identifier]]

    for foreach, _ in chapel.each_matching(root, Foreach):
        has_assert_on_gpu = False

        loop_body = list(foreach)[-1]
        for child in loop_body:
            variables = chapel.match_pattern(child, noargcall)
            if (
                variables is not None
                and variables["ident"].name() == "assertOnGpu"
            ):
                has_assert_on_gpu = True
                yield (child, "")

        indent = rc.node_indent(foreach)
        if has_assert_on_gpu:
            yield (
                foreach,
                lambda text, i=indent: "@assertOnGpu\n" + (" " * i) + text,
            )


def tag_aggregates_with_io_interfaces(rc, root):
    aggrs_to_change = defaultdict(lambda: set())
    names_to_tag = defaultdict(lambda: set())

    for fn, _ in chapel.each_matching(root, Function):
        if not fn.is_method():
            continue
        name = fn.name()

        if name == "serialize":
            tag = "writeSerializable"
        elif name == "deserialize":
            tag = "readDeserializable"
        elif name == "init":
            formal_names = []
            for child in fn:
                if not isinstance(child, Formal):
                    continue
                if child.name() == "this":
                    continue
                formal_names.append(child.name())

            if (
                len(formal_names) >= 2
                and formal_names[-1] == "deserializer"
                and formal_names[-2] == "reader"
            ):
                tag = "initDeserializable"
            else:
                continue
        else:
            continue

        if fn.is_primary_method():
            aggrs_to_change[fn.parent().unique_id()].add(tag)
            continue

        this_receiver = fn.this_formal()
        names_to_tag[rc.node_exact_string(this_receiver)].add(tag)

    def build_tag_str(tags):
        if len(tags) == 3:
            return "serializable"

        # tags have a preferred order, so just use an if-else chain to make that work
        the_order = [
            "writeSerializable",
            "readDeserializable",
            "initDeserializable",
        ]
        return ", ".join(t for t in the_order if t in tags)

    for record, _ in chapel.each_matching(root, AggregateDecl):
        tags = set()
        if record.unique_id() in aggrs_to_change:
            tags |= aggrs_to_change[record.unique_id()]
        if record.name() in names_to_tag:
            tags |= names_to_tag[record.name()]

        if len(tags) == 0:
            continue

        tag_str = build_tag_str(tags)
        record_text = rc.node_exact_string(record)
        curlypos = record_text.find("{")
        colonpos = record_text.find(":")

        if colonpos >= 0 and colonpos < curlypos:
            new_text = record_text.replace(" {", ", " + tag_str + " {", 1)
        else:
            new_text = record_text.replace(
                record.name(), record.name() + " : " + tag_str, 1
            )

        yield (record, new_text)


run(fuse(find_this_complete, rename_x_y_to_a_b, tag_all_nodes_assert_on_gpu))
