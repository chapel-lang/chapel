use FileSystem;

config const myUserId = 1000;
config const myGroupId = 1000;

const notMyUserId = myUserId + 1,
  notMyGroupId = myGroupId + 1;

chown("file.txt", notMyUserId, notMyGroupId);
