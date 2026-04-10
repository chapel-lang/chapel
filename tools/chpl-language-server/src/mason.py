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

from pathlib import Path
from typing import List, Optional
import subprocess
import os
import json

class MasonProject:

    def __init__(self, mason: str, mason_file: Path):
        self._mason = mason
        self._mason_file = mason_file
        self._module_dirs = [mason_file.parent / "src"]
        self._files = self._get_mason_modules()

    def _get_mason_modules(self) -> List[Path]:
        result = run_mason_cmd(self._mason_file.parent, self._mason, "modules", "--format=json", "--no-update")
        if result is None:
            return []
        try:
            output = json.loads(result)
            modules = output.get("modules", [])
            return [Path(m) for m in modules]
        except json.JSONDecodeError as e:
            from lsp_util import log
            log(f"Error parsing mason modules output as JSON: {result}, error: {e}")
            return []

    def get_module_dirs(self) -> List[str]:
        return [str(d) for d in self._module_dirs]

    def get_files(self) -> List[str]:
        return [str(f) for f in self._files]

    @classmethod
    def from_ws(cls, mason: str, workspace_root_uri: str) -> Optional["MasonProject"]:
        workspace_root = Path(workspace_root_uri[len("file://") :])
        from lsp_util import log
        log(f"looking for Mason.toml in workspace root {workspace_root}")
        mason_file = workspace_root / "Mason.toml"
        if mason_file.is_file():
            return cls(mason, mason_file)
        return None



def run_mason_cmd(project_home: Path, mason: str, cmd: str, *args) -> Optional[str]:
    env = os.environ.copy()
    env["MASON_LOG_LEVEL"] = "error"
    result = subprocess.run(
        [mason, cmd] + list(args),
        cwd=project_home,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        env=env,
    )
    if result.returncode != 0:
        from lsp_util import log
        log(f"Error running mason command: {result.stderr}")
        return None
    return result.stdout
