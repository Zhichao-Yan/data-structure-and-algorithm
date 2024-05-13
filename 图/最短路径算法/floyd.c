#define MAXVEX 100
#define INFINITY 65535


typedef char VertexType;
typedef int  EdgeType; 
typedef int Pathmatrix[MAXVEX][MAXVEX];
typedef int ShortPathTable[MAXVEX][MAXVEX];

/* 用邻接矩阵存储无向网 */
typedef struct
{
    VertexType vexs[MAXVEX];
    EdgeType arc[MAXVEX][MAXVEX];   // 边权重数组，表示顶点之间的边上的权值，无穷大小表示顶点之间不存在遍
    int numVertexs,numEdges;        // 顶点数目和边数目
}MGraph;


//这代码是真的简洁优美
/**
 * @description: 求解图中每一对顶点之间的最短路径
 * @param {MGraph} G
 * @param {Pathmatrix} *P
 * @param {ShortPathTable} *D
 * @return {*}
 */
void Floyd(MGraph G,Pathmatrix *P,ShortPathTable *D)
{   //初始化矩阵D和P
    for(int v=0;v<G.numVertexs;v++)
    {
        for(int w=0;w<G.numVertexs;w++)
        {
            (*D)[v][w]=G.arc[v][w];//初始时就是邻接矩阵
            (*P)[v][w]=w;//无中间结点，有v到w的下一落脚顶点就是w
        }
    }
    for(int k=0;k<G.numVertexs;++k)//外层循环，寻找所有顶点经过顶点k到其他顶点的最短路径，更新D和P
    {   
        for(int v=0;v<G.numVertexs;++v)
        {
            for(int w=0;w<G.numVertexs;++w)
            {
                if( (*D)[v][w]>(*D)[v][k]+(*D)[k][w] )
                {
                    (*D)[v][w]=(*D)[v][k]+(*D)[k][w];//如果经过顶点k距离更小，则更新权值
                    (*P)[v][w]=(*P)[v][k];//将由v到k的最短路径纳入v到w的最短路径
                }
            }
        }
    }
    return;
}