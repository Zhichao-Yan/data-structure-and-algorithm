#include "B+.hpp"

int node::binary_search(ElemType target)
{
    ElemType *keys = this->keys;
    int left = 1, right = this->key_num;
    while (left <= right)
    {
        int mid = (left + right) / 2;
        if (keys[mid] == target)
        {
            return mid; // 找到目标值，返回其索引
        }
        else if (keys[mid] < target)
        {
            left = mid + 1; // 目标值在右半区间，更新左边界
        }
        else
        {
            right = mid - 1; // 目标值在左半区间，更新右边界
        }
    }
    // right < left
    return right; // 始终返回right
}

/* 向索引结点中插入关键字和结点指针 */
void inner_node::insert(ElemType key, node *ptr)
{
    int i = binary_search(key) + 1;
    insert(key, ptr, i);
    return;
}

/* 插入关键字和子结点指针到索引结点i位置 */
void inner_node::insert(ElemType key, node *ptr, int i)
{
    for (int k = key_num; k >= i; --k)
    {
        keys[k + 1] = keys[k];
        chd[k + 1] = chd[k];
    }
    keys[i] = key;
    chd[i] = ptr;
    ptr->parent = this;
    ++key_num;
    return;
}

/* 删除索引结点位置i处的关键字和记录 */
void inner_node::drop(unsigned int i)
{
    // 为了删除key和对应的记录，后面的数据从后往前移动
    for (int j = i + 1; j <= key_num; ++j)
    {
        keys[j - 1] = keys[j];
        chd[j - 1] = chd[j];
    }
    keys[key_num] = 0;
    chd[key_num] = NULL;
    --key_num;
}

/* 裂解内部索引结点 */
inner_node *inner_node::split()
{
    // 内部结点分裂后的新的内部索引结点
    inner_node *ptr = new inner_node(od);
    int k = (int)ceil((double)od / 2);
    int start = k;
    int len = od - k + 1;

    for (int i = 0; i < len; i++)
    {
        ptr->keys[i] = keys[start + i];
        ptr->chd[i] = chd[start + i];
        chd[start + i] = nullptr;
        keys[start + i] = 0;
        // 子结点的父结点应该执行新的内部索引结点
        ptr->chd[i]->parent = ptr;
        ptr->key_num++;
        key_num--;
    }
    // ptr->keys[0]之后是要上移的关键字，所以不算在ptr的关键字之内
    ptr->key_num--;
    return ptr;
}

/* 把右边索引结点合并到当前索引结点中 */
void inner_node::merge(node *ptr)
{
    inner_node *inner = static_cast<inner_node *>(ptr);
    for (int k = 0; k <= inner->key_num; ++k)
    {
        insert(inner->keys[k], inner->chd[k], key_num + 1);
        // 合并过来的子结点的父结点要调整
        inner->chd[k]->parent = this;
    }
    delete inner; // 释放结点ptr
    return;
}

/* 从相邻的内部索引结点借 */
void inner_node::borrow(node *ptr)
{
    // 将被借的结点转换成内部索引结点
    inner_node *p = static_cast<inner_node *>(ptr);
    // 被借结点p是左兄弟索引结点
    if (p->keys[p->key_num] < keys[key_num])
    {
        // 获得左兄弟的最大关键字和子结点指针
        ElemType key = p->keys[p->key_num];
        node *qtr = p->chd[p->key_num];
        // 删除左兄弟的最大关键字和子结点指针
        p->drop(ptr->key_num);
        // 插入到索引结点ptr的0位置
        insert(key, qtr, 0);
        return;
    }
    // 被借结点p是右兄弟索引结点
    if (p->keys[p->key_num] > keys[key_num])
    {
        // 获得右兄弟的最小关键字和子结点指针
        ElemType key = p->keys[0];
        node *qtr = p->chd[0];
        // 删除右兄弟的最小关键字和子结点指针
        p->drop(0);
        // 插入到索引结点ptr的末尾位置
        insert(key, qtr, key_num + 1);
        return;
    }
    return;
}

/* 在叶子结点中插入记录 */
void leaf_node::insert(Record *r)
{
    ElemType key = *r; // 得到记录中的关键字
    insert(key, r);    // 调用重载函数
    return;
}

void leaf_node::insert(ElemType key, Record *r, int i)
{
    for (int k = key_num; k >= i; --k)
    {
        keys[k + 1] = keys[k];
        data[k + 1] = data[k];
    }
    keys[i] = key;
    data[i] = r;
    ++key_num;
    return;
}

/* 往叶子结点中插入关键字和记录，关键字无法由记录获取的情况 */
void leaf_node::insert(ElemType key, Record *r)
{
    int i = binary_search(key); // 查找可能的位置
    if (i > 0 && keys[i] == key)
    {
        std::cout << "关键字" << key << "对应的记录已经存在" << std::endl;
        return;
    }
    ++i; // 获得插入位置
    // 将关键字和记录指针插入在i位置
    insert(key, r, i);
    return;
}

/* 删除位置i处的关键字和记录 */
void leaf_node::drop(unsigned int i)
{
    // 为了删除key和对应的记录，后面的数据从后往前移动
    for (int j = i + 1; j <= key_num; ++j)
    {
        keys[j - 1] = keys[j];
        data[j - 1] = data[j];
    }
    keys[key_num] = 0;
    data[key_num] = NULL;
    --key_num;
}

/* 从叶子结点中删除记录r */
void leaf_node::drop(Record *r)
{
    ElemType key = *r; // 得到记录中的关键字
    drop(key);
    return;
}

/* 从叶子结点中删除key关联的记录 */
void leaf_node::drop(ElemType key)
{
    unsigned int i = binary_search(key); // 查找可能的位置
    // key关键字不在叶子结点中
    if (i == 0 || (i > 0 && keys[i] != key))
    {
        std::cout << "叶子结点中不存在该关键字和对应的记录！" << std::endl;
        return;
    }
    // i是关键字key在叶子结点中的位置
    drop(i);
    return;
}

/* 在叶子结点中查找记录并且返回记录指针 */
Record *leaf_node::search(ElemType key)
{
    // 在叶子结点中查找位置
    int i = binary_search(key);
    // 没有找到该关键字
    if ((i == 0) || (i > 0 && keys[i] != key))
        return nullptr;
    return data[i];
}

/* 裂解叶子结点 */
leaf_node *leaf_node::split()
{
    leaf_node *ptr = new leaf_node(od);
    int k = od / 2;
    int len = od - k;
    int start = k + 1;
    for (int i = 0; i < len; i++)
    {
        ptr->data[1 + i] = data[start + i];
        ptr->keys[1 + i] = keys[start + i];
        data[start + i] = nullptr;
        keys[start + i] = 0;
        ptr->key_num++;
        key_num--;
    }
    // 链接新的叶子结点
    ptr->next = next;
    next = ptr;
    return ptr;
}

/* 把结点ptr的关键字和对应记录指针合并到当前结点中 */
void leaf_node::merge(node *ptr)
{
    leaf_node *leaf = static_cast<leaf_node *>(ptr);
    for (int k = 1; k <= ptr->key_num; ++k)
    {
        insert(leaf->keys[k], leaf->data[k], key_num + 1);
    }
    // 叶子结点合并后需保存链表的连续
    next = leaf->next;
    delete leaf; // 释放结点ptr
    return;
}

/* 从相邻的叶子结点ptr借 */
void leaf_node::borrow(node *ptr)
{
    // p是被借叶子结点，this是主借叶子结点
    leaf_node *p = static_cast<leaf_node *>(ptr);

    // p是右兄弟，需要从右边兄弟借最小关键字和记录指针给左边
    if (p->keys[1] > keys[1])
    {
        // 获取右兄弟的最大关键字和其记录指针
        ElemType key = p->keys[1];
        Record *r = p->data[1];
        // 删除右兄弟最小关键字和记录指针
        p->drop((unsigned int)1);
        // 插入到主借结点的尾部
        insert(key, r, key_num + 1);
        return;
    }
    // p是左兄弟，从左边兄弟借最大关键字和记录指针给右边
    if (p->keys[1] < keys[1])
    {
        // 获取左兄弟的最大关键字和其记录指针
        ElemType key = p->keys[p->key_num];
        Record *r = p->data[p->key_num];
        // 删除左兄弟最大关键字和记录指针
        p->drop((unsigned int)p->key_num);
        // 插入到主借结点的位置1处
        insert(key, r, 1);
        return;
    }
    return;
}

/* 查找关键字可能存在的叶子结点 */
node *bp_tree::search_node(ElemType target)
{
    node *ptr = root;
    while (ptr && !ptr->leaf)
    {
        int i = ptr->binary_search(target);
        inner_node *p = static_cast<inner_node *>(ptr);
        ptr = p->chd[i];
    }
    return ptr;
}

/* 在B+树中根据关键字查找记录所在指针 */
Record *bp_tree::search(ElemType target)
{
    // 找到关键字所在的叶子结点
    node *ptr = search_node(target);
    // 转换成叶子结点
    leaf_node *p = static_cast<leaf_node *>(ptr);
    // 在叶子结点中根据关键字查找记录
    return p->search(target);
}

/* 在B+树中插入记录 */
void bp_tree::insert(Record *r)
{
    ElemType key = *r; // 得到待插入记录中的关键字
    insert(key, r);
    return;
}

/* 在B+树中插入关键字和其关联的记录指针 */
void bp_tree::insert(ElemType key, Record *r)
{
    if (root == sqt && root == nullptr)
    {
        leaf_node *ptr = new leaf_node(order);
        ptr->insert(key, r);
        root = sqt = ptr;
        return;
    }
    node *ptr = search_node(key);                 // 查找关键字应该所在的叶子结点
    leaf_node *q = static_cast<leaf_node *>(ptr); // 转换成叶子结点
    q->insert(key, r);                            // 在叶子结点进行插入操作
    check1(ptr);                                  // 坚持刚刚插入的叶子结点
    return;
}

/* 从B+树中删除记录r */
void bp_tree::drop(Record *r)
{
    ElemType key = *r; // 得到待删除记录中的关键字
    drop(key);
    return;
}

/* 从B+树中删除关键字key和关联的记录指针 */
void bp_tree::drop(ElemType key)
{
    if (root == sqt && root == nullptr)
    {
        std::cout << "B+树当前为空，删除失败" << std::endl;
        return;
    }
    node *ptr = search_node(key);                 // 查找关键字应该所在的叶子结点
    leaf_node *q = static_cast<leaf_node *>(ptr); // 转换成叶子结点
    q->drop(key);                                 // 从叶子结点尝试删除关键字
    check2(ptr);                                  // 检查该结点是否满足要求
    return;
}

/* 检查结点的关键字个数是否超过限制 */
void bp_tree::check1(node *ptr)
{
    while (ptr) // 插入后处理
    {
        if (ptr->key_num <= order - 1) // 不超过上限，直接退出
        {
            break;
        }
        node *qtr = ptr->split(); // 裂解结点
        ElemType up;              // 需要上移到父结点的关键字
        if (qtr->leaf)            // 如果qtr是叶子结点
        {
            up = qtr->keys[1];
        }
        else
        { // 如果qtr是内部索引结点
            up = qtr->keys[0];
            qtr->keys[0] = 0;
        }
        if (!ptr->parent) // 父结点不存在
        {
            inner_node *father = new inner_node(order); // 生成一个父结点（内部索引结点）
            father->chd[0] = ptr;                       // 插入ptr作为第一个子结点指针
            ptr->parent = father;                       // 确定ptr和father的父子关系
            father->insert(up, qtr, 1);                 // 插入qtr到位置1处
            root = father;                              // 更新B+树的根结点
        }
        else
        {
            inner_node *father = static_cast<inner_node *>(ptr->parent); // 父结点存在，则获取父结点
            father->insert(up, qtr);                                     // 插入up和qtr到父结点中
        }
        ptr = ptr->parent;
    }
    return;
}

/* 检查结点的关键字个数是否少于限制 */
void bp_tree::check2(node *ptr)
{
    int bottom = (int)ceil((double)order / 2) - 1;
    while (ptr)
    {
        if (ptr->key_num >= bottom)
            break;
        inner_node *father = static_cast<inner_node *>(ptr->parent); // 获得其父结点
        if (father)
        {
            ElemType key = ptr->keys[1];        // 获得该结点的一个关键字，用于查找ptr在father中的位置，方便查找兄弟结点
            int i = father->binary_search(key); // 获得子结点在其中的位置
            // 获得可能的左右兄弟结点
            node *left = nullptr, *right = nullptr;
            if (i != 0)
                left = father->chd[i - 1];
            if (i != father->key_num)
                right = father->chd[i + 1];

            if (left && left->key_num > bottom)
            {
                // 需要从左兄弟结点借
                // ptr本身也是叶子结点，左兄弟也是叶子结点
                if (ptr->leaf)
                {
                    // 从左兄弟left借得关键字和记录后
                    // ptr的最小关键字被更新
                    ptr->borrow(left);
                    father->keys[i] = ptr->keys[1];
                }
                else
                {
                    // 父结点关键字下移
                    ptr->keys[0] = father->keys[i];
                    ptr->borrow(left);
                    // 上移
                    father->keys[i] = ptr->keys[0];
                    ptr->keys[0] = 0;
                }
                break;
            }
            else if (right && right->key_num > bottom) // 使用右兄弟的最小关键字补充ptr，使ptr满足数量条件
            {
                // 从右兄弟借
                // ptr本身也是叶子结点，右兄弟也是叶子结点
                if (ptr->leaf)
                {
                    // 右兄弟right最小关键字和记录被借走后
                    // 右兄弟最小关键字被更新
                    ptr->borrow(right);
                    father->keys[i + 1] = right->keys[1];
                }
                else
                {
                    // 父结点关键字下移
                    right->keys[0] = father->keys[i + 1];
                    ptr->borrow(right);
                    // 将右兄弟最小关键字上移动
                    father->keys[i + 1] = right->keys[0];
                    right->keys[0] = 0;
                }
                break;
            }
            else if (left && left->key_num == bottom)
            {
                // 如果是内部索引结点的合并，操作有所区别
                if (!ptr->leaf)
                    ptr->keys[0] = father->keys[i]; // 将父结点中对应的关键字下放到0位置，一起合并
                left->merge(ptr);
                father->drop(i);
                ptr = father;
            }
            else if (right && right->key_num == bottom)
            {
                // 如果是内部索引结点的合并，操作有所区别
                if (!right->leaf)
                    right->keys[0] = father->keys[i + 1]; // 将父结点中对应的关键字下放到0位置，一起合并
                ptr->merge(right);
                father->drop(i + 1);
                ptr = father;
            }
        }
        else
        {
            // 没有父结点，所以ptr指向根结点
            // 根结点必须最少有一个关键字
            if (ptr->key_num < 1)
            {
                // 如果它还是一个叶子结点,也是根结点，因此它是B+树中最后一个结点
                if (ptr->leaf)
                {
                    root = nullptr;
                    sqt = nullptr;
                }
                else
                {
                    // 它是一个内部索引结点
                    // 删除根结点，要变更B+树的root指针变量
                    inner_node *p = static_cast<inner_node *>(ptr);
                    root = p->chd[0];
                }
                delete ptr; // 释放原来的根结点
                ptr = nullptr;
                break;
            }
            else
                break; // 可以直接退出，不再向上操作
        }
    }
}

/* 顺序遍历叶子结点链表 */
void bp_tree::traverse()
{
    if (sqt == nullptr)
        return;
    leaf_node *ptr = sqt;
    while (ptr)
    {
        for (int i = 1; i <= ptr->key_num; ++i)
        {
            std::cout << ptr->keys[i] << '|';
        }
        ptr = ptr->next;
        if (ptr)
            std::cout << "->";
    }
    std::cout << std::endl;
    return;
}
