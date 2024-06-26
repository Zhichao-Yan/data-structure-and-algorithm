# B+树
* 一种B-树的变型树
* 一棵平衡排序树
* 多路查找树
* 可以随机查找也可以顺序查找
* 为了磁盘或其它存储设备而设计的


### 两种定义
第一种定义：关键字个数和孩子结点个数相同
* 结点最多有m个关键字，最多有m个子树
* 结点最少有$\lceil \frac{m}{2} \rceil$个关键字，最少有$\lceil \frac{m}{2} \rceil$棵子树
* 叶子结点含有m+1个关键字时进行分裂：左结点得到前$\lceil \frac{m+1}{2} \rceil$个关键字，右结点得到剩余关键字


第二种定义：关键字个数比孩子结点个数小1
* 结点（包括叶子结点）最多有m-1个关键字，最多有m个子树
* 结点最少有$\lceil \frac{m}{2} \rceil$ - 1个关键字，最少有$\lceil \frac{m}{2} \rceil$个棵子树
* 叶子结点含有m个关键字时进行分裂：左结点得到前$\lceil \frac{m}{2} \rceil$个关键字，右结点得到剩余关键字





公共特点：
* 包含2种类型的结点：内部结点（也称索引结点）和叶子结点
* 内部结点不保存数据，所有的非终端节点（内部结点）可以看成是叶子结点的索引部分（稀疏索引）
* 非终端结点关键字是其子树中的最大或最小关键字
* 叶子节点相当于是存储（关键字）数据的数据层
* 叶子节点内按关键字顺序排序
* 叶子结点之间依关键字的大小自小而大顺序油指针链接
* 叶子节点中包含了全部关键字的信息，及指向含这些关键字记录的指针
* 每个叶子结点都存有相邻叶子结点的指针
* 根结点既可能是内部结点，也可能是叶子结点。在只有一层单个结点时作为叶子结点，在有很多层的时候作为内部结点。
* 根结点关键字个数至少为1个
* 关键字左树中的所有key都小于它，右子树中的key都大于等于它



### 和B树区别
1. B+树与B树最大的不同是内部结点不保存数据，只用于索引，所有数据（或者说记录）都保存在叶子结点中。相比之下，B树中保存记录，每一个结点都是保存数据的结点。
2. B+树不管查找成功与否，每次查找都是走了一条从根到叶子节点的路径，不可能在非叶子节点命中。相比之下，B树会在非叶子结点命中。
3. B+树可以在叶子结点层从最小关键字起顺序查找

### 使用
通常用于数据库和操作系统的文件系统中，作为元数据索引


### 为什么用b/b+树
数据库如MySQL的数据一般放在磁盘中，而读盘的过程包括两次机械运动盘片旋转和磁头移动，这一过程比较耗时，所以对数据的访问性能受限于访问磁盘的次数。
b/b+树相对于其他搜索树如平衡二叉树和红黑树能够明显减少索引次数，b/b+树的特点是单个节点能够保存很多键，所以保存相同数量的的键和值内容时，b系树的高度要明显小于二叉查找树，所以b系树访问磁盘的次数也要更小。

### 为什么b+树比b树更适合做数据库索引
* b+树的磁盘读写代价更低。
如果把所有同一内部节点的关键字存放在同一盘块中，那么b+树盘块所能容纳的关键字数量也越多，一次性读入内存的需要查找的关键字也就越多，相对IO读写次数就降低了。相比b树耗费时间更少
* b+树的叶节点通过链表进行链接，所以在遍历数据时更加方便。相比之下，B树需要中序遍历才能实现,其并没有解决元素遍历的效率低下的问题
* b+树的叶节点通过链表链接，所以可以很容易进行范围查询。在数据库中基于范围的查询是非常频繁的，而B树不支持这样的操作或者说效率太低。
* b+树查询更加稳定。因为只有叶节点保存值，所以每次查询的路径长度相同

### b+树和b树查找
* 共同点：先找到节点之后然后在对有序数组进行二分查找，根据查找的结果确定子节点的链接
* 不同点：b树的查找，查找到的每个节点都可能是要找的节点，而b+树一定要查找到叶节点

### b+树和b树删除
b/b+树的非根节点都需要满足节点数不少于ceil(M/2)-1个，在删除的过程中节点会因为关键字太少而需要调整
步骤
1. 找到关键字所在的结点进行删除
    * 在B树中，关键字所在结点可能不是终端结点，使用替代策略，删除终端结点来替代
    * 在B+树中，关键字所在结点都是叶子结点
2. 结点关键字满足数量条件则删除结束
3. 不满足数量条件
    * 如果兄弟不仅仅满足条件，还有多余关键字，则向兄弟借
    * 如果兄弟也刚刚满足条件，则和兄弟和父结点关键字一起合并
    * 父结点关键字减少，则跳转到1继续执行
区别
1. b+树不存在从内部节点删除键值对的问题，删除只能是从叶子节点开始的
2. b+树需要考虑叶节点和内部节点在borrow和merge时的操作有所不同
    * 进行叶节点的borrow或merge，要操作的对象包括key、value
    * 内部节点的borrow或merge则需要考虑key和child的问题
    * 在内部节点borrow和merge的过程中，都需要考虑child的parent的问题，要重新设置child的parent



### 参考
* [B树和B+树图解](https://www.cnblogs.com/nullzx/p/8729425.html)
* [b树和b+树实现](http://wxgg.cc/blogs/2019/b树和b+树实现.html)
* [B+树](https://www.cnblogs.com/wade-luffy/p/6292784.html)
* [B+树的几点总结](https://www.jianshu.com/p/6f68d3c118d6)
* [B+树的相关原理及实现](https://ivanzz1001.github.io/records/post/data-structure/2018/06/16/ds-bplustree#72-innodb)
* [为什么MySQL数据库索引选择使用B+树](https://www.cnblogs.com/tiancai/p/9024351.html)
* [MySQL索引背后的数据结构及算法原理](http://blog.codinglabs.org/articles/theory-of-mysql-index.html)
* [MySQL索引背后的数据结构及算法原理](http://blog.codinglabs.org/articles/theory-of-mysql-index.html)
