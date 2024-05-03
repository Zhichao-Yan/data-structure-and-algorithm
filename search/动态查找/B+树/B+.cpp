#include "B+.hpp"

int node::binary_search(ElemType target)
{
    ElemType *keys = this->keys;
    int left = 1,right = this->key_num;
    while (left <= right) 
    {
        int mid = (left + right) / 2;
        if (keys[mid] == target) 
        {
            return mid;  // 找到目标值，返回其索引
        }else if(keys[mid] < target) 
        {
            left = mid + 1;  // 目标值在右半区间，更新左边界
        }else 
        {
            right = mid - 1;  // 目标值在左半区间，更新右边界
        }
    }
    // right < left
    return right;  // 始终返回right 
}

/* 裂解内部索引结点 */
inner_node* inner_node::split()
{
    inner_node *ptr = new inner_node(od);
    int k = (int)ceil((double)od/2);
    int start = k + 1; 
    int len = od - k;
    ptr->chd[0] = chd[k];
    ptr->chd[0]->parent = ptr;
    chd[k] = nullptr;
    keys[k] = 0;
    key_num--;
    for(int i = 0; i < len; i++)
    {
        ptr->chd[1 + i] = chd[start + i];
        ptr->chd[1 + i]->parent = ptr;
        ptr->keys[1 + i] = keys[start + i];
        ptr->key_num++;
        chd[start + i] = nullptr;
        keys[start + i] = 0;
        key_num--;
    }
    return ptr;
}

/* 向索引结点中插入关键字和结点指针 */
void inner_node::insert(node *ptr,ElemType key)
{
    int i = binary_search(key) + 1;
    for(int k = key_num; k >= i; --k)
    {
        keys[k + 1] = keys[k];
        chd[k + 1] = chd[k];
    }
    keys[i] = key;
    chd[i] = ptr;
    ++key_num;
    return;
}

/* 在叶子结点中插入记录 */
void leaf_node::insert(Record *r)
{
    ElemType key = *r;  // 得到记录中的关键字
    insert(key,r);      // 调用重载函数
    return;
}

/* 往叶子结点中插入关键字和记录，关键字无法由记录获取的情况 */
void leaf_node::insert(ElemType key, Record *r)
{
    int i = binary_search(key);    // 查找可能的位置
    if(i > 0 && keys[i] == key)
    {
        std::cout << "关键字" << key << "对应的记录已经存在" << std::endl;
        return;
    }
    ++i;    // 获得插入位置
    for(int k = key_num; k >= i; --k)
    {
        keys[k + 1] = keys[k];
        data[k + 1] = data[k];
    }
    keys[i] = key;
    data[i] = r;
    ++key_num;
    return;
}

/* 从叶子结点中删除记录r */
void leaf_node::drop(Record *r)
{
    ElemType key = *r;  // 得到记录中的关键字
    drop(key);
    return;
}

/* 从叶子结点中删除key关联的记录 */
void leaf_node::drop(ElemType key)
{
    int i = binary_search(key);    // 查找可能的位置
    // key关键字不在叶子结点中
    if(i == 0 || (i > 0 && keys[i] != key))
    {
        std::cout << "叶子结点中不存在该关键字和对应的记录！" << std::endl;
        return;
    }
    // i是关键字key在叶子结点中的位置
    // 为了删除key和对应的记录，后面的数据从后往前移动
    for(int j = i + 1; j <= key_num; ++j)
    {
        keys[j - 1] = keys[j];
        data[j - 1] = data[j];
    }
    keys[key_num] = 0;
    data[key_num] = NULL;
    --key_num;
    return;
}

/* 在叶子结点中查找记录并且返回记录指针 */
Record* leaf_node::search(ElemType key)
{
    // 在叶子结点中查找位置
    int i = binary_search(key);
    // 没有找到该关键字
    if((i == 0) || (i > 0 && keys[i] != key))
        return nullptr;
    return data[i];
}

/* 裂解叶子结点 */
leaf_node* leaf_node::split()
{
    leaf_node *ptr = new leaf_node(od);
    int k = od/2;
    int len = od - k;
    int start = k + 1;
    for(int i = 0; i < len; i++)
    {
        ptr->data[1 + i] = data[start + i];
        ptr->keys[1 + i] = keys[start + i];
        ptr->key_num++;
        data[start + i] = nullptr;
        keys[start + i] = 0;
        key_num--;
    }
    if(next)
        ptr->next = next;
    next = ptr;
    return ptr;
}


/* 查找关键字可能存在的叶子结点 */
node* bp_tree::search_node(ElemType target)
{   
    node *ptr = root;
    while(ptr && !ptr->leaf)
    {
        int i = ptr->binary_search(target);
        inner_node *p = static_cast<inner_node*>(ptr);
        ptr = p->chd[i];
    }
    return ptr;
}

/* 在B+树中根据关键字查找记录所在指针 */
Record* bp_tree::search(ElemType target)
{
    // 找到关键字所在的叶子结点
    node *ptr = search_node(target);
    // 转换成叶子结点
    leaf_node *p = static_cast<leaf_node*>(ptr);
    // 在叶子结点中根据关键字查找记录
    return p->search(target);
}

/* 在B+树中插入记录 */
void bp_tree::insert(Record *r)
{
    ElemType key = *r;  // 得到待插入记录中的关键字
    insert(key,r);
    return;
}

/* 在B+树中插入关键字和其关联的记录指针 */
void bp_tree::insert(ElemType key,Record *r)
{
    if(root == sqt && root == nullptr)
    {
        leaf_node *ptr = new leaf_node(order);
        ptr->insert(key,r);
        root = sqt = ptr;
        return;
    }
    node *ptr = search_node(key); // 查找关键字应该所在的叶子结点
    leaf_node *q = static_cast<leaf_node*>(ptr);    // 转换成叶子结点
    q->insert(key,r);   // 在叶子结点进行插入操作
    check1(ptr);     // 坚持刚刚插入的叶子结点
    return;
}



/* 检查结点的关键字个数是否超过限制 */
void bp_tree::check1(node *ptr)
{
    // 插入后处理
    while(ptr)
    {
        if(ptr->key_num <= order - 1)
        {
            break;
        }
        ElemType up;
        int k,start,len;
        if(ptr->leaf)   // 如果是叶子结点
        {
            up = ptr->keys[order/2 + 1];
        }else{
            up = ptr->keys[(int)ceil((double)order/2)];
        }
        // k处关键字是前面结点的最后一个，k+1是后面结点的第一个
        node *qtr = ptr->split();
        if(!ptr->parent)
        {
            inner_node *father = new inner_node(order);
            father->chd[0] = ptr;
            father->keys[1] = up;
            father->chd[1] = qtr;
            father->key_num++;
            ptr->parent = father;
            qtr->parent = father;
            root = father;
        }else{
            inner_node * father = static_cast<inner_node*>(ptr->parent);
            qtr->parent = father;
            father->insert(qtr,up);
        }
        ptr = ptr->parent;
    }
    return;
}

/* 从B+树中删除记录r */
void bp_tree::drop(Record *r)
{
    ElemType key = *r;  // 得到待删除记录中的关键字
    drop(key);
    return;
}

/* 从B+树中删除关键字key和关联的记录指针 */
void bp_tree::drop(ElemType key)
{
    if(root == sqt && root == nullptr)
    {
        std::cout << "B+树当前为空，删除失败" << std::endl;
        return;
    }
    node *ptr = search_node(key); // 查找关键字应该所在的叶子结点
    leaf_node *q = static_cast<leaf_node*>(ptr);    // 转换成叶子结点
    q->drop(key);
    check2(ptr);
    return;
}

void bp_tree::check2(node *ptr)
{
    
    while(ptr)
    {

    }
}




/* 在叶子结点中查找记录并且返回关键字位置 */
// int leaf_node::search2(ElemType key)
// {
//     // 在叶子结点中查找位置
//     int i = binary_search(key);
//     // 没有找到该关键字
//     if((i == 0) || (i > 0 && keys[i] != key))
//         return 0;
//     return i;
// }
