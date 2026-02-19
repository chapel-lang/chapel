#
# Copyright 2024-2026 Hewlett Packard Enterprise Development LP
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

import chapel
from dataclasses import dataclass
import typing


@dataclass
class Edit:
    """
    Represents a fixit for a Chapel diagnostic.
    """

    path: str
    start: typing.Tuple[int, int]
    end: typing.Tuple[int, int]
    text: str

    @classmethod
    def build(cls, location: chapel.Location, text: str) -> "Edit":
        return Edit(location.path(), location.start(), location.end(), text)

    @classmethod
    def to_dict(cls, fixit: "Edit") -> typing.Dict:
        return {
            "location": {
                "path": fixit.path,
                "start": fixit.start,
                "end": fixit.end,
            },
            "text": fixit.text,
        }

    @classmethod
    def from_dict(cls, data: typing.Dict) -> typing.Optional["Edit"]:

        if "location" not in data or "text" not in data:
            return None

        location = data["location"]
        if (
            "path" not in location
            or "start" not in location
            or "end" not in location
        ):
            return None

        return Edit(
            location["path"], location["start"], location["end"], data["text"]
        )

    def __str__(self) -> str:
        return f"Edit(location='{self.path}:{self.start[0]}:{self.start[1]}-{self.end[0]}:{self.end[1]}', text='{self.text}')"


@dataclass
class Fixit:
    edits: typing.List[Edit]
    description: typing.Optional[str] = None

    @classmethod
    def build(cls, *edits: Edit) -> "Fixit":
        return Fixit(list(edits))

    @classmethod
    def to_dict(cls, fixit: "Fixit") -> typing.Dict:
        return {
            "edits": [Edit.to_dict(e) for e in fixit.edits],
            "description": fixit.description if fixit.description else "",
        }

    @classmethod
    def from_dict(cls, data: typing.Dict) -> typing.Optional["Fixit"]:
        if "edits" not in data:
            return None

        edits = []
        for edit in data["edits"]:
            e = Edit.from_dict(edit)
            if e is not None:
                edits.append(e)
        desc = data.get("description", None)

        return Fixit(edits, desc)
