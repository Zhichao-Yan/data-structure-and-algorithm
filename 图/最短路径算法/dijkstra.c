
#define MAXVEX 100
#define INFINITY 65535

typedef char VertexType;
typedef int  EdgeType; 
typedef int **Path;
typedef int Table;


/* 用邻接矩阵存储无向网 */
typedef struct
{
    VertexType vexs[MAXVEX];
    EdgeType arc[MAXVEX][MAXVEX];   // 边权重数组，表示顶点之间的边上的权值，无穷大小表示顶点之间不存在遍
    int numVertexs,numEdges;        // 顶点数目和边数目
}MGraph;


/**
 * @description: 求单个源点v0到其他点的最短距离和路径
 * @param {MGraph} G 
 * @param {int} v0 单个源点
 * @param {Patharc} *P   P[v][w]为TRUE，则w是v0到v路径上的点
 * @param {ShortPathTable} *D   D[v]为v0到v的最短带权路径长度
 * @return {*}
 */
void dijkstra(MGraph G,int v0, Path P,Table *D)
{
    // final代表S集，S集为已经求得最短路径的终点的集合
    int final[MAXVEX];
    for(int v = 0; v < G.numVertexs; ++v)
    {
        // 默认置空S集，当前没有任何顶点处在S中
        final[v]  = 0;
        // 先默认置空v0到v上的路径，即默认不存在路径
        for(int w = 0; w < G.numVertexs; ++w)
            P[v][w] = 0;
        // 获取v0到v的权值
        D[v] = G.arc[v0][v];
        // 如果D[v]存在且不为无穷，则说明必定存在v0到v的路径
        if(D[v] < INFINITY)
        {
            P[v][v0] = 1;
            P[v][v] = 1;
        }
    }
    D[v0] = 0;  // 求得v0到v0的最短路径是0
    final[v0]  = 1; // 将v0放入S集中
    // 开始主循环，求得v0到剩余G.numVertexs - 1个顶点的最短路径及其权值
    for(int i = 1; i < G.numVertexs; ++i)
    {
        int min = INFINITY;
        int k = 0;
        // 找到当前v0到各个顶点w中路径权值和最小的那个顶点k
        for(int w = 0; w < G.numVertexs; ++w)
        {
            // w顶点不在S中
            if(final[w] == 0)
            {
                if(D[w] < min)
                {
                    min = D[w];
                    k = w;  
                }
            }
        }
        final[k] = 1; // 将顶点k加入S集合
        // 根据最短距离和路径，更新可能的次短路径及其距离
        for(int w = 0; w < G.numVertexs; ++w)
        {
            if(final[w] == 0 && (min + G.arc[k][w]) < D[w])
            {
                // 更新可能的次短距离
                D[w] = min + G.arc[k][w];
                // 更新可能的次短路径
                // v0到w的路径包含v0到k的路径
                P[w] = P[k];
                // v0到w的路径存在，那么顶点w在对应的路径中
                P[w][w] = 1;
            }
        }
    }
}