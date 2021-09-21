//
//  main.cpp
//  experiment
//
//  Created by Shaw Young on 2021/1/3.
//

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<ctype.h>
#define UNVISITED 0
#define VISITED 1
#define INFINITY 100000
#define VexType char
#define Status int
#define ERROR 0
#define OK 1

using  namespace std;
//邻接数组
typedef enum{DG = 0, UDG = 1, DN = 2, UDN = 3} GraphKind;
//图的4种类型：有向图，无向图，有向带权图，无向带权图
typedef struct{
    VexType *vexs;  //存储顶点
    int **arcs;     //边的关系存储
    int n, e;        //顶点，边数
    GraphKind kind; //图的类型
    int *tag;        //标志数组，图的遍历中使用
} MGraph;

typedef struct{
    //存储边的信息
    VexType v, w;   //边的两端
    int info;    //边的权值
}  ArcInfo;

//*****************************************队列
typedef int ElemType;
typedef struct LQNode{
    ElemType data;
    struct LQNode *next;
}LQNode, *QueuePtr;   //结点及其指针类型

typedef struct{
    VexType adjvex;
    int lowcost;
} prim,closedge;//Prim算法


typedef struct{
    QueuePtr front;//头指针
    QueuePtr rear;  //尾指针
}LQueue;

void InitQueue_LQ(LQueue &Q){ //初始化
    Q.front = Q.rear = (LQNode*)malloc(sizeof(LQNode));
    if(!Q.front) return;
    Q.front->next = NULL;
    return;
}


Status EnQueue_LQ(LQueue &Q, ElemType e){ //在队列Q的队尾插入元素e
    LQNode *p;
    p = (LQNode*)malloc(sizeof(LQNode));
    if(NULL == p)
        return ERROR;
    p->data = e;
    p->next = NULL;
    if(NULL == Q.front)
        Q.front=p;    //e插入空队列
    else
        Q.rear->next = p;            //e插入非空队列
    Q.rear = p;                       //队尾指针指向新的队尾
    return OK;
}

Status DeQueue_LQ(LQueue &Q, ElemType &e){
    LQNode *p;
    if(NULL == Q.front)
        return ERROR;
    p = Q.front;     //指向队头结点
    e = p->data;     //保存队头结点的数据
    Q.front = p->next;
    if(Q.rear == p)
        Q.rear = NULL;  //队列只有一个结点，删去后队列变空
    free(p);                  //释放被删头结点
    return OK;
}


//*****************************************队列

int LocateVex_M(MGraph G, VexType v ){
    /*
     *查找顶点坐标在图G顶点数组中的位序,v为顶点
     *return v的坐标
     */
    int i;
    for (i = 0; i < G.n; i++)
        if(G.vexs[i] == v)
          return i;
    return -1;
}

Status InitGraph_M(MGraph &G, GraphKind kind, VexType *vexs, int n){
    /*
     *初始化包含n个顶点和零条边的图（四种图都可以），kind为图的类型，vexs为顶点的信息，n为顶点个数
     *return 状态值，初始化含n个顶点且无边的kind类的图G
     */
    int i, j, info;
    if(n < 0 || (n > 0 && vexs == NULL))
        return ERROR;
    if(kind == DG || kind == UDG)
        info = 0;               //有向图，无向图权值为0
    else if(kind == DN || kind == UDN)
            info = INFINITY;  //有向带权图，无向带权图
        else
            return ERROR;
    G.n = n; G.e = 0; G.kind = kind;      //零条边
    if(0 == n)
        return OK;//空图
    if(NULL == (G.vexs = (VexType*)malloc(n*sizeof(VexType))))//初始化顶点数组
        return ERROR;
        //初始化顶点信息
    for(i = 0; i<G.n; i++)
        G.vexs[i] = vexs[i];  //建立顶点数组
    if(NULL == (G.arcs= (int **)malloc(n*sizeof(int*))))//初始化关系（边）数组
        return ERROR;
    for(i = 0; i < G.n; i++)  //分配每个指针所指向的数组
        if(NULL == (G.arcs[i] = (int *)malloc(n*sizeof(int))))
            return ERROR;
    if(NULL == (G.tag = (int *)malloc(n*sizeof(int))))
        return ERROR;
    for(i = 0; i < G.n; i++){ //初始化标记数组和关系数组
        G.tag[i] = UNVISITED;
        for(j = 0; j < G.n; j++)
          G.arcs[i][j] = info;
    }
    return OK;
}

Status CreateUDN_M(MGraph &G, VexType *vexs, int n, ArcInfo *arcs, int e){
    /*
     * 创建n个顶点，e条边的Kind类的图G，vexs为顶点的信息，arcs为边的信息（v,w）
     *return 状态值
     */
     int i, j, k;
    VexType v, w;
     if(InitGraph_M(G, UDN, vexs, n) != OK)
         return ERROR; //创建一个包含n个顶点，零条边的图G
     G.e = e; //边数
     for(k = 0; k < G.e; k++){    //创建e条边
         v = arcs[k].v;
         w = arcs[k].w; //读入边v,w
         i = LocateVex_M(G, v);
         j = LocateVex_M(G, w);  //确定v和w在顶点数组中的位序i和j
       if(i < 0 || j < 0)
           return ERROR;
       G.arcs[i][j] = G.arcs[j][i] = arcs[k].info;
     }
     return OK;
}

Status DestroyGraph_M(MGraph &G){
    //销毁图G
    int i;
    for(i = 0; i < G.n; i++){
        free(G.arcs[i]);
        G.arcs[i] = NULL;
    }
    free(G.arcs);
    G.arcs = NULL;
    free(G.vexs);
    G.vexs = NULL;
    G.n = G.e = 0;
    return  OK;
}

Status GetVex_M(MGraph G, int k, VexType &w){
    //取图G的k顶点的值到w
    if(k < 0 || k >= G.n)
        return ERROR;
    w = G.vexs[k];
    return OK;
}

Status PutVex_M(MGraph G, int k, VexType &w){
    //对图的k顶点赋值w
    if(k < 0 || k >= G.n)
        return ERROR;
    G.vexs[k] = w;
    return OK;
}

int FirstAdjvex(MGraph G, int k){
    //求图G中k顶点的第一个邻接顶点的位序
    if(k < 0 || k >= G.n)
        return -1;  //k顶点不存在
    int j = 0;
    for(; j < G.n; j++) //查找第一个值非0且非∞的元素
      if(G.arcs[k][j] != INFINITY)
          return j;
    return -2; //k顶点无邻接顶点
}

int NextAdjvex(MGraph G, int i, int j){
    //j为i顶点的邻接顶点，求图G中i顶点相对于j顶点的下一个邻接顶点的位序
    if(i < 0 || i >= G.n || j < 0 || j >= G.n)
        return -1;//i顶点或j顶点不存在
    if(G.arcs[i][j] == INFINITY)
        return -2;//j顶点不是i顶点的邻接顶点
    int p;
    for(p = j+1; p < G.n; p++)
       if(G.arcs[i][p] != INFINITY)
           return p;
    return -3; //图G中不存在i顶点相对于j顶点的下一个邻接顶点
}

Status Add_Arcs(MGraph &G, int k, int m, int info){
    //在图G中增加k顶点到m顶点的边或弧，若为带权图，info为权值，否则为1
    if(k < 0 || k >= G.n || m < 0 || m >= G.n)
        return ERROR;//k顶点或m顶点不存在
    if(G.arcs[k][m] != INFINITY && G.arcs[k][m] != 0)
        return ERROR;//k顶点到m顶点的边或弧已存在
    G.arcs[k][m] = info;
    if(G.kind == 1 || G.kind == 3)
        G.arcs[m][k] = info;
    G.e++;
    return OK;
}

Status RemoveArc_M(MGraph &G, int k, int m){
    //在图G中删除k顶点到m顶点的边或弧
    if(k < 0 || k >= G.n || m<0 ||m >= G.n)
        return ERROR;//k顶点或m顶点不存在
    if(G.arcs[k][m] == INFINITY)
        return ERROR;//k顶点到m顶点的边或弧不存在
    G.arcs[k][m] = G.arcs[m][k] = INFINITY;
    G.e--;
    return OK;
}

Status DFS_M(MGraph G, int i){
    //从连通图中的指定i顶点，先访问该顶点
    printf("%-4d%c\n", i, G.vexs[i]);
    G.tag[i] = VISITED;
    int w;
    //然后对v的所有邻接顶点wi依检次查
    for(w = FirstAdjvex(G,i); w >= 0; w = NextAdjvex(G,i,w)){
    //若wi未被遍历，则以wi为新起点递归进行深度优先遍历
      if(UNVISITED == G.tag[w])
        DFS_M(G,w);
    }
    return OK;
}

Status DFSTraverse_M(MGraph G){//深度优先遍历图G
    int i;
    for(i = 0; i < G.n; i++)
        G.tag[i]=UNVISITED;//初始化标志数组
    for(i = 0; i < G.n; i++)
      if(G.tag[i] == UNVISITED)  //若i顶点未访问，则以其为起点进行深度优先遍历
        if(ERROR == DFS_M(G,i))
            return ERROR;
    return OK;
}

Status isReachable(MGraph G, int i, int t){//是否存在i顶点到t顶点的路径
    Status found = ERROR;
    G.tag[i] = VISITED;
    if(i == t)
        return OK;
    int w;
    for(w = FirstAdjvex(G,i); found == ERROR && w >= 0; w = NextAdjvex(G,i,w)){
    //若wi未被遍历，则以wi为新起点递归进行深度优先遍历
      if(UNVISITED == G.tag[w])
        found = isReachable(G,w,t);
    }
    return found;
}

Status visit(MGraph &G, int i){  //访问i顶点
    printf("%-4d%c\n", i, G.vexs[i]);
    return OK;
}

Status BFSTraverse_M(MGraph G){//广度优先遍历
    int i,j,k;
    LQueue Q;
    InitQueue_LQ(Q);//初始化链队列Q
    for(i = 0; i < G.n; i++)
        G.tag[i] = UNVISITED;    //初始化标志数组
    for(i = 0;i < G.n; i++) //依次检查所有顶点
      if(UNVISITED == G.tag[i]){ //i顶点未被访问
        if(ERROR == visit(G,i))
            return ERROR;
        G.tag[i] = VISITED;
        EnQueue_LQ(Q,i);  //访问i顶点，并入队
        while(OK == DeQueue_LQ(Q,k)){  //出队元素到K
          for(j = FirstAdjvex(G,k); j >= 0; j = NextAdjvex(G,k,j))
            //依次判断k顶点的所有邻接顶点j，若未访问，则访问它，并入队
          
            if(UNVISITED == G.tag[j]){
                if(ERROR == visit(G,j))
                    return ERROR;
                G.tag[j] = VISITED;
                EnQueue_LQ(Q,j);
          }
        }
      }
    return OK;
}

int Degree(MGraph G, int i){
    //求i顶点的度
    if(i < 0 || i >= G.n) return -1;
    int j,s = 0;
    for(j = 0; j < G.n; j++){
      if(G.arcs[i][j] != INFINITY)
        s++;
      }
    return s;
}

void printGraphAdj(MGraph G) { //输出所有顶点的所有邻接顶点
    
    for(int i = 0; i<G.n; i++){
      int j = FirstAdjvex(G,i);
      if(j >= 0 && j < G.n)
          printf("%d的所有邻接顶点是 %d ",i,j);
      else
          if(j<0)
              return;
      int k = NextAdjvex(G,i,j);
      int p;
      while(k >= 0 && k < G.n){
        p = k;
        printf("%-4d", p);
        k=NextAdjvex(G, i, p);
      }
      printf("\n");
    }
}

void printGraph(MGraph G) {   //输出图G
    if(G.n == 0){
        printf("The graph is not existed.\n");
        return;
    }
    for(int i = -1; i < G.n ; ++i){
        if(i != -1)
            printf("%-4c",G.vexs[i]);
        for(int j = 0; j < G.n ; ++j){
            if(i == -1){
                if(j == 0)
                    printf("    ");
                printf("%-4c",G.vexs[j]);
            }
            else{
                if(G.arcs[i][j] != INFINITY)
                    printf("%-4d",G.arcs[i][j]);
                else
                    printf("∞   ");
            }
        }
        printf("\n");
    }
}

int minArc(MGraph &G, prim* closedge){//选择最小代价的边
    int min = INFINITY;
    int res = 0;
    for (int i = 0; i < G.n; i++){
        if(closedge[i].lowcost != 0 && closedge[i].lowcost < min){
            min = closedge[i].lowcost;
            res = i;
        }
            
    }
    return res;
}


int MiniSpanTree_Prim(MGraph &G, VexType V){
    prim closedge[G.n];
    int k = LocateVex_M(G, V);//确定第一个顶点的位置
    closedge[k].lowcost = 0;//并入U集
    for (int i = 0; i < G.n; i++)//初始化辅助数组
        if(i != k){
            closedge[i].adjvex = V;//存入起始结点
            closedge[i].lowcost = G.arcs[k][i];//邻接矩阵该行拷贝到辅助数组中
        }
    int minSum = 0;//最短路径之和
    for (int i = 0; i < G.n - 1; i++){//选择其余G.vexnum - 1个顶点
        int mincost = minArc(G, closedge);//选择最小代价的边
        printf("%c--%c  %d\n", closedge[mincost].adjvex, G.vexs[mincost], closedge[mincost].lowcost);//输出路径与权值
        minSum += closedge[mincost].lowcost;//最短路径之和
        closedge[mincost].lowcost = 0;//将mincost并入U集
        for (int j = 0; j < G.n; j++){
            if (G.arcs[mincost][j] < closedge[j].lowcost){//如果有更小的边，把小的替换原来的
                closedge[j].adjvex = G.vexs[mincost];
                closedge[j].lowcost = G.arcs[mincost][j];
            }
        }
    }
    return minSum;                                                                                      //返回最短路径之和
}
/*
typedef struct {
    int v, w;
    int info;
}  EdgeType;

bool Compare(EdgeType a, EdgeType b){
    EdgeType tmp;
    bool flag = 0;
    if(a.info > b.info){
        tmp = a;
        a = b;
        b = tmp;
        flag = 1;
    }
    return  1;
}

EdgeType* InitEdge(MGraph &G){
    EdgeType Edge[(G.n-1) * (G.n-1)];
    for(int i = 0; i < G.n; i++)
        for(int j = i+1 ; j < G.n; j++){
            if(G.arcs[i][j] != INFINITY && G.arcs[i][j] != 0){
                Edge[i].v = G.vexs[i];
                Edge[i].w = G.vexs[j];
                Edge[i].info = G.arcs[i][j];
                i++;
            }
    }
    return Edge;
}

void MiniSpanTree_Kruskal(MGraph &G){
    if(G.kind != 1 && G.kind != 3){
        printf("ERROR:Designed for Undirected\n");
        return;
    }
    EdgeType *edge;
    edge = InitEdge(G);
    for(int i = 0; i < sizeof(edge); i++){//对边表按权值从小到大排序
        for(int j = i; j < sizeof(edge); j++)
            Compare(edge[i], edge[j]);
    }
    for(int i = 0; i < sizeof(edge); i++){
        printf("%c<->%c\n", G.vexs[edge[i]].v, G.vexs[edge[i]].w);
    }
    printf("\nMiniSpanTree_Kruskal:\n");
    int parent[1000];
    for (int i = 0; i < G.n; i++)
    {
        parent[i] = 0;
    }
    for (int i = 0; i < sizeof(edge); i++)
    {
        int n = Find(parent, edge[i].num1);
        int m = Find(parent, edge[i].num2);
        if (n != m)
        {
            parent[n] = m;
            cout << edge[i].num1 << "-->" << edge[i].num2 << "   权值：" << edge[i].weight << endl;
        }
    }
}

*/
//MiniSpanTree_Kruskal






void GUIPrint(){
    printf(" ===================================================\n");
    printf("||         Data Structure Experiment Design        ||\n");
    printf("||                                                 ||\n");
    printf("||               Undirected Graph                  ||\n");
    printf("||                                                 ||\n");
    printf("||             Choose Your Opration                ||\n");
    printf("||                                                 ||\n");
    printf("||    A.Arcs_Add            P.PrintGraph           ||\n");
    printf("||                                                 ||\n");
    printf("||    B.BFS_Traversal       V.Vexs_Edit            ||\n");
    printf("||                                                 ||\n");
    printf("||    D.DFS_Traversal       M.Manual_Mode          ||\n");
    printf("||                                                 ||\n");
    printf("||    E.EXIT                                       ||\n");
    printf("||                                                 ||\n");
    printf("||    F.FirstAdjVex                                ||\n");
    printf("||                                                 ||\n");
    printf(" ===================================================\n");
}


Status Add_Vexs(MGraph &G, VexType V){
    int count = 0;
    int ** NewArcs;

    for(int i = 0; G.vexs[i]; i++){
        count++;
        if(V == G.vexs[i]) return ERROR;
    }
    if(count < G.n)
        G.vexs[count] = V;
    else{
        NewArcs = (int**)malloc((count+1)*sizeof(int*));
        for(int i = 0; i < count+1 ; i++){
            NewArcs[i] = ((int*)malloc((count+1)*sizeof(int)));
        }
    if(!NewArcs){
            printf("OVERFLOWED\n");
            return  OVERFLOW;
    }
    else{
        for(int i = 0; i < count+1; i++)
            for(int j = 0; j < count+1; j++){
                if(j < count && i < count)
                    NewArcs[i][j] = G.arcs[i][j];
                else if(G.kind == 0 || G.kind == 1)
                        NewArcs[i][j] = 0;
                     else
                        NewArcs[i][j] = INFINITY;
            }
            for(int i = 0; i < count; i++){
                free(G.arcs[i]);
                G.arcs[i] = NULL;
            }
            free(G.arcs);
            G.arcs = NULL;
            G.arcs = NewArcs;
            G.n +=1;
            G.vexs[G.n-1] = V;
        }
                                    
            
    }
    printf("Added Successfully\n");
    return OK;
}

Status Delete_Vexs(MGraph &G, VexType V){
    int k = LocateVex_M(G, V);
    int **arcs = G.arcs;
    if(k == G.n-1){
        int i, j;
        for(i = 0, j = G.n; i < G.n; i++)
        arcs[i][j] = 32767;
        free(arcs[G.n-1]);
        arcs[G.n-1] = NULL;
        G.n--;
        return  OK;
    }
    if(arcs && k != -1){
        int i,j;
        for(i = k; i < G.n-1; i++)
            for(j = 0; j < G.n; j++){
            arcs[i][j] = arcs[i+1][j];//将邻接矩阵下面的元素上移填补空缺
        }
        
        for(j = k; j < G.n-1; j++)
            for(i = 0; i < G.n-1; i++){
            arcs[i][j] = arcs[i][j+1];
        }//左移，此时空缺已填补完毕，得到缩小的邻接矩阵，但最外层仍是原来的数值，需要进行更改
        
        for(i = 0, j = G.n; i < G.n; i++)
            if(G.kind == 1 || G.kind == 3)
                arcs[i][j] = INFINITY;
            else
                arcs[i][j] = 0;
        
        free(arcs[G.n-1]);
        arcs[G.n-1] = NULL;
        //迫真释放内存
        for(i = k; i < G.n-1; i++)
            G.vexs[i] = G.vexs[i+1];//搬动顶点数组
        G.n--;
        
        return  OK;
    }
    return  ERROR;
}

int main(){
    MGraph G;
    VexType vexs[100];
    GraphKind GKind;
    int GSize,flag = 1;
    char operation = '\0';
    VexType tmp;
    //input Start

    printf("Pls choose the kind of graph(0.DG, 1.UDG, 2.DN, 3.UDN):");
    while(flag){
        flag = !scanf("\n%d",&GKind);
        if(GKind > 3 || GKind < 0)
            flag = 1;
        if(flag){
            printf("Error type of input, pls try again.\n");
            fflush(stdin);
            printf("Pls choose the kind of graph(0.DG, 1.UDG, 2.DN, 3.UDN):");
        }
    }
    printf("You've chosen %d.\n",GKind);
    printf("Pls choose the number vexs in the graph:");
    while(!scanf("\n%d",&GSize)){
        printf("Error type of input, pls try again.\n");
        fflush(stdin);
        printf("Pls choose the number of vexs in the graph:");
        
    }
    InitGraph_M(G, GKind, vexs, GSize);
    for(int i = 0; i < G.n; i++){
        printf("pls input the No.%d Vex:",i+1);
        scanf("\n%c",&tmp);
        Add_Vexs(G, tmp);
    }
    printGraph(G);
    printf("Trying to add some arcs?\n");
    while(tmp != 'N'){
        scanf("\n%c",&tmp);
        if(tmp == 'Y'){
            int k,m,info;
            printf("Pls input the first vex:");
            scanf("\n%c",&tmp);
            printf("The vexs you've input is in vexs[%d].\n",k = LocateVex_M(G, tmp));
            printf("Pls input the second vex:");
            scanf("\n%c",&tmp);
            printf("The vexs you've input is in vexs[%d].\n",m = LocateVex_M(G, tmp));
            printf("Pls input the info between %c and %c:",G.vexs[k],G.vexs[m]);
            scanf("\n%d",&info);
            if(ERROR == Add_Arcs(G, k, m, info))
                printf("Added Failed\n");
            printGraph(G);
        }
        printf("Input 'N' to EXIT.\n");
        printf("Input 'Y' to CONTINUE.\n");
    }
    printf("Graph initialized successfully, more operations are included in the next step.\n");
    printGraph(G);
    GUIPrint();
    while(operation != 'E'){
        printf("Choose your operation:");
        fflush(stdin);
        scanf("\n%c",&operation);
        switch (operation) {
            case 'V':
                printf("Do you want to add or delete vex?(A or D):");
                scanf("\n%c",&operation);
                if(operation == 'A'){
                    printf("Pls input the name of the vex you wanna add:");
                    while(!scanf("\n%c",&tmp)&&((tmp >= 'a' && tmp <= 'z')||(tmp >= 'A' && tmp <= 'Z')))
                        ;
                    while(!Add_Vexs(G, tmp)){
                        printf("Error:Duplicated vex names are found.\n");
                        printf("Pls input the name of the vex you wanna add:");
                        scanf("\n%c",&tmp);
                        fflush(stdin);
                    }
                }
                else if(operation == 'D'){
                    printf("Pls input the name of the vex you wanna delete:");
                    scanf("\n%c",&tmp);
                    while(!Delete_Vexs(G, tmp)){
                        printf("Error:Could not found vex name.\n");
                        printf("Pls input the name of the vex you wanna add:");
                        scanf("\n%c",&tmp);
                        fflush(stdin);
                    }
                }
                else
                    printf("Error type of input, exited operation.\n");
                break;
            case 'A':
                tmp = '\0';
                
                    
                    
                    int k,m,info;
                    printf("Pls input the first vex:");
                    scanf("\n%c",&tmp);
                    printf("The vexs you've input is in vexs[%d].\n",k = LocateVex_M(G, tmp));
                    printf("Pls input the second vex:");
                    scanf("\n%c",&tmp);
                    printf("The vexs you've input is in vexs[%d].\n",m = LocateVex_M(G, tmp));
                    printf("Pls input the info between %c and %c:",G.vexs[k],G.vexs[m]);
                    scanf("\n%d",&info);
                    if(ERROR == Add_Arcs(G, k, m, info))
                        printf("Added Failed\n");
                    
                   
                    
                break;
            case 'B':
                BFSTraverse_M(G);
                printf("BFS Traversal Finished\n");
                break;
            case 'D':
                DFSTraverse_M(G);
                printf("DFS Traversal Finished\n");
                break;
            case 'E':
                printf("Exited Program\n");
                break;
            case 'H':
                GUIPrint();
                break;
                
            case 'P':
                printGraph(G);
                break;
            case 'M':
                printf("Select a vexs to start:\n");
                scanf("\n%c",&tmp);
                printf("\nTne shortest path of Graph:\n\n");
                MiniSpanTree_Prim(G, G.vexs[LocateVex_M(G, tmp)]);
                break;
                
            default:
                printf("Wanna get some help? Input 'H' for more infomation.\n");
                break;
                
        }
    }
    
    return 0;

}
