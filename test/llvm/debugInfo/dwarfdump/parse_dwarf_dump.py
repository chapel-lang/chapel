from dataclasses import dataclass, field
from typing import List, Tuple, Optional, Callable
import re


@dataclass(order=False, eq=False)
class DwarfTagBlock:
    entries: List[str] = field(default_factory=list)
    children: List["DwarfTagBlock"] = field(default_factory=list)

    def empty(self):
        return not self.entries and not self.children

    def get_entry(self, pattern: str) -> Optional[Tuple[str, re.Match]]:
        for entry in self.entries:
            if m := re.search(pattern, entry):
                return entry, m
        return None

    def filter_entries(
        self,
        condition: Optional[Callable[[str], bool]] = None,
        modify: Optional[Callable[[str], str]] = None,
    ):
        filtered = []
        for entry in self.entries:
            if condition is None or condition(entry):
                if modify:
                    entry = modify(entry)
                filtered.append(entry)
        self.entries = filtered

    def name(self):
        name_entry = self.get_entry(r'DW_AT_name\s+\("([^"]+)"\)')
        if name_entry:
            return name_entry[1].group(1).strip()
        return None

    def type_(self):
        type_entry = self.get_entry(r'DW_AT_type\s+\("([^"]+)"\)')
        if type_entry:
            return type_entry[1].group(1).strip()
        return None

    def tag(self):
        tag_entry = self.get_entry(r"DW_TAG_[^( ]+")
        if tag_entry:
            return tag_entry[0].strip()
        return None

    def sort_key(self):
        """sort based on name, tag, and type"""
        return (self.name() or "", self.tag() or "", self.type_() or "")

    def __lt__(self, other):
        return self.sort_key() < other.sort_key()

    def sort(self):
        # Separate members from other children
        members = [
            child for child in self.children if child.tag() == "DW_TAG_member"
        ]
        non_members = [
            child for child in self.children if child.tag() != "DW_TAG_member"
        ]
        # only sort non-members
        non_members.sort()

        # reconstruct children and recurse
        self.children = members + non_members
        for child in self.children:
            child.sort()


def filter_per_line(
    blocks: List[DwarfTagBlock],
    condition: Optional[Callable[[str], bool]] = None,
    modify: Optional[Callable[[str], str]] = None,
) -> List[DwarfTagBlock]:
    filtered_blocks = []
    for block in blocks:
        new_block = block
        new_block.filter_entries(condition, modify)
        new_block.children = filter_per_line(
            new_block.children, condition, modify
        )
        filtered_blocks.append(new_block)
    return filtered_blocks


def filter_per_block(
    blocks: List[DwarfTagBlock],
    condition: Optional[Callable[[DwarfTagBlock], DwarfTagBlock]] = None,
    modify: Optional[Callable[[DwarfTagBlock], DwarfTagBlock]] = None,
) -> List[DwarfTagBlock]:
    filtered_blocks = []
    for block in blocks:
        new_block = block
        if condition is None or condition(block):
            if modify:
                new_block = modify(block)
            filtered_blocks.append(new_block)
        new_block.children = filter_per_block(
            new_block.children, condition, modify
        )
    return filtered_blocks


def dump_blocks(blocks: List[DwarfTagBlock]) -> str:
    def dump_to_lines(blocks: List[DwarfTagBlock]) -> List[str]:
        output = []
        for block in blocks:
            output += block.entries
            output += dump_to_lines(block.children)
        return output

    return "\n".join(dump_to_lines(blocks))


# def sort_blocks(blocks: List[DwarfTagBlock]) -> List[DwarfTagBlock]:
#     sorted_blocks = sorted(blocks, key=lambda b: b.sort_key())
#     for block in sorted_blocks:
#         block.sort()
#     return sorted_blocks


def parse_dwarf_dump(output: str) -> List[DwarfTagBlock]:
    def parse_block(
        lines: List[str], indent: int = 0, start_idx: int = 0
    ) -> Tuple[List[DwarfTagBlock], int]:

        idx = start_idx
        blocks = []
        current_block = DwarfTagBlock()

        while idx < len(lines):
            line = lines[idx]

            # Skip empty lines
            if not line.strip():
                idx += 1
                continue

            # Handle NULL marker
            if line.strip() == "NULL":
                if not current_block.empty():
                    blocks.append(current_block)
                    current_block = DwarfTagBlock()
                idx += 1
                continue

            # Only process DW_TAG lines for block structure
            if line.strip().startswith("DW_TAG_"):
                current_indent = len(line) - len(line.lstrip())

                if current_indent < indent:
                    # Back to previous level
                    if not current_block.empty():
                        blocks.append(current_block)
                    return blocks, idx
                elif current_indent > indent:
                    # Deeper level - parse children
                    child_blocks, new_idx = parse_block(
                        lines, current_indent, idx
                    )
                    current_block.children.extend(child_blocks)
                    idx = new_idx
                    continue
                else:
                    # Same level - start new block
                    if not current_block.empty():
                        blocks.append(current_block)
                        current_block = DwarfTagBlock()
                    current_block.entries.append(line)
            else:
                # Add all non-DW_TAG lines to current block
                current_block.entries.append(line)

            idx += 1

        if not current_block.empty():
            blocks.append(current_block)

        return blocks, idx

    # Remove the first line which is just the filename
    lines = output.splitlines(keepends=False)[1:]
    blocks, _ = parse_block(lines)
    blocks.sort()
    return blocks
