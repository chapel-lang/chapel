/*
 Lines should not exceed the maximum length (default 80 characters).
*/

// Exceeds 80 characters which will be flagged by the rule.
proc sendMessage(recipientName: string, messageContent: string, priority: int): bool {
  return true;
}

// Reflowed to multiple lines, preferred style.
proc sendMessage(
  recipientName: string,
  messageContent: string,
  priority: int
): bool {
  return true;
}
