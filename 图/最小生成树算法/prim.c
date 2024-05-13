
typedef char VertexType;
typedef int  EdgeType;
#define MAXVEX 100
#define INFINITY 65535

/* 用邻接矩阵存储无向网 */
typedef struct
{
    VertexType vexs[MAXVEX];
    EdgeType arc[MAXVEX][MAXVEX];   // 边权重数组，表示顶点之间的边上的权值，无穷大小表示顶点之间不存在遍
    int numVertexs,numEdges;        // 顶点数目和边数目
}MGraph;


void MiniSpanTree_Prim(MGraph G)
{
    // 辅助数组
    int adjvex[MAXVEX];     
    int lowcost[MAXVEX];   
    // N = {V,{E}} 表示无向图G 
    // TE = {} 包含最小生成树中的边，初始为空
    // U = {} 包含逐渐加入生成树的顶点
    // lowcost[i]=0,代表此下标的顶点加入生成树，此处将顶点V0加入生成树U，进行初始化
    adjvex[0]=0;
    lowcost[0]=0;
    for(int i=1;i<G.numVertexs;i++)
    {
        lowcost[i]=G.arc[0][i];     // 将顶点Vi与顶点V0相连的边权值存入数组，如没有边，则存入无穷大
        adjvex[i]=0;                // 该对应边在最小生成树顶点集U中的连接顶点，此处为V0
    }
    // 依次将剩下的顶点加入最小生成树
    for(int i=1; i<G.numVertexs; i++)
    {
        // 从辅助数组中获取权值最小的边，以及其对应的暂时不在最小生成树中的顶点
        int min=INFINITY;   // 初始化最小权值为无穷大
        int k=0;
        for(int j=1; j<G.numVertexs; j++)//循环全部顶点
        {
            // lowcost[j] != 0 说明不在生成树中
            if(lowcost[j]!=0 && lowcost[j] < min)
            {
                min=lowcost[j];
                k=j;    //  标记权值最小的边
            }
        }
        printf("将顶点为%d和%d,权为%d的边加入生成树\n",k, adjvex[k],lowcost[k]);
        lowcost[k]=0;//加入生成树T
        // 对辅助数组进行更新，更新lowcost[]和adjvex[]
        for(int j=1; j < G.numVertexs; j++)
        {    
             if(lowcost[j] != 0 && G.arc[k][j] < lowcost[j])
             {  
                // 更新顶点Vj到最小生成树联通分量U最短距离的边权值以及对应顶点为Vk
                lowcost[j] = G.arc[k][j];
                adjvex[j] = k;
             }
        }
    }
   
}

