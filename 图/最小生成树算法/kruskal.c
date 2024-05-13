
typedef char VertexType;
typedef int  EdgeType;
#define MAXVEX 100
#define INFINITY 65535

/* 用邻接矩阵存储无向网 */
typedef struct
{
    VertexType vexs[MAXVEX];
    EdgeType arc[MAXVEX][MAXVEX];
    int numVertexs,numEdges;
}MGraph;

typedef struct
{
    int begin;      //边开始的顶点位置
    int end;        //边结尾的顶点位置
    int weight;     //边上的权值
}Edge;


// 在Same数组中每一连通分量只有一个出口，即肯定存在某个Same[i] = 0，但是有多个入口
// Find函数是为每一个入口寻找出口
// 比如为edges[i].begin和edges[i].end这两个入口寻找出口，得到出口分别为m和n
// 如果m和n相等，则出口相同，说明在同一个连通分量
// 如果m和n不相等，则不在同一个连通分量
int Find(int *Same,int f){
    while(Same[f] > 0)
        f = Same[f];
    return f;
}

void convert(MGraph *G,Edge *edges)
{
    /* 初始化edges数组 */
    for (int i = 0; i < G->numEdges; i++)
    {
        edges[i].begin = 0;
        edges[i].end = 0;
        edges[i].weight = 0;
    }

    /* 将邻接矩阵转换为edges数组，并按边的权重排序 */
    for (int i = 0; i < G->numVertexs; i++)
    {
        for (int j = 0; j < G->numVertexs; j++)
        {
            if (G->arc[i][j] != 0 && G->arc[i][j] != INFINITY)
            {
                edges[i].begin = i;
                edges[i].end = j;
                edges[i].weight = G->arc[i][j];
            }
        }
    }

    /* 对edges数组按边的权重进行排序 */
    for (int i = 0; i < G->numEdges - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < G->numEdges; j++)
        {
            if (edges[j].weight < edges[minIndex].weight)
            {
                minIndex = j;
            }
        }
        if (minIndex != i)
        {
            Edge temp = edges[i];
            edges[i] = edges[minIndex];
            edges[minIndex] = temp;
        }
    }
}

void MiniSpan_Kruskal(MGraph G)
{
    Edge *edges = malloc(sizeof(Edge) * G.numEdges);    // 共有G.numEdges条边
    convert(&G,edges);
    int *Same = malloc(sizeof(int) * G.numVertexs);  // 共有G.numVertexs个顶点
    for(int i = 0; i < G.numVertexs; i++)
        Same[i] = 0;          // 将Same数组初始化为0,在开始之前，各个单独的顶点都是一个独立的联通分量

    for(int i = 0; i < G.numEdges; i++)
    {   
        //通过比较m是否与n相等 判断是否begin和end是否已经属于同一个连通分量
        int m = Find(Same, edges[i].begin);
        int n = Find(Same, edges[i].end);
        if(m != n)
        {
            //如果不属于同一个连通分量，将各自所属的连通分量合并成一个连通分量
            Same[m] = n;
            printf("将代价为%d的(%d,%d)加入生成树",edges[i].weight,edges[i].begin,edges[i].end);     
        }
    }
}
