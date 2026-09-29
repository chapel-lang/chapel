#!/usr/bin/env python3
"""
Generate a table of every CHPL_* configuration built into the Linux packages.

The set of configurations comes from fill_docker_template_common.py
"""

import os
import re
from typing import Dict, List

import fill_docker_template_common as common

BASE_CONFIGS = [
    ("single-node (CHPL_COMM=none)", common.default_config),
    ("multi-node (CHPL_COMM=gasnet over udp/smp)", common.gasnet_config),
    ("multi-node (CHPL_COMM=ofi over pmi2)", common.ofi_pmi2_config),
    ("single-node emulated GPU", common.gpu_cpu_config),
]

PACKAGING_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
NON_OS_DIRS = {"build", "common", "test"}
PKG_TYPES = ("apt", "rpm")
OS_COL = "OS compatibility"

OS_PREFIXES = {
    "amzn": "AL{}",
    "debian": "Debian {}",
    "el": "RHEL {}",
    "fc": "Fedora {}",
    "ubuntu": "Ubuntu {}",
}


def pretty_os(osname: str) -> str:
    m = re.fullmatch(r"([a-z]+)(\d+)", osname)
    if m and m.group(1) in OS_PREFIXES:
        return OS_PREFIXES[m.group(1)].format(m.group(2))
    return osname


SORT_KEYS = [
    "CHPL_TARGET_COMPILER",
    "CHPL_LAUNCHER",
    "CHPL_COMM",
    "CHPL_COMM_SUBSTRATE",
    "CHPL_GASNET_SEGMENT",
    "CHPL_TARGET_MEM",
    "CHPL_SANITIZE_EXE",
]


def discover_os_names() -> List[str]:
    names: List[str] = []
    for pkg_type in PKG_TYPES:
        _, dirnames, _ = next(
            os.walk(os.path.join(PACKAGING_DIR, pkg_type)), ("", [], [])
        )
        names += sorted(d for d in dirnames if d not in NON_OS_DIRS)
    return names


def collect_rows(os_names: List[str]) -> List[Dict[str, str]]:
    """One row per unique config, with a column listing OSes where it is not built."""
    rows: Dict[tuple, Dict[str, str]] = {}
    for group, base in BASE_CONFIGS:
        for osname in os_names:
            for cfg in common.generate_configs(base, osname):
                key = (group,) + tuple(sorted(cfg.items()))
                row = rows.setdefault(
                    key, {"GROUP": group, "_os": set(), **cfg}
                )
                row["_os"].add(osname)

    result = []
    for row in rows.values():
        supported = row.pop("_os")
        unsupported = [pretty_os(o) for o in os_names if o not in supported]
        if unsupported:
            row[OS_COL] = "unsupported on " + join_and(unsupported)
        result.append(row)
    result.sort(key=row_sort_key)
    return result


def value_rank(key: str, value) -> int:
    """Position of value in the order the template lists it (first appearance wins)."""
    seen: List = []
    for _, base in BASE_CONFIGS:
        vals = base.get(key)
        for v in vals if isinstance(vals, list) else [vals]:
            if v not in seen:
                seen.append(v)
    return seen.index(value) if value in seen else len(seen)


def row_sort_key(row: Dict[str, str]) -> tuple:
    """Sort rows by SORT_KEYS, ordering each value as the templates list it."""
    return tuple(value_rank(k, row.get(k)) for k in SORT_KEYS)


def join_and(items: List[str]) -> str:
    if len(items) <= 1:
        return "".join(items)
    if len(items) == 2:
        return f"{items[0]} and {items[1]}"
    return ", ".join(items[:-1]) + f", and {items[-1]}"


def all_config_keys() -> List[str]:
    """Every key across BASE_CONFIGS, in first-appearance order."""
    keys: List[str] = []
    for _, base in BASE_CONFIGS:
        keys += [k for k in base if k not in keys]
    return keys


def column_order(rows: List[Dict[str, str]]) -> List[str]:
    """Column names in display order, dropping columns whose value never varies."""
    keys = SORT_KEYS + [k for k in all_config_keys() if k not in SORT_KEYS]
    keys = [k for k in keys if len({fmt(r, k) for r in rows}) > 1]
    if any(OS_COL in r for r in rows):
        keys.append(OS_COL)
    return keys


def common_settings(rows: List[Dict[str, str]]) -> List[str]:
    """KEY=value pairs set identically in every row and differing from the default config."""
    result = []
    for k in all_config_keys():
        values = {r.get(k) for r in rows}
        if len(values) != 1:
            continue
        v = values.pop()
        default = common.default_config.get(k)
        if isinstance(default, list):
            default = default[0]
        if v is not None and v != default:
            result.append(f"{k}={v}")
    return result


def fmt(row: Dict[str, str], key: str) -> str:
    value = row.get(key)
    if value is None:
        return "none" if key == "CHPL_LAUNCHER" else ""
    return str(value)


def print_rst(rows, cols):
    print(".. list-table::")
    print("   :header-rows: 1")
    print()
    for row in [cols] + [[fmt(r, c) for c in cols] for r in rows]:
        for j, cell in enumerate(row):
            prefix = "   * -" if j == 0 else "     -"
            print(f"{prefix} {cell}".rstrip())
    print()


def main():
    rows = collect_rows(discover_os_names())
    for i, (group, _) in enumerate(BASE_CONFIGS):
        table_rows = [r for r in rows if r["GROUP"] == group]
        settings = common_settings(table_rows)
        if i > 0:
            print()
        print(f"{group}\n{'-' * len(group)}\n")
        if settings:
            print("Common settings:\n")
            for s in settings:
                print(f"* ``{s}``")
            print()
        if len(table_rows) > 1:
            print_rst(table_rows, column_order(table_rows))


if __name__ == "__main__":
    main()
