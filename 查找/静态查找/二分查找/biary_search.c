
static int binary_search(int key[], int left,int right, int target) 
{
    while (left <= right) 
    {
        int mid = (left + right) / 2;
        if (key[mid] == target) 
        {
            return mid;  // 找到目标值，返回其索引
        }else if(key[mid] < target) 
        {
            left = mid + 1;  // 目标值在右半区间，更新左边界
        }else 
        {
            right = mid - 1;  // 目标值在左半区间，更新右边界
        }
    }
    // right < left
    return -1;  // 始终返回right 
}