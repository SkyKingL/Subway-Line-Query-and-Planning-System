#include<stdio.h>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<conio.h>//getch()函数
#include<cstring>//memset函数
#include <cmath>    //ceil()向上取整函数

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
using namespace std;
#define MAXSIZE 100

const double SubwaySpeed=10;//速度单位为米每秒,时间用分钟做单位 
const double WalkSpeed=1.5;

const int MaxChangeNum = 500;
// W 最大换乘站点数量
const int MaxLineNum=20;
//C 表示线路条数的最大值
const int MaxNodeNum=700;
//C 站点个数的最大值 
const int infinity=0x3f3f3f3f;
//C 表示无穷大,地铁站之间无法到达则距离为无穷大
//如果想将数组都赋值为无穷大,可用memset(数组名,0x3f,sizeof(数组名)) 
int LineCnt=0;					
//C 对线路进行计数,初始化为0
char Name[MaxNodeNum][20];		
//C 专门对换乘节点进行编号,这里记录的是换乘节点的名字 
int NameCnt=0;					
//C 对换乘节点进行计数,初始化为0 
const int FileMaxNum=2000;
//C 对文件中的信息的计数最大值，用于开放地铁站 

//C 地铁站节点 
typedef struct StationNode{
	StationNode *h;			//指向头结点,可以直接得到这个地铁站所处的线路 
	char name[20];			//地铁站的名字(非头结点)/线路名字(头结点)  
	int number[MaxLineNum];	//站点位于线路的名称数组，为-1表示为空，为其他值表示head的下标,即可表示可以换乘的线路 
	StationNode *front;		//前驱
	int f_weight; 			//往前的权重 ，头结点的next如果不是环，则为infinity,否则不为infinity 
	StationNode *next;		//尾结点指空，往后的代价不为零，默认成环，指向线路的第一个节点(非头结点) 
							//p直接指向头结点->next; 即线路的第一站
	int n_weight; 			//往后的代价
	 //n>1时，可换乘
}StationNode ,*LinkNode;

//C 邻接表的表结点(表示边) 
typedef struct WayNode{
	int way;					//这条线邻接的线路 
	struct WayNode *nextway;	//指向下一条邻接的边 
}WayNode;

//C 邻接表的头结点(表示点)
typedef struct VNode{
	int num;				//线路的标号
	struct WayNode *first;	//指向第一条和这条线路邻接的弧 
}VNode,v_list[MaxLineNum];

//C 邻接表 
typedef struct MGraph{
	int n,e;				//结点数和边数//好像没用到 
	v_list vertice;			//线路数组
							//讨论换乘次数的问题
}MGraph;

//C 权重矩阵 
typedef struct NGraph{
	int adjvex[MaxNodeNum][MaxNodeNum]; //换乘节点权重矩阵
}NGraph;

//C 用于暂时存储文件中信息的结构体,用于删去地铁站的操作 
typedef struct filewrite
{
	char linename[20];
	char name[20];
	int weight;
	int open;
}filewrite;

int FileCnt=0;
//C 用于对下面这个结构的计数 

filewrite f[FileMaxNum];
//C  用于暂时存储文件中信息的结构体数组 

//C 
StationNode head[MaxLineNum];//定义线路的头结点

//C
MGraph G1;		//线路图(把线路看成一个图点)
NGraph G2;		//各换乘结点的权重矩阵

//F 栈 
typedef struct{
	int *base;
	int *top;
	int nu;
	int stacksize;
}SqStack;

//F 队列 
typedef struct QNode{
	int data;
	struct QNode *next;
}QNode,*QueuePtr;

//F 队列 
typedef struct{
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

//F 邻接表??? 
typedef struct TreeNode{
	int way_locate;
	int next_number;//表示后继节点的个数 
	struct TreeNode *pointer[15];
	struct TreeNode *front; 
}TreeNode;

typedef int datatype;
//W 王嘉豪的队列 
struct Queue{
    datatype data[MAXSIZE];
    int length;
    Queue() {
        memset(data, 0, sizeof(data));
        length = 0;
    }
};

//W 用于存储站点对应序号信息
struct Point {
    char name[20];
    int number;
};

//W
struct Line {
    char name[20];
    int number;
};

int ChangeWayDisCnt = 0;
struct ChangeSituation {
    char StationName[20];
    char linename1[20];
    char linename2[20];
    int distance;
};
ChangeSituation ChangeWayDis[MaxChangeNum];
Point pos[MaxNodeNum * MaxLineNum];
//W
Line LineNumber[MaxLineNum];  
//W 序号存储线路信息
int dist[MaxNodeNum];
//W 记录路径长度
int st[MaxNodeNum];
//W 标记 
int Prev[MaxNodeNum]; 
//W 记录路径前一点
int DLine[MaxNodeNum][MaxNodeNum];
//W 用于通过同一线路的两个站点的序号判断此时路径处于几号线
int StationNumber;
//不同站点个数
int DWeight[MaxNodeNum][MaxNodeNum];
//W 用于存储站点与站点之间的距离/权值,未找到则返回-1
int P[MaxNodeNum][MaxNodeNum];
//W 用于记录前一路径的数组

void InitStack(SqStack *S);
int GetTop(SqStack *S);
void Push(SqStack *S,int e);
void Pop(SqStack *S,int *e);
//F 1.0栈操作 

void InitQueue(LinkQueue *Q);
void DestroyQueue(LinkQueue *Q);
void EnQueue(LinkQueue *Q,int e);
void DeQueue(LinkQueue *Q,int *e);
//F 1.0队列操作

void insertnode(StationNode &s,char linename[],char name[],int w);
//C 1.0 在s后面添加一个节点并初始化地铁站名称,权重 
void insertheadnode(StationNode &s,char linename[]);
//C 1.0 插入(初始化)头结点,头结点存线路名字 
void addLine(StationNode &p,int j);
//C 1.0 给p这个地铁站添加j这个换乘线路,j表示的head[j]的下标 
void insertv(VNode &p,int i);
//C 1.0 在邻接表中添加边,这里是在把线路看成点的邻接表中添加一条线路到另一条线路(即i)的边
//i对应的是head[i]的下标 
int get_ChangeStation_num(char name[]);
//C 1.0 在已知的换乘站中找到对应的下标,即Name[i]的下标
//如果找不到会返回-1 
void CreateGraph(MGraph &G,NGraph &G2);
//C W 1.0 读取地铁站文件,初始化链表,把线路看成点的图(G1),和换乘节点权重矩阵(G2)
int getlinenum(char name[]);//得到这个线路对应的是几号头结点 
//C 1.0 通过线路的名字遍历所有的头结点找到对应的头结点的下标,即head[i]的下标 
void PrintLine(int i);
//C 1.0 输出线路head[i]的所有站点 
void start_ui();
//C 1.0 系统的开始界面 
void information_check();
//C 1.0 信息查询界面 
void information_change();
//C 1.3 后台界面
void open(char name[]);
//C 1.3 开放地铁站操作的函数 
void close(char name[]);
//C 1.3关闭地铁站操作的函数 
void help();
//C 3.0 帮助界面 
int cost(char start[],char end[],int n);
//F 1.0 在邻接的路线之间转移的具体线路（如果有环还需完善） 返回代价值 
void change_least_way(MGraph &G);
//F 1.0 输出换乘最少线路 
void DFSTraverse(int s_way,int e_way,char start[],char end[],MGraph &G,int n);
//F 1.0  寻找换乘最少的所有线路
int change_time(int s_way,int e_way,char start[],char end[],MGraph &G);
//F 1.0 统计换乘次数 
void best_way(char start[],int change[],char end[],int n);
//F 1.0 给出终点和起点以及换乘路线数组，求出最优路线
void change_way(char start[],char end[],int n);
//F 1.0 给出两个站点之间的具体线路（两站点所处同一线路） 
int distance(char start[],char end[],int n);
//F     给出两站点之间的最小距离，由cost函数优化 
int GetLineNumber(char *name); 
//W 1.0 通过线路名称获得线路序号
char *GetLineName(int number);
//W 1.0 通过线路序号获取线路名称
int GetSiteNumber(char *name);
//W 1.0 根据站点名称获得站点序号,未找到则返回-1，返回的是pos[i]的下标 
char *GetSiteName(int number);
//W 1.0 通过站点序号获取name
int GetDistance(int num1, int num2);
//W 1.0 获取两个站点之间的直接权值，如果不能直接可达，返回-1
void Dijkstra(char start[], char end[]);
//W 1.0 获取start为起点，end为终点的最短路径
void Traceback(int v, int i, Queue &Q);
//W 1.0 输出最短路径 v源点，i终点,Q为存储站序号的队列
void GetPay(char start[], char end[]);
// W 1.0 获取start到end的票价
void Fast_Path_UI();
//W 1.0 最快路径查询ui
int Min(int a, int b);
//W 1.0 求较小值
bool Is_NULL(Queue Q);
//W 1.0 判断队列是否为空
void En_Queue(Queue &Q, int &e);
//W 1.0 入队
void De_Queue(Queue &Q, int &e);
//W 1.0 出队
void Display_Queue(Queue &Q);
//W 1.0 

// 6.25 wjh
int GetWalkDistance(char Sname[], char name1[], char name2[]);
// W 1.0 获取换乘步行距离，如果不存在，则返回0
void InitChangeWalk(void);
// W 1.0 初始化换乘步行距离
int main()
{
	CreateGraph(G1,G2);
	InitChangeWalk();
	while(1)
    {
    	start_ui();
    	char a;
        scanf("%s",&a);
        getchar();
        system("cls");
        
        switch(a)
        {
        	case '1':information_check();break;
        	case '2': Fast_Path_UI();break;
    		case '3':change_least_way(G1);break;
    		case '#':information_change();break;
    		case '4':help();break; 
    		case '0':return 0;
		}
    }
	return 0;
}
void help()
{
	cout<<"票价规则:"<<endl;
	cout<<"6公里(含)内3元"<<endl;
	cout<<"6-12公里(含)4元；"<<endl;
	cout<<"12-22公里(含)5元；"<<endl;
	cout<<"22-32公里(含)6元；"<<endl;
	cout<<"32公里以上每加1元可乘20公里。"<<endl;
	printf("请按Enter键返回上一级菜单\n");
	getch();
	system("cls");
} 
void close(char name[])
{
	FILE *fp;
	fp=fopen("2.2.txt","w");//文件不存在则创建，存在则清空
	for(int i=0;i<LineCnt;i++)
	{
		StationNode *p;
		p=&head[i];
		StationNode *q;
		q=p;
		while(q!=NULL)
		{
			if(q!=p)
			{
				if(strcmp(q->name,name)==0)
				{
					if(q->next!=NULL)//如果删除的这个地铁站不是终点 
					q->front->n_weight=q->n_weight+q->front->n_weight;
					else if(q->next==NULL&&q->n_weight>=infinity)//是终点但不是环 
					{
						q->front->n_weight= q->n_weight+q->front->n_weight;
					}
					else if(q->next==NULL&&q->n_weight<infinity)//是终点但是环 
					{
						q->front->n_weight= q->n_weight+q->front->n_weight;
					}
					if(q->h->next==q)//如果删的是起点站
					{
						if(q->f_weight<infinity)//如果是环就需要操作 
						{
							StationNode *s;
							s=q;
							while(s->next!=NULL)
							{
								s=s->next;
							}
							s->n_weight=s->n_weight+q->n_weight;
						}
					} 
				}
				//其实都是一样的 
			}
			q=q->next; 
		}
	}
	for(int i=0;i<LineCnt;i++)
	{
		StationNode *p;
		p=&head[i];
		StationNode *q;
		q=p;
		while(q!=NULL)
		{
			if(q==p)
			{
				fprintf(fp,"%s\n",p->name);
			}
			else
			{
				if(strcmp(q->name,name)==0)
				{
					fprintf(fp,"%s %s %d %d\n",p->name,q->name,q->n_weight,0);
				}
				else 
				fprintf(fp,"%s %s %d %d\n",p->name,q->name,q->n_weight,1);
			}
			q=q->next; 
		}
	}
	fclose(fp);
	cout<<name<<"已关闭"<<endl;
	printf("请按Enter键返回上一级菜单\n");
	getch();
	system("cls");
	CreateGraph(G1,G2);
}

void open(char name[])
{
	//先检查这个地铁站是否是被关闭的
	int sign=0;
	for(int i=0;i<LineCnt;i++)
	{
		StationNode *p; 
	    p=head[i].next;
	    while(p!=NULL)
	    {
	       	if(strcmp(name,p->name)==0)
	       	{
	        	sign=1;
	        	break;
	       	}
	       		p=p->next;
	    }
	      	if(sign==1)break;
	}
	if(sign==1)
	{
		cout<<"该地铁站未被关闭"<<endl;
		return;
	}	
	     		
	ifstream ifp;
	ifp.open("2.2.txt",ios::in);
	char linename[20];//表示当前正在记录的 
	char linename1[20];
	
	char sname[20];
	int w;
	int o;
	int num[10];
	int cnt=0;
	while(!ifp.eof())
	{
		ifp>>linename1;
		if(strcmp(linename1,linename)!=0)
		{
			strcpy(linename,linename1);
			strcpy(f[FileCnt].linename,linename);
			strcpy(f[FileCnt].name,linename);
		}
		else
		{
			ifp>>sname>>w>>o;
			strcpy(f[FileCnt].linename,linename);
			strcpy(f[FileCnt].name,sname);
			f[FileCnt].weight=w;
			if(strcmp(sname,name)==0)
			{
				o=1;
				num[cnt++]=FileCnt;
			}
			f[FileCnt].open=o;
		}
		FileCnt++;
	}
	ifp.close();
	
	for(int i=0;i<cnt;i++)//更新为原来的权值 
	{
		int j=num[i];
		if(strcmp(f[j-1].linename,f[j-1].name)==0)//如果开放的地铁站是起点站
		{
			int k=j+1;
			while(1)
			{
			 	if(k==FileCnt||strcmp(f[k].linename,f[j].linename)!=0)
			 	{
			 		k--;	
				}
				k++;
			}
			f[k].weight=f[k].weight-f[j].weight;
		}
		else
		{
			f[j-1].weight=f[j-1].weight-f[j].weight;
		}
	}
	
	FILE *fp;
	fp=fopen("2.2.txt","w");
	for(int i=0;i<FileCnt;i++)
	{
		if(strcmp(f[i].linename,f[i].name)==0)
		{
			fprintf(fp,"%s\n",f[i].linename);
		}
		else
		{
			fprintf(fp,"%s %s %d %d\n",f[i].linename,f[i].name,f[i].weight,f[i].open);
		}
	}
	CreateGraph(G1,G2);
	cout<<name<<"已开放"<<endl;
}

void information_change()
{
	while(1)
	{
		system("cls"); 
		cout<<endl<<endl<<endl<<endl<<endl; 
	    cout<<"                             ☆欢迎进入系统后台☆                             "<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".......                     ——1.>开放地铁站                          ......."<<endl;
	    cout<<".......                     ——2.>关闭地铁站                          ......."<<endl;
	    cout<<".......                     ——0.>回到上一级菜单                      ......."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
		char a;
		scanf("%s",&a);
		if(a=='1')
		{
			cout<<"请输入要开启的地铁站";
			char name[20];
			cin>>name;
			open(name);
			printf("请按Enter键返回上一级菜单\n");
	        getch();
	        system("cls");
	        break;
		}
		if(a=='2')
		{
			while(1)
			{
				cout<<"请输入要关闭的地铁站:";
				char name[20];
				cin>>name;
				StationNode *p; 
					int sign=0;
					for(int i=0;i<LineCnt;i++)
					{
						p=head[i].next;
						while(p!=NULL)
						{
							if(strcmp(name,p->name)==0)
							{
								sign=1;
								break;
							}
							p=p->next;
						}
						if(sign==1)break;
					}
				if(sign==0) cout<<"站点不存在请重新输入！"<<endl;
				else
				{
					close(name);
					break;
				}
			}
		}
		else break;
	}
	CreateGraph(G1,G2);
	
}

int cost(char start[],char end[],int n){
	//标记为n的线路上起点到终点的路线 
	//printf("jjjj");
	StationNode *p;
	int sum=0;
	//j用来查找站点所处线路是否为转接点 
	p=&head[n];
	//printf("%s",p->name);
	int direction=0;
	while(strcmp(p->name,start))
	{
		p=p->next;
		if(!strcmp(p->name,end))
		direction=1;
	}

	if(direction==0){

		while(strcmp(p->name,end)){

			sum+=p->n_weight;
			p=p->next;
			
		}
		
	}
	else{
		while(strcmp(p->name,end)){
			sum+=p->f_weight;
			p=p->front;
		}	
	}
	return sum;
}

//s_way=2即二号线，在数组中下标为一 

void insertnode(StationNode &s,char linename[],char name[],int w)
{
	StationNode *p=(StationNode *)malloc(sizeof(StationNode));
	p->next=NULL;//先初始化为空
	p->front=NULL;
	memset(p->number,-1,sizeof(p->number));
	p->front=&s;
	p->f_weight=s.n_weight;
	strcpy(p->name,name);
	p->n_weight=w;
	s.next=p;
	p->h=s.h;
}

void insertheadnode(StationNode &s,char linename[])
{
	strcpy(s.name,linename);
	s.h=&s;//指针回指 
	s.front=NULL;
	s.next=NULL;
	s.n_weight=infinity;
	s.f_weight=infinity;
	memset(s.number,-1,sizeof(s.number));
}

void addLine(StationNode &p,int j)//给p这个地铁站添加j这个换乘线路 
{
	int i=0;
	while(p.number[i]!=-1)i++;
	p.number[i]=j;
//	cout<<p.name<<"可换乘"<<head[p.number[i]].name<<endl; 
}

void insertv(VNode &p,int i)
{
	if(p.first==NULL)
	{
		
		WayNode *l=(WayNode *)malloc(sizeof(WayNode));
		l->nextway=NULL;
		p.first=l;
		l->way=i;
	}
	else
	{
		int sign=1;
		WayNode *q=p.first;
		while(q->nextway!=NULL)
		{
			if(q->way==i)
			{
				sign=0;break;
			}
			q=q->nextway;
		}
		if(sign==1&&q->way==i)sign=0;
		if(sign==1)
		{
			WayNode *l=(WayNode *)malloc(sizeof(WayNode));
			l->nextway=NULL;
			q->nextway=l;
			l->way=i;
		}
	}
}

int get_ChangeStation_num(char name[])
{
	for(int i=0;i<NameCnt;i++)
	{
		if(strcmp(name,Name[i])==0)
		return i;
	}
	return -1;
}
void CreateGraph(MGraph &G, NGraph &G2) {
	LineCnt=0;
	StationNumber=0;
	
    //先把每条线路每个结点输入到内存中
    ifstream ifp;  //定义文件输入流ifp
    StationNode *p;
    p=NULL;//初始化为空 
    ifp.open("2.2.txt", ios::in);
    char linename[20];  //表示当前正在记录的链表所处的线路
    char linename1[20];
    char name[20];
    int w;
    int open;
    //初始化站点间距离设置为INF
    //初始化站点到站点的线路序号为INF
    for (int i = 0; i < MaxNodeNum; i++)
        for (int j = 0; j < MaxNodeNum; j++) {
            DWeight[i][j] = DWeight[j][i] = infinity;
            DLine[i][j] = infinity;
        }
    while (!ifp.eof()) {
        ifp >> linename1;
        if (strcmp(linename1, linename) != 0)
		{
			if(p!=NULL&&p->n_weight!=infinity)
			{
				p->next=NULL;
				p->h->next->f_weight=p->n_weight;
			}
			else if(p!=NULL&&p->n_weight==infinity)
			{
				int j=GetSiteNumber(p->name);
				int i=GetSiteNumber(p->front->name);
				DWeight[i][j] = Min(DWeight[i][j], p->f_weight);
				DWeight[j][i] = DWeight[i][j];
				DLine[i][j] = LineCnt - 1;
                DLine[j][i] = LineCnt - 1;
			}
//			cout<<"0"<<endl;
            strcpy(linename, linename1);
            strcpy(LineNumber[LineCnt].name, linename);
            LineNumber[LineCnt].number = LineCnt;
            p = &head[LineCnt++];  //这里LineCnt是对线路的计数
            insertheadnode(*p, linename);
        }
		else
		{
            ifp >> name >> w >> open;
//            cout<<'1'<<endl;
            if(open==1)
            {
            	if (GetSiteNumber(name) == -1) 
				{
	                strcpy(pos[StationNumber].name, name);
	                pos[StationNumber].number = StationNumber;
	                StationNumber++;
	            }
				//也不能是头结点 
				if(p->front!=p->h&&p!=p->h)//不能是第一个地铁站，因为权重描述的是这一站到下一站 
				{
					int j=GetSiteNumber(p->name);
					int i=GetSiteNumber(p->front->name);
					DWeight[i][j] = Min(DWeight[i][j], p->f_weight);
					DWeight[j][i] = DWeight[i][j];
					DLine[i][j] = LineCnt - 1;
	                DLine[j][i] = LineCnt - 1;
				}
	            insertnode(*p, linename, name, w);
	            p = p->next;
			}
            

        }
        //		p=p->next;//p始终指向这个链表的最后一个
        //		printf("%s %s %d\n",linename,name,w);
    }
    ifp.close();

    //    //打印输出DWeight
    //    for (int i = 0; i < 2; i++)
    //        for (int j = 0; j < 2; j++) {
    //            cout << DWeight[i][j] << " ";
    //            if (j == 1) cout << endl;
    //        }

    //完善换乘信息
    for (int i = 0; i < LineCnt - 1; i++) {
        StationNode *q;
        p = head[i].next;
        while (p != NULL) {
            for (int j = i + 1; j < LineCnt; j++) {
                q = head[j].next;
                while (q != NULL) {
                    if (strcmp(p->name, q->name) == 0) {
                        addLine(*p, j);
                        addLine(*q, i);
                    }
                    q = q->next;
                }
            }
            p = p->next;
        }
    }

    //初始化MGraph,即把线路看成点的图的邻接表
    for (int i = 0; i < LineCnt; i++) {
        p = head[i].next;
        G.vertice[i].num = i;
        while (p != NULL) {
            int t = 0;
            while (p->number[t] != -1) {
                insertv(G.vertice[i], p->number[t]);
                t++;
            }
            p = p->next;
        }
    }

    //初始化 G2
    for (int i = 0; i < LineCnt; i++) {
        p = head[i].next;
        while (p->next != NULL) {
            StationNode *q;
            q = p->next;
            int distance = p->n_weight;
            while (q != NULL) {
                //				cout<<"2"<<endl;
                if (p->number[0] != -1 && q->number[0] != -1) {
                    int n1;
                    if (get_ChangeStation_num(p->name) == -1) {
                        n1 = NameCnt;
                        strcpy(Name[NameCnt], p->name);
                        NameCnt++;
                    } else {
                        n1 = get_ChangeStation_num(p->name);
                    }

                    int n2;
                    if (get_ChangeStation_num(q->name) == -1) {
                        n2 = NameCnt;
                        strcpy(Name[NameCnt], q->name);
                        NameCnt++;
                    } else {
                        n2 = get_ChangeStation_num(q->name);
                    }
                    // n1,n2分别是两个换乘节点的编号
                    G2.adjvex[n1][n2] = distance;
                    G2.adjvex[n2][n1] = distance;
                    //					cout<<Name[n1]<<" "<<Name[n2]<<" "<<distance<<endl;
                }
                q = q->next;
                if (q != NULL) distance = distance + q->f_weight;
            }
            p = p->next;
        }
    }
}

int getlinenum(char name[])//得到这个线路对应的是几号头结点 
{
	for(int i=0;i<LineCnt;i++)
	{
		StationNode *p;
		p=&head[i];
		if(strcmp(p->name,name)==0)
		{
			return i;
		}
	}
	return -1;
}

void PrintLine(int i)
{
	StationNode *p;
	StationNode *q;
	p=&head[i];//p始终指向头结点
	q=p;
	while(q!=NULL)
	{
		if(q==p)//q为头结点
		{
			cout<<q->name<<endl;
		}
		else
		{
			printf("%s ",q->name,q->n_weight);
			if(q->next!=NULL)printf("<->"); 
		}
		if(q->next==NULL&&q->n_weight<infinity)
		{
			cout<<endl<<"注意此线路为环线!!!"<<endl;
		}
		q=q->next;
		
	}
	cout<<endl;
}

void start_ui()
{
	system("cls");
    cout<<endl<<endl<<endl<<endl<<endl; 
    cout<<"                     ☆北京地铁线路查询及规划系统欢迎您 ☆                    "<<endl;
    cout<<".............................................................................."<<endl;
    cout<<".............................................................................."<<endl;
    cout<<".............................................................................."<<endl;
    cout<<".......                     ——1.>地铁信息查询                        ......."<<endl;
    cout<<".......                     ——2.>最快路径查询                        ......."<<endl;
    cout<<".......                     ——3.>最少换乘查询                        ......."<<endl;
    cout<<".......                     ——4.>帮助                                ......."<<endl;
    cout<<".......                     ——0.>退出系统                            ......."<<endl;
    cout<<".............................................................................."<<endl;
    cout<<".............................................................................."<<endl;
    cout<<".............................................................................."<<endl;
}

void information_check()
{
	while(1)
	{
		system("cls"); 
		cout<<endl<<endl<<endl<<endl<<endl; 
	    cout<<"                             ☆地铁信息查询☆"<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".......                     ——1.>按线路名称查询                      ......."<<endl;
	    cout<<".......                     ——2.>按站点名称查询                      ......."<<endl;
	    cout<<".......                     ——3.>查询线路换乘信息                    ......."<<endl;
	    cout<<".......                     ——0.>回到上一级菜单                      ......."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl; 
		char a;
	    scanf("%s",&a);
	    system("cls");
	    
	    if(a=='1')
	    {
	    	while(1)
	    	{
	    		for(int i=0;i<LineCnt;i++)
	    		{
	    			StationNode *p;
	    			p=&head[i];
					cout<<"——"<<i+1<<".  >"<<p->name<<endl; 
				}
				cout<<"——0.  >返回上一级菜单"<<endl;
	        	cout<<endl<<endl<<endl<<"请输入对应数字查询相关线路信息:";
	        	int c;
	        	cin>>c;
	        	if(c>=1&&c<=LineCnt)
	        	{
	        		PrintLine(c-1);
	        		printf("请按Enter键返回上一级菜单");
	            	getch();
	        		system("cls");
	        		break;
				}
				else break;
			}
		}
		else if(a=='2')
		{
			while(1)
	  		{
	   			cout<<"            >>>>>>>>>请输入要查询的站点名：";
	   			char name[20];
	   			cin>>name;
	   			StationNode *p;
	   
	     		//找到名字为name的结点 
	    		int sign=0;
	     		for(int i=0;i<LineCnt;i++)
	     		{
	      			p=head[i].next;
	      			while(p!=NULL)
	      			{
	       				if(strcmp(name,p->name)==0)
	       				{
	        				sign=1;
	        				break;
	       				}
	       				p=p->next;
	      			}
	      			if(sign==1)break;
	     		}
	     
	//   cout<<p->name<<endl;
	   			if(strcmp(p->name,name))cout<<"站点不存在请重新输入！"<<endl;
	   			else
	   			{
	   				cout<<"该站点所在的地铁线路有："<<endl;
	    			StationNode *q;
	    			q=p->h;
	    			PrintLine(getlinenum(q->name));
	    			int i=0;
	    			while(p->number[i]!=-1)
	    			{
	     				PrintLine(p->number[i]);
	     				cout<<endl;
	     				i++;
	    			}
	    			printf("\n");
	                printf("请按Enter键返回上一级菜单");
	                getch();
	                system("cls");
	                break;
	   			}
	  		} 
		}
		else if(a=='3')
		{
			while(1)
			{
				cout<<"查询线路换乘的相关信息"<<endl;
				cout<<"请输入线路名称:";
				char name[20];
				cin>>name;
				if(getlinenum(name)==-1)
				{
					cout<<"站点不存在请重新输入！"<<endl;
				}
				else
				{
					WayNode *p;
					p=G1.vertice[getlinenum(name)].first;
					cout<<name<<"可换乘:"<<endl;
					while(p!=NULL)
					{
						cout<<head[p->way].name<<endl;
						p=p->nextway;
					}
					printf("请按Enter键返回上一级菜单");
			        getch();
			        system("cls");
			        break;
				}
			}
		}
		else if(a=='0')break; 
	}
	
}

void InitQueue(LinkQueue *Q){
	Q->front=Q->rear=(QueuePtr)malloc(sizeof(QNode));
	if(!Q->front) exit(0);
	Q->front->next=NULL; 
}

void DestroyQueue(LinkQueue *Q){
	while(Q->front){
		Q->rear=Q->front->next;
		free(Q->front);
		Q->front=Q->rear;
	}
}

void EnQueue(LinkQueue *Q,int e){
	QueuePtr p;
	p=(QueuePtr)malloc(sizeof(QNode));
	if(!p) exit(0);
	p->data=e;
	p->next=NULL;
	Q->rear->next=p;
	Q->rear=p;
}

void DeQueue(LinkQueue *Q,int *e){
	QueuePtr p;
	p=Q->front->next;
	*e=p->data;
	Q->front->next=p->next;
	if(Q->rear==p)
	Q->rear=Q->front;
	free(p);
}

void InitStack(SqStack *S){
	
	S->base=(int *)malloc(STACK_INIT_SIZE * sizeof(int));
	if(!S->base) exit(0);
	S->top=S->base;
	S->nu=0;
	S->stacksize=STACK_INIT_SIZE;

}

int GetTop(SqStack *S){
	int e;
	if(S->top==S->base) return 0;
	e=*(S->top-1);
	return e;
}

void Push(SqStack *S,int e){
	if(S->top-S->base>=S->stacksize){
		S->base=(int *)realloc(S->base,(S->stacksize+STACKINCREMENT)*sizeof(int));
		if(!S->base) exit(0);
		S->top=S->base+S->stacksize;
		S->stacksize+=STACKINCREMENT;
		 
	}
	*S->top++=e;
	S->nu++;
}

void Pop(SqStack *S,int *e){
	if(S->top==S->base) 
	return;
	*e=*--S->top;
	S->nu--;
}

int change_time(int s_way,int e_way,char start[],char end[],MGraph &G){
	int visited[20];
	int frontway[20];//记录线路的前一个线路 
	int change_ci[10];//假设最多中转十次 
	for(int i=0;i<10;i++)
	change_ci[i]=-1;
	int b,j=0;
	int  *u=&b;
	for(int i=0;i<MaxLineNum;i++)
	{visited[i]=0;
	frontway[i]=-1;//代表无前驱； 
	}
	LinkQueue *Q;
	Q=(LinkQueue *)malloc(sizeof(LinkQueue));
	InitQueue(Q);
	SqStack *S=(SqStack *)malloc(sizeof(SqStack));
	InitStack(S);
	
	struct WayNode *p;
	EnQueue(Q,s_way);
	//printf("%d ",p->way);
	visited[s_way]=1;
	while(visited[e_way]!=1){
		p=G.vertice[Q->front->next->data].first;
		//printf("%d ",p->way);
		while(p){
		if(!visited[p->way]){
			EnQueue(Q,p->way);
			//printf("%d ",p->way);
			visited[p->way]=1;
			frontway[p->way]=Q->front->next->data;
			p=p->nextway;
			}
			else 
			p=p->nextway;
		}
		DeQueue(Q,u);
    }
    int i=e_way;
    while(frontway[i]!=-1){
    	//printf("%d ",frontway[i]);
    	Push(S,frontway[i]);
    	i=frontway[i];
	}
	//printf("最小换乘方式线路为：\n");
	while(S->base!=S->top){
		Pop(S,u);
		change_ci[j]=b;
		j++;
		//printf("%s",head[b].name);
		//printf("->");
	}
	change_ci[j]=e_way;
	return j;
}




void change_least_way(MGraph &G){
	cout << "                             ☆换乘次数最少查询☆" << endl;
	int nodes_locate[20];//定位起点所处路径 
	int j=1;
	char name_s[20];
	while(1)
	{
			//站点所处的线路最多为三个 
			printf("请输入起点站点名："); 
			cin>>name_s;
			StationNode *p;
			
					//找到名字为name的结点 
					int sign=0;
					for(int i=0;i<LineCnt;i++)
					{
						p=head[i].next;
						while(p!=NULL)
						{
							if(strcmp(name_s,p->name)==0)
							{
								sign=1;
								break;
							}
							p=p->next;
						}
						if(sign==1)break;
					}
					
//			cout<<p->name<<endl;
			if(sign==0)cout<<"站点不存在请重新输入！"<<endl;
			else
			{
				//cout<<"该站点所在的地铁线路有："<<endl;
				StationNode *q;
				q=p->h;
				nodes_locate[0]=getlinenum(q->name);
				//PrintLine(nodes_locate[0]);//要注释 
				 int i=0;//j代表节点位于线路数组的下标 
    			while(p->number[i]!=-1)
    		{	nodes_locate[j]=p->number[i];
    			//PrintLine(nodes_locate[j]);
     			cout<<endl;
     			i++;
     			j++;
    		}
    		break;
		}
			
	} //

	int nodee_locate[20];//定位终点所处路径 
	int m=1;
	char name_e[20];
			while(1)
			{
				printf("请输入终点站点名：");			
				//站点所处的线路最多为三个 
				cin>>name_e;
				StationNode *p;
			
					//找到名字为name的结点 
					int sign=0;
					for(int i=0;i<LineCnt;i++)
					{
						p=head[i].next;
						while(p!=NULL)
						{
							if(strcmp(name_e,p->name)==0)
							{
								sign=1;
								break;
							}
							p=p->next;
						}
						if(sign==1)break;
					}
					
//			cout<<p->name<<endl;
				if(sign==0)cout<<"站点不存在请重新输入！"<<endl;
				else
				{
					//cout<<"该站点所在的地铁线路有："<<endl;
					StationNode *q;
					q=p->h;
					nodee_locate[0]=getlinenum(q->name);
					//PrintLine(nodee_locate[0]);//要注释
					 int i=0;//j代表节点位于线路数组的下标
	    			while(p->number[i]!=-1)
		    		{
						nodee_locate[m]=p->number[i];
		    			//PrintLine(nodee_locate[m]);
		     			cout<<endl;
		     			i++;
		     			m++;
		    		}
		    		break;
				}
			
			} //while
	int min=10,c=0; 
	for(int i=0;i<j;i++)
	{
		for(int n=0;n<m;n++)
		{
			if(change_time(nodes_locate[i],nodee_locate[n],name_s,name_e,G1)<=min)
			{
				min=change_time(nodes_locate[i],nodee_locate[n],name_s,name_e,G1);
				c++;
			}
		}
	}//一个站点可能同时位于多个线路中 
	for(int i=0;i<j;i++)
	{
		for(int n=0;n<m;n++)
		{
			if(change_time(nodes_locate[i],nodee_locate[n],name_s,name_e,G1)==min)
			DFSTraverse(nodes_locate[i],nodee_locate[n],name_s,name_e,G1,min+1);
			printf("\n");
		}
	}
			printf("请按Enter键返回上一级菜单");
		    getch();
		    system("cls");
}

void DFSTraverse(int s_way,int e_way,char start[],char end[],MGraph &G,int n){
	if(s_way==e_way){
		printf("无需换乘：\n");
		change_way(start,end,s_way);
		int t=0,sum=distance(start,end,e_way);
		printf("%s\n",end);
		GetPay(start,end);
		printf("\n");
		t=sum/SubwaySpeed/60;//
		int hh = t / 60;
    	int mm = t - hh * 60;
    	if (hh == 0) 
		cout << "用时为：" << mm << "分钟";
    	else { cout << "用时为：" << hh << "小时" << mm << "分钟"; }
		printf("\t\t\t步行总距离为：0米\n");
		
		return; 
	} 
	TreeNode Way[MaxLineNum];//再将线路抽象 
	struct WayNode *p;
	for(int i=0;i<MaxLineNum;i++)
	{
		for(int j=0;j<15;j++)
		{
			Way[i].pointer[j]=NULL;
		}
	}//pointer指针全部初始为null 
	for(int i=0;i<MaxLineNum;i++)
	{
		p=G.vertice[i].first;
		Way[i].way_locate=i;
		int j=0;
		while(p!=NULL)
		{
			Way[i].pointer[j]=&Way[p->way];//线路与线路之间相连 
			p=p->nextway;
			j++;
		}
		Way[i].next_number=j; 
	}
	//TreeNode中的front好像没有用到

	int deep;
	 struct TreeNode *q[10];
	 q[0]=&Way[s_way];
	//int change[10][n];//最多十条转接路线 
	printf("为您规划以下换乘最少线路：\n\n");
	for(int i=0;i<q[0]->next_number;i++)
	{	
		deep=2;//第一重循环 
	 	q[1]=q[0]->pointer[i];
	 	if(deep==n)
		{
	 		if(q[1]->way_locate==e_way)
			{
	 			int change_ci[10];
	 			for(int k=0;k<n;k++)
				{
	 				printf("%s  ",head[q[k]->way_locate].name);
	 				change_ci[k]=q[k]->way_locate;
				}
				printf("\n\n");
				best_way(start,change_ci,end,n);
				printf("\n\n");
			}
	 			continue;
		}
	 		
	 		
	 		
	 	for(int l=0;l<q[1]->next_number;l++)
	 	{
		 	deep=3;//第二重循环 
	 		q[2]=q[1]->pointer[l];
	 		if(deep==n)
	 		{
			 	if(q[2]->way_locate==e_way)
				{
		 			int change_ci[10];
		 			for(int k=0;k<n;k++)
					{
		 				printf("%s  ",head[q[k]->way_locate].name);
		 				change_ci[k]=q[k]->way_locate;	
					}
					printf("\n\n");
					best_way(start,change_ci,end,n);
					printf("\n\n");
				}
	 				continue;
			}
	 			
	 			
	 		for(int x=0;x<q[2]->next_number;x++)
			{
	 			deep=4;
	 			q[3]=q[2]->pointer[x];
	 			if(deep==n)
				{
	 				if(q[3]->way_locate==e_way)
					{
	 					int change_ci[10];
	 					for(int k=0;k<n;k++)
						{
	 						printf("%s  ",head[q[k]->way_locate].name);
	 						change_ci[k]=q[k]->way_locate;
	 					}
					 	printf("\n\n");
					 	best_way(start,change_ci,end,n);
					 	printf("\n\n");
				 	}
	 				continue;
				}
	 			
	 			for(int y=0;y<q[3]->next_number;y++)
				{
	 				deep=5;
	 				q[4]=q[3]->pointer[y]; 
	 				if(deep==n)
					{
	 					if(q[4]->way_locate==e_way)
						{
		 					int change_ci[10];
		 					for(int k=0;k<n;k++)
							{
		 						printf("%s  ",head[q[k]->way_locate].name);
		 						change_ci[k]=q[k]->way_locate;
							}
					 		printf("\n\n");
					 		best_way(start,change_ci,end,n);
					 		printf("\n\n");
				 		}
				 		continue;
					}
	 				
					for(int z=0;z<q[4]->next_number;z++)
					{
	 					deep=6;
	 					q[5]=q[4]->pointer[z];
	 					if(deep==n)
						{
	 						if(q[5]->way_locate==e_way)
							{
	 							int change_ci[10];
	 							for(int k=0;k<n;k++)
								{
	 								printf("%s  ",head[q[k]->way_locate].name);
	 								change_ci[k]=q[k]->way_locate;
	 							}
					 			printf("\n\n");
					 			best_way(start,change_ci,end,n);
					 			printf("\n\n");
				 			}
				 			continue;	
						}
					}//第五重循环	
				}//第四重循环 
			}//第三重循环 
		}//第二重循环 
	}//第一重循环
	//每一重表示换乘一次 
}


void best_way(char start[], int change[], char end[], int n) {
    // change数组表示顺序换乘的节点
    InitChangeWalk();
    char *front_station[10];
    front_station[0] = start;
    front_station[n] = end;
    int sum = 0;
    int walk_dis = 0;
    for (int i = 0; i < n - 1; i++) {
        StationNode *p = &head[change[i]];  //先初始化为最开始所在的线路
        int num = 0, flag = 0;
        int min;
        char *zhuan[5];
        int zh[5];
        while (p) {
            p = p->next;
            int c = 0;
            while (p->number[c] != -1) {
                if (p->number[c] == change[i + 1])  //因为这里是i+1所以循环条件是n-1
                {
                    zhuan[num] = p->name;
                    zh[num] = distance(front_station[i], zhuan[num], change[i]);
                    num++;
                    c++;
                }
                c++;
            }
            if (p->next == NULL) break;
        }
        min = zh[0];

        for (int j = 1; j < num; j++) {
            if (zh[j] < min) {
                min = zh[j];
                flag = j;
                break;
            }
        }
        front_station[i + 1] = zhuan[flag];
        change_way(front_station[i], front_station[i + 1], change[i]);
        sum += distance(front_station[i], front_station[i + 1], change[i]);
        walk_dis += GetWalkDistance(zhuan[flag], head[change[i]].name, head[change[i + 1]].name);
    }

    change_way(front_station[n - 1], end, change[n - 1]);
    sum += distance(front_station[n - 1], end, change[n - 1]);
    printf("%s\n\n", end);
    int t = 0;
    GetPay(start, end);
    printf("\n");
    t = sum / SubwaySpeed / 60 + walk_dis / WalkSpeed / 60;  //
    int hh = t / 60;
    int mm = t - hh * 60;
    if (hh == 0) cout << "用时为：" << mm << "分钟";
    // 6.26
    else {
        cout << "用时为：" << hh << "小时";
        if (mm != 0) cout << mm << "分钟";
    }
    printf("\t\t\t步行总距离为：%d米\n", walk_dis);
}

void InitChangeWalk() {
    ifstream ifp;
    ifp.open("Change.txt", ios::in);
    char Sname[20], name1[20], name2[20];
    int dis;
    while (!ifp.eof()) {
        ifp >> Sname >> name1 >> name2 >> dis;
        strcpy(ChangeWayDis[ChangeWayDisCnt].StationName, Sname);
        strcpy(ChangeWayDis[ChangeWayDisCnt].linename1, name1);
        strcpy(ChangeWayDis[ChangeWayDisCnt].linename2, name2);
        ChangeWayDis[ChangeWayDisCnt].distance = dis;
        ChangeWayDisCnt++;
    }
}

//获取换乘步行距离，如果不存在，则返回0
int GetWalkDistance(char Sname[], char name1[], char name2[]) {
    // cout << Sname << " " << name1 << " " << name2 << endl;
    for (int i = 0; i < ChangeWayDisCnt; i++) {
        if (strcmp(Sname, ChangeWayDis[i].StationName) == 0) {
            if ((strcmp(name1, ChangeWayDis[i].linename1) == 0 && strcmp(name2, ChangeWayDis[i].linename2) == 0) ||
                (strcmp(name1, ChangeWayDis[i].linename2) == 0 && strcmp(name2, ChangeWayDis[i].linename1) == 0)) {
                return ChangeWayDis[i].distance;
            }
        }
    }
    return 0;
}

void change_way(char start[],char end[],int n){
	StationNode *p=&head[n];
	StationNode *q=&head[n];
	q=q->next;
	//printf("hhhh");
	int sum=0;//sum计算一条线路的总权值 

	int circle=0;//标记线路是否为环 
	if(head[n].next->f_weight!=infinity)
	circle=1;
	int direction=0;
	while(strcmp(p->name,start)){
		
		p=p->next;
		if(!strcmp(p->name,end))
		direction=1;
	} 

	printf("(%s)",head[n].name);
	if(circle==0)
	{
			if(direction==0){
		while(strcmp(p->name,end)){
			printf("%s->",p->name);
			p=p->next;
			
		}
		
	}
	else{
		while(strcmp(p->name,end)){
			printf("%s->",p->name);
			p=p->front;
			
			
		}	
	}
		}
	else{
		while(q->next){
		sum+=q->n_weight;
		q=q->next;
	}
	sum+=q->n_weight;
	if(cost(start,end,n)>sum/2)
	direction=!direction;
	 	if(direction==0){
		while(strcmp(p->name,end)){
			printf("%s->",p->name);
			p=p->next;
			if(p==NULL){
				p=head[n].next;
			}
		}
		
	}
	else{
		while(strcmp(p->name,end)){
			printf("%s->",p->name);
			p=p->front;
				if(p==p->h)
				p=p->front;//由于首结点指针往回指，指向头结点时直接跳过防止输出线信息 
				if(p==NULL){
				p=q;
			}
			
		}	
	}
	}

	
}
int distance(char start[],char end[],int n) {
	StationNode *p=&head[n];
	StationNode *q=&head[n];
	q=q->next;
	int sum=0;
	int circle=0;
	if(head[n].next->f_weight!=infinity)
	circle=1;
	int direction=0;
	while(strcmp(p->name,start)){
		
		p=p->next;
		if(!strcmp(p->name,end))
		direction=1;
	} 

	if(circle==0)
	return cost(start,end,n);
		
	else{
		while(q->next){
		sum+=q->n_weight;
		q=q->next;
	}
	sum+=q->n_weight;
	if(cost(start,end,n)>sum/2)
	return sum-cost(start,end,n);
	else
	return cost(start,end,n);
	}

}
//根据站点名称获得站点序号,未找到则返回-1
int GetSiteNumber(char name[]) {
    for (int i = 0; i < StationNumber; i++) {
        if (strcmp(pos[i].name, name) == 0) { return i; }
    }
    return -1;
}

//通过站点序号获取name
char *GetSiteName(int number) {
    for (int i = 0; i < StationNumber; i++) {
        if (pos[i].number == number) { return pos[i].name; }
    }
}

int GetLineNumber(char name[]) {
    for (int i = 0; i < LineCnt; i++) {
        if (strcmp(LineNumber[i].name, name) == 0) { return i; }
    }
    return -1;
}

char *GetLineName(int number) {
    for (int i = 0; i < LineCnt; i++) {
        if (LineNumber[i].number == number) { return LineNumber[i].name; }
    }
}

//求较小值
int Min(int a, int b) { return a < b ? a : b; }

//获取start为起点，end为终点的最短路径
void Dijkstra(char start[], char end[]) {
    GetPay(start, end);
    memset(dist, 0x3f, sizeof(dist));
    int s = GetSiteNumber(start);
    int e = GetSiteNumber(end);
    int mindis, dis;
    //初始化
    // dist[i]表示s到i的最小距离
    for (int i = 0; i < StationNumber; i++) {
        dist[i] = DWeight[s][i];
        st[i] = 0;
        if (dist[i] == infinity) Prev[i] = -1;
        else
            Prev[i] = s;
    }
    dist[s] = 0;
    int t;
    for (int i = 0; i < StationNumber; i++) {
        mindis = infinity;
        t = s;
        for (int j = 0; j < StationNumber; j++) {
            if (!st[j] && dist[j] < mindis) {
                mindis = dist[j];
                t = j;
            }
        }
        st[t] = 1;
        for (int j = 0; j < StationNumber; j++) {
            if (st[j] == 0 && DWeight[t][j] < infinity) {
                dis = dist[t] + DWeight[t][j];
                if (dist[j] > dis) {
                    dist[j] = dis;
                    Prev[j] = t;
                }
            }
        }
    }
    //    cout<<dist[e]<<endl;
    //    cout<<dist[GetSiteNumber("生命科学园")]+DWeight[GetSiteNumber("西二旗")][GetSiteNumber("生命科学院")]<<endl;
    cout << endl << endl;
    cout << "为您规划从[" << start << "]到[" << end << "]的最快线路为：" << endl << endl;
    Queue Q;
    Traceback(s, e, Q);
    // cout << endl << "Show Queue" << endl;
    // Display_Queue(Q);
    cout << endl << endl;
    cout << "具体换乘信息如下：" << endl;
    cout << "从 [" << start << "] 出发--> ";
    int WalkDistance = 0;
    if (Q.length == 2) {
        int u, t;
        De_Queue(Q, u);
        De_Queue(Q, t);
        int line0 = DLine[u][t];
        cout << "搭乘{" << GetLineName(line0) << "}" << endl;
    } else {
        int preLine = infinity;  //记录前一站所处线路
        int nowLine = infinity;  //记录现在所处线路
        int t_p, t_n;
        De_Queue(Q, t_p);
        while (!Is_NULL(Q)) {
            De_Queue(Q, t_n);
            nowLine = DLine[t_p][t_n];

            // cout << t_p << " " << t_n << " " << nowLine << endl;
            if (nowLine != preLine) {
                if (preLine != infinity) {
                    // 6.25 wjh
                    WalkDistance += GetWalkDistance(GetSiteName(t_p), GetLineName(preLine), GetLineName(nowLine));
                    cout << "搭乘{" << GetLineName(preLine) << "} --> ";
                    cout << "于[" << GetSiteName(t_n) << "]换乘 --> ";
                }
            }
            preLine = nowLine;
            t_p = t_n;
        }
        // 6.26
        cout << "搭乘{" << GetLineName(preLine) << "} --> ";
    }

    cout << "到达目标站[" << end << "]" << endl << endl;
    cout << "步行距离为" << WalkDistance << "米" << endl << endl;
    int Sum_Time = ceil((dist[e] * 1.0 / SubwaySpeed + WalkDistance * 1.0 / WalkSpeed) / 60);  //总用时，单位分钟
    // cout << Sum_Time << endl;
    int hh = Sum_Time / 60;
    int mm = Sum_Time - hh * 60;
    if (hh == 0) cout << "总时长约为" << mm << "分钟" << endl;
    // 6.26
    else {
        cout << "用时为：" << hh << "小时";
        if (mm != 0) cout << mm << "分钟";
    }
}
//输出最短路径 v源点，i终点
void Traceback(int v, int i, Queue &Q) {
    // 源点等于终点时，即找出全部路径
    if (v == i) {
        En_Queue(Q, i);
        cout << "[" << GetSiteName(i) << "]";
        return;
    }
    Traceback(v, Prev[i], Q);
    cout << "-->[" << GetSiteName(i) << "]";
    En_Queue(Q, i);
}

//最快路径查询ui
void Fast_Path_UI() {
    system("cls");
    cout << endl << endl << endl << endl << endl;
    cout << "                             ☆最快路径查询☆" << endl;
    cout << ".............................................................................." << endl;
L1:
    cout << "            >>>>>>>>>请输入起始站点名：";
    char start[20];
    cin >> start;
    StationNode *p;

    //找到名字为start的结点
    int sign = 0;
    for (int i = 0; i < LineCnt; i++) {
        p = head[i].next;
        while (p != NULL) {
            if (strcmp(start, p->name) == 0) {
                sign = 1;
                break;
            }
            p = p->next;
        }
        if (sign == 1) break;
    }

    //   cout<<p->name<<endl;
    if (sign==0) {
        cout << "站点不存在请重新输入！" << endl;
        goto L1;
    }

L2:
    cout << "            >>>>>>>>>请输入终点站点名：";
    char end[20];
    cin >> end;

    //找到名字为end的结点
    sign = 0;
    for (int i = 0; i < LineCnt; i++) {
        p = head[i].next;
        while (p != NULL) {
            if (strcmp(end, p->name) == 0) {
                sign = 1;
                break;
            }
            p = p->next;
        }
        if (sign == 1) break;
    }

    if (sign==0) {
        cout << "站点不存在请重新输入！" << endl;
        goto L2;
    }
    Dijkstra(start, end);
    cout << endl;
    printf("请按Enter键返回上一级菜单");
    getch();
}
//判断队列是否为空
bool Is_NULL(Queue Q) { return (Q.length == 0); }

//入队
void En_Queue(Queue &Q, int &e) {
    Q.data[Q.length] = e;
    Q.length++;
}

//出队
void De_Queue(Queue &Q, int &e) {
    if (Is_NULL(Q)) {
        cout << "The queue is empty~";
        return;
    }
    e = Q.data[0];
    for (int i = 0; i < Q.length - 1; i++) { Q.data[i] = Q.data[i + 1]; }
    Q.length--;
}

void Display_Queue(Queue &Q) {
    for (int i = 0; i < Q.length; i++) { cout << Q.data[i] << " "; }
    cout << endl;
}

void GetPay(char start[], char end[]) {
    memset(dist, 0x3f, sizeof(dist));
    int s = GetSiteNumber(start);
    int e = GetSiteNumber(end);
    int mindis, dis;
    //初始化
    // dist[i]表示s到i的最小距离
    for (int i = 0; i < StationNumber; i++) {
        dist[i] = DWeight[s][i];
        st[i] = 0;
        if (dist[i] == infinity) Prev[i] = -1;
        else
            Prev[i] = s;
    }
    dist[s] = 0;
    int t;
    for (int i = 0; i < StationNumber; i++) {
        mindis = infinity;
        t = s;
        for (int j = 0; j < StationNumber; j++) {
            if (!st[j] && dist[j] < mindis) {
                mindis = dist[j];
                t = j;
            }
        }
        st[t] = 1;
        for (int j = 0; j < StationNumber; j++) {
            if (st[j] == 0 && DWeight[t][j] < infinity) {
                dis = dist[t] + DWeight[t][j];
                if (dist[j] > dis) {
                    dist[j] = dis;
                    Prev[j] = t;
                }
            }
        }
    }
    mindis = dist[e];  //从start到end的最短路径长度

    int pay;
    mindis = ceil(mindis * 1.0 / 1000);  //单位由米转公里，向上取整
    // 6公里(含)内3元；6-12公里(含)4元；12-22公里(含)5元；22-32公里(含)6元；32公里以上每加1元可乘20公里。
    if (mindis <= 6) {
        pay = 3;
    } else if (mindis > 6 && mindis <= 12) {
        pay = 4;
    } else if (mindis > 12 && mindis <= 22) {
        pay = 5;
    } else if (mindis > 22 && mindis <= 32) {
        pay = 6;
    } else {
        pay = 6 + ceil((mindis - 32) / 20);
    }

    cout << "从[" << start << "]到[" << end << "]的票价为 " << pay << "元";
}
