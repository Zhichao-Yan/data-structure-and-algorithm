#define MAXVEX 100
#define OK 1
#define ERROR -1

typedef int Status;
typedef struct EdgeNode
{
    int adjvex;
    int weight;
    struct EdgeNode *next;
} EdgeNode; // 边结点(带有权值)

typedef struct VertexNode
{
    int in;                    // 该顶点入度
    int data;                  // 顶点域
    EdgeNode *firstedge;       // 边表头指针
} VertexNode, AdjList[MAXVEX]; // 顶点表结点

typedef struct
{
    AdjList adjList;
    int numVertexes, numEdges; // 当前图的顶点数和边数
} graphAdjList, *GraphAdjList;

int *etv, *ltv; // 事件发生的最早时间和最迟发生时间数组
int *stack2;    // 存储拓扑序列
int top2;       // stack2的栈顶指针

// 这里使用栈存储入度为0的顶点，避免每个查找都要遍历顶点表找有没有入度为0的顶点
Status TopologicalSort(GraphAdjList G)
{
    EdgeNode *e;
    int count = 0; // 统计输出顶点个数
    int top = 0;   // 栈指针下标
    int *stack;
    stack = (int *)malloc(G->numVertexes * sizeof(int));
    for (int i = 0; i < G->numVertexes; i++)
        if (G->adjList[i].in == 0)
            stack[++top] = i; // 将入度为0的顶点入栈
    etv = (int *)malloc(G->numVertexes * sizeof(int));
    for (int i = 0; i < G->numVertexes; i++)
        etv[i] = 0; // 初始化为0
    int top2 = 0;   // 栈顶指针
    stack2 = (int *)malloc(G->numVertexes * sizeof(int));
    while (top != 0)
    {
        int gettop = stack[top--]; // 去stack中入度为0的顶点
        count++;                   // 统计顶点个数，用于最后判断有向图是否有环
        stack2[++top2] = gettop;   // 将从stack中出来的入度为0的顶点入stack2，stack2中存放拓扑序列
        for (e = G->adjList[gettop].firstedge; e; e = e->next)
        {
            int k = e->adjvex;
            if (!(--G->adjList[k].in))              // 先自减，然后判断减完是否等于0
                stack[++top] = k;                   // 变成入度为0的顶点需要入栈
            if ((etv[gettop] + e->weight) > etv[k]) // 求各顶点事件最早发生时间
                etv[k] = etv[gettop] + e->weight;
        }
    }
    if (count < G->numVertexes)
        return ERROR; // 出错，有环
    else
        return OK; // 存在拓扑序列，并存在Stack2中
}

void CriticalPath(GraphAdjList G)
{
    EdgeNode *e;
    int ete, lte;                                      // 相应活动的最早发生时间和最迟发生时间变量
    TopologicalSort(G);                                // 先进行拓扑排序
    ltv = (int *)malloc(G->numVertexes * sizeof(int)); // 事件最晚发生的时间
    for (int i = 0; i < G->numVertexes; i++)           // 对ltv初始化
        ltv[i] = etv[G->numVertexes - 1];              // 为啥初始化成etv[G->numVertexes-1]？
    // 因为根据之前得到拓扑排序得到终点的最早的发生时间，终点事件的最早发生时间等于最晚发生时间
    // 为啥呢？既然都到终点了，还分早晚?这个终点的最早发生时间意味着无论如何前面的活动都已经完成
    // 此刻已经无需等待,最晚时间没有意义，它之前顶点的最晚时间必须小于终点的最晚发生时间
    while (top2 != 0) // 拓扑序列的逆序，我们需要从终点倒推出每个事件的最晚发生时间
    {
        int gettop = stack2[top2--]; // 拓扑序列倒序出栈
        for (e = G->adjList[gettop].firstedge; e; e = e->next)
        {
            int k = e->adjvex;                    // k是gettop的领接顶点,有弧<gettop,k>
            if (ltv[k] - e->weight < ltv[gettop]) // 已知顶点k的最晚发生时间
                ltv[gettop] = ltv[k] - e->weight; // 求gettop顶点的最晚发生时间
        } // 已知某顶点后序领接顶点事件的最晚发生时间，求该顶点最晚发生时间。见正文分析
    }
    for (int j = 0; j < G->numVertexes; j++)
    { // 对每一条弧表示的活动，求活动的最早开始时间，和活动最晚发生时间
        for (e = G->adjList[j].firstedge; e; e = e->next)
        {
            int k = e->adjvex;        // k是顶点j的后序领接顶点，即存在弧<j,k>表示一个活动
            ete = etv[j];             // 活动的最早开始时间就等于顶点j表示事件的最早发生时间，
            lte = ltv[k] - e->weight; // 活动的最晚发生时间就等于顶点k最晚发生时间减活动所需时间
            if (ete == lte)
            {                                  // 两者相等则此活动没有任何空闲，其在关键路径上，且为关键活动
                printf("<V%d,V%d> length:%d,", // 打印输出关键路径
                       G->adjList[j].data, G->adjList[k].data, e->weight);
            }
        }
    }
}