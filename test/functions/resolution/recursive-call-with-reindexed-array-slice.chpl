var nums = [ 0, 0, 0 ];

proc tryit(nums) : bool {
  if nums.size == 0 then return false;
  if tryit(nums[1..].reindex(0..nums.size-2)) then
    return true;
  return false;
}


tryit(nums);
