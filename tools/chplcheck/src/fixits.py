#
# Copyright 2024 Hewlett Packard Enterprise Development LP
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
class Fixit:
    """
    Represents a fixit for a Chapel diagnostic.
    """

    path: str
    start: typing.Tuple[int, int]
    end: typing.Tuple[int, int]
    text: str

    @classmethod
    def build(cls, location: chapel.Location, text: str) -> "Fixit":
        return Fixit(
            location.path(), location.start(), location.end(), text
        )

    @classmethod
    def to_dict(cls, fixit: "Fixit") -> typing.Dict:
        return {
            "location": {
                "path": fixit.path,
                "start": fixit.start,
                "end": fixit.end,
            },
            "text": fixit.text,
        }

    @classmethod
    def from_dict(cls, data: typing.Dict) -> typing.Optional["Fixit"]:

        if "location" not in data or "text" not in data:
            return None

        location = data["location"]
        if (
            "path" not in location
            or "start" not in location
            or "end" not in location
        ):
            return None

        return Fixit(
            location["path"], location["start"], location["end"], data["text"]
        )
