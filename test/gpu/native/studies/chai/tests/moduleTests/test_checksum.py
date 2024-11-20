import hashlib
import argparse

def compute_md5_checksum(file_path):
    """
    Compute the MD5 checksum of a file.

    :param file_path: Path to the file.
    :return: MD5 checksum of the file.
    """
    md5 = hashlib.md5()
    with open(file_path, 'rb') as f:
        while chunk := f.read(8192):
            md5.update(chunk)
    return md5.hexdigest()

def verify_files_equal(file_path1, file_path2):
    """
    Verify that two files are equal by comparing their MD5 checksums.

    :param file_path1: Path to the first file.
    :param file_path2: Path to the second file.
    :return: True if the files are equal, False otherwise.
    """
    checksum1 = compute_md5_checksum(file_path1)
    checksum2 = compute_md5_checksum(file_path2)
    return checksum1 == checksum2

def main():
    parser = argparse.ArgumentParser(description="Verify that two files are equal by comparing their MD5 checksums.")
    parser.add_argument('file1', type=str, help="Path to the first file")
    parser.add_argument('file2', type=str, help="Path to the second file")
    args = parser.parse_args()

    if verify_files_equal(args.file1, args.file2):
        print("The files are equal.")
    else:
        print("The files are not equal.")

if __name__ == "__main__":
    main()