#include<stdio.h>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<conio.h>//getch()����
#include<cstring>//memset����
#include <cmath>    //ceil()����ȡ������

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
using namespace std;
#define MAXSIZE 100

const double SubwaySpeed=10;//�ٶȵ�λΪ��ÿ��,ʱ���÷�������λ 
const double WalkSpeed=1.5;

const int MaxChangeNum = 500;
// W ��󻻳�վ������
const int MaxLineNum=20;
//C ��ʾ��·���������ֵ
const int MaxNodeNum=700;
//C վ����������ֵ 
const int infinity=0x3f3f3f3f;
//C ��ʾ�����,����վ֮���޷����������Ϊ�����
//����뽫���鶼��ֵΪ�����,����memset(������,0x3f,sizeof(������)) 
int LineCnt=0;					
//C ����·���м���,��ʼ��Ϊ0
char Name[MaxNodeNum][20];		
//C ר�ŶԻ��˽ڵ���б��,�����¼���ǻ��˽ڵ������ 
int NameCnt=0;					
//C �Ի��˽ڵ���м���,��ʼ��Ϊ0 
const int FileMaxNum=2000;
//C ���ļ��е���Ϣ�ļ������ֵ�����ڿ��ŵ���վ 

//C ����վ�ڵ� 
typedef struct StationNode{
	StationNode *h;			//ָ��ͷ���,����ֱ�ӵõ��������վ��������· 
	char name[20];			//����վ������(��ͷ���)/��·����(ͷ���)  
	int number[MaxLineNum];	//վ��λ����·���������飬Ϊ-1��ʾΪ�գ�Ϊ����ֵ��ʾhead���±�,���ɱ�ʾ���Ի��˵���· 
	StationNode *front;		//ǰ��
	int f_weight; 			//��ǰ��Ȩ�� ��ͷ����next������ǻ�����Ϊinfinity,����Ϊinfinity 
	StationNode *next;		//β���ָ�գ�����Ĵ��۲�Ϊ�㣬Ĭ�ϳɻ���ָ����·�ĵ�һ���ڵ�(��ͷ���) 
							//pֱ��ָ��ͷ���->next; ����·�ĵ�һվ
	int n_weight; 			//����Ĵ���
	 //n>1ʱ���ɻ���
}StationNode ,*LinkNode;

//C �ڽӱ�ı���(��ʾ��) 
typedef struct WayNode{
	int way;					//�������ڽӵ���· 
	struct WayNode *nextway;	//ָ����һ���ڽӵı� 
}WayNode;

//C �ڽӱ��ͷ���(��ʾ��)
typedef struct VNode{
	int num;				//��·�ı��
	struct WayNode *first;	//ָ���һ����������·�ڽӵĻ� 
}VNode,v_list[MaxLineNum];

//C �ڽӱ� 
typedef struct MGraph{
	int n,e;				//������ͱ���//����û�õ� 
	v_list vertice;			//��·����
							//���ۻ��˴���������
}MGraph;

//C Ȩ�ؾ��� 
typedef struct NGraph{
	int adjvex[MaxNodeNum][MaxNodeNum]; //���˽ڵ�Ȩ�ؾ���
}NGraph;

//C ������ʱ�洢�ļ�����Ϣ�Ľṹ��,����ɾȥ����վ�Ĳ��� 
typedef struct filewrite
{
	char linename[20];
	char name[20];
	int weight;
	int open;
}filewrite;

int FileCnt=0;
//C ���ڶ���������ṹ�ļ��� 

filewrite f[FileMaxNum];
//C  ������ʱ�洢�ļ�����Ϣ�Ľṹ������ 

//C 
StationNode head[MaxLineNum];//������·��ͷ���

//C
MGraph G1;		//��·ͼ(����·����һ��ͼ��)
NGraph G2;		//�����˽���Ȩ�ؾ���

//F ջ 
typedef struct{
	int *base;
	int *top;
	int nu;
	int stacksize;
}SqStack;

//F ���� 
typedef struct QNode{
	int data;
	struct QNode *next;
}QNode,*QueuePtr;

//F ���� 
typedef struct{
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

//F �ڽӱ�??? 
typedef struct TreeNode{
	int way_locate;
	int next_number;//��ʾ��̽ڵ�ĸ��� 
	struct TreeNode *pointer[15];
	struct TreeNode *front; 
}TreeNode;

typedef int datatype;
//W ���κ��Ķ��� 
struct Queue{
    datatype data[MAXSIZE];
    int length;
    Queue() {
        memset(data, 0, sizeof(data));
        length = 0;
    }
};

//W ���ڴ洢վ���Ӧ�����Ϣ
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
//W ��Ŵ洢��·��Ϣ
int dist[MaxNodeNum];
//W ��¼·������
int st[MaxNodeNum];
//W ��� 
int Prev[MaxNodeNum]; 
//W ��¼·��ǰһ��
int DLine[MaxNodeNum][MaxNodeNum];
//W ����ͨ��ͬһ��·������վ�������жϴ�ʱ·�����ڼ�����
int StationNumber;
//��ͬվ�����
int DWeight[MaxNodeNum][MaxNodeNum];
//W ���ڴ洢վ����վ��֮��ľ���/Ȩֵ,δ�ҵ��򷵻�-1
int P[MaxNodeNum][MaxNodeNum];
//W ���ڼ�¼ǰһ·��������

void InitStack(SqStack *S);
int GetTop(SqStack *S);
void Push(SqStack *S,int e);
void Pop(SqStack *S,int *e);
//F 1.0ջ���� 

void InitQueue(LinkQueue *Q);
void DestroyQueue(LinkQueue *Q);
void EnQueue(LinkQueue *Q,int e);
void DeQueue(LinkQueue *Q,int *e);
//F 1.0���в���

void insertnode(StationNode &s,char linename[],char name[],int w);
//C 1.0 ��s�������һ���ڵ㲢��ʼ������վ����,Ȩ�� 
void insertheadnode(StationNode &s,char linename[]);
//C 1.0 ����(��ʼ��)ͷ���,ͷ������·���� 
void addLine(StationNode &p,int j);
//C 1.0 ��p�������վ���j���������·,j��ʾ��head[j]���±� 
void insertv(VNode &p,int i);
//C 1.0 ���ڽӱ�����ӱ�,�������ڰ���·���ɵ���ڽӱ������һ����·����һ����·(��i)�ı�
//i��Ӧ����head[i]���±� 
int get_ChangeStation_num(char name[]);
//C 1.0 ����֪�Ļ���վ���ҵ���Ӧ���±�,��Name[i]���±�
//����Ҳ����᷵��-1 
void CreateGraph(MGraph &G,NGraph &G2);
//C W 1.0 ��ȡ����վ�ļ�,��ʼ������,����·���ɵ��ͼ(G1),�ͻ��˽ڵ�Ȩ�ؾ���(G2)
int getlinenum(char name[]);//�õ������·��Ӧ���Ǽ���ͷ��� 
//C 1.0 ͨ����·�����ֱ������е�ͷ����ҵ���Ӧ��ͷ�����±�,��head[i]���±� 
void PrintLine(int i);
//C 1.0 �����·head[i]������վ�� 
void start_ui();
//C 1.0 ϵͳ�Ŀ�ʼ���� 
void information_check();
//C 1.0 ��Ϣ��ѯ���� 
void information_change();
//C 1.3 ��̨����
void open(char name[]);
//C 1.3 ���ŵ���վ�����ĺ��� 
void close(char name[]);
//C 1.3�رյ���վ�����ĺ��� 
void help();
//C 3.0 �������� 
int cost(char start[],char end[],int n);
//F 1.0 ���ڽӵ�·��֮��ת�Ƶľ�����·������л��������ƣ� ���ش���ֵ 
void change_least_way(MGraph &G);
//F 1.0 �������������· 
void DFSTraverse(int s_way,int e_way,char start[],char end[],MGraph &G,int n);
//F 1.0  Ѱ�һ������ٵ�������·
int change_time(int s_way,int e_way,char start[],char end[],MGraph &G);
//F 1.0 ͳ�ƻ��˴��� 
void best_way(char start[],int change[],char end[],int n);
//F 1.0 �����յ������Լ�����·�����飬�������·��
void change_way(char start[],char end[],int n);
//F 1.0 ��������վ��֮��ľ�����·����վ������ͬһ��·�� 
int distance(char start[],char end[],int n);
//F     ������վ��֮�����С���룬��cost�����Ż� 
int GetLineNumber(char *name); 
//W 1.0 ͨ����·���ƻ����·���
char *GetLineName(int number);
//W 1.0 ͨ����·��Ż�ȡ��·����
int GetSiteNumber(char *name);
//W 1.0 ����վ�����ƻ��վ�����,δ�ҵ��򷵻�-1�����ص���pos[i]���±� 
char *GetSiteName(int number);
//W 1.0 ͨ��վ����Ż�ȡname
int GetDistance(int num1, int num2);
//W 1.0 ��ȡ����վ��֮���ֱ��Ȩֵ���������ֱ�ӿɴ����-1
void Dijkstra(char start[], char end[]);
//W 1.0 ��ȡstartΪ��㣬endΪ�յ�����·��
void Traceback(int v, int i, Queue &Q);
//W 1.0 ������·�� vԴ�㣬i�յ�,QΪ�洢վ��ŵĶ���
void GetPay(char start[], char end[]);
// W 1.0 ��ȡstart��end��Ʊ��
void Fast_Path_UI();
//W 1.0 ���·����ѯui
int Min(int a, int b);
//W 1.0 ���Сֵ
bool Is_NULL(Queue Q);
//W 1.0 �ж϶����Ƿ�Ϊ��
void En_Queue(Queue &Q, int &e);
//W 1.0 ���
void De_Queue(Queue &Q, int &e);
//W 1.0 ����
void Display_Queue(Queue &Q);
//W 1.0 

// 6.25 wjh
int GetWalkDistance(char Sname[], char name1[], char name2[]);
// W 1.0 ��ȡ���˲��о��룬��������ڣ��򷵻�0
void InitChangeWalk(void);
// W 1.0 ��ʼ�����˲��о���
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
	cout<<"Ʊ�۹���:"<<endl;
	cout<<"6����(��)��3Ԫ"<<endl;
	cout<<"6-12����(��)4Ԫ��"<<endl;
	cout<<"12-22����(��)5Ԫ��"<<endl;
	cout<<"22-32����(��)6Ԫ��"<<endl;
	cout<<"32��������ÿ��1Ԫ�ɳ�20���"<<endl;
	printf("�밴Enter��������һ���˵�\n");
	getch();
	system("cls");
} 
void close(char name[])
{
	FILE *fp;
	fp=fopen("2.2.txt","w");//�ļ��������򴴽������������
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
					if(q->next!=NULL)//���ɾ�����������վ�����յ� 
					q->front->n_weight=q->n_weight+q->front->n_weight;
					else if(q->next==NULL&&q->n_weight>=infinity)//���յ㵫���ǻ� 
					{
						q->front->n_weight= q->n_weight+q->front->n_weight;
					}
					else if(q->next==NULL&&q->n_weight<infinity)//���յ㵫�ǻ� 
					{
						q->front->n_weight= q->n_weight+q->front->n_weight;
					}
					if(q->h->next==q)//���ɾ�������վ
					{
						if(q->f_weight<infinity)//����ǻ�����Ҫ���� 
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
				//��ʵ����һ���� 
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
	cout<<name<<"�ѹر�"<<endl;
	printf("�밴Enter��������һ���˵�\n");
	getch();
	system("cls");
	CreateGraph(G1,G2);
}

void open(char name[])
{
	//�ȼ���������վ�Ƿ��Ǳ��رյ�
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
		cout<<"�õ���վδ���ر�"<<endl;
		return;
	}	
	     		
	ifstream ifp;
	ifp.open("2.2.txt",ios::in);
	char linename[20];//��ʾ��ǰ���ڼ�¼�� 
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
	
	for(int i=0;i<cnt;i++)//����Ϊԭ����Ȩֵ 
	{
		int j=num[i];
		if(strcmp(f[j-1].linename,f[j-1].name)==0)//������ŵĵ���վ�����վ
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
	cout<<name<<"�ѿ���"<<endl;
}

void information_change()
{
	while(1)
	{
		system("cls"); 
		cout<<endl<<endl<<endl<<endl<<endl; 
	    cout<<"                             �ӭ����ϵͳ��̨��                             "<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".......                     ����1.>���ŵ���վ                          ......."<<endl;
	    cout<<".......                     ����2.>�رյ���վ                          ......."<<endl;
	    cout<<".......                     ����0.>�ص���һ���˵�                      ......."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
		char a;
		scanf("%s",&a);
		if(a=='1')
		{
			cout<<"������Ҫ�����ĵ���վ";
			char name[20];
			cin>>name;
			open(name);
			printf("�밴Enter��������һ���˵�\n");
	        getch();
	        system("cls");
	        break;
		}
		if(a=='2')
		{
			while(1)
			{
				cout<<"������Ҫ�رյĵ���վ:";
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
				if(sign==0) cout<<"վ�㲻�������������룡"<<endl;
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
	//���Ϊn����·����㵽�յ��·�� 
	//printf("jjjj");
	StationNode *p;
	int sum=0;
	//j��������վ��������·�Ƿ�Ϊת�ӵ� 
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

//s_way=2�������ߣ����������±�Ϊһ 

void insertnode(StationNode &s,char linename[],char name[],int w)
{
	StationNode *p=(StationNode *)malloc(sizeof(StationNode));
	p->next=NULL;//�ȳ�ʼ��Ϊ��
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
	s.h=&s;//ָ���ָ 
	s.front=NULL;
	s.next=NULL;
	s.n_weight=infinity;
	s.f_weight=infinity;
	memset(s.number,-1,sizeof(s.number));
}

void addLine(StationNode &p,int j)//��p�������վ���j���������· 
{
	int i=0;
	while(p.number[i]!=-1)i++;
	p.number[i]=j;
//	cout<<p.name<<"�ɻ���"<<head[p.number[i]].name<<endl; 
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
	
    //�Ȱ�ÿ����·ÿ��������뵽�ڴ���
    ifstream ifp;  //�����ļ�������ifp
    StationNode *p;
    p=NULL;//��ʼ��Ϊ�� 
    ifp.open("2.2.txt", ios::in);
    char linename[20];  //��ʾ��ǰ���ڼ�¼��������������·
    char linename1[20];
    char name[20];
    int w;
    int open;
    //��ʼ��վ����������ΪINF
    //��ʼ��վ�㵽վ�����·���ΪINF
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
            p = &head[LineCnt++];  //����LineCnt�Ƕ���·�ļ���
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
				//Ҳ������ͷ��� 
				if(p->front!=p->h&&p!=p->h)//�����ǵ�һ������վ����ΪȨ������������һվ����һվ 
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
        //		p=p->next;//pʼ��ָ�������������һ��
        //		printf("%s %s %d\n",linename,name,w);
    }
    ifp.close();

    //    //��ӡ���DWeight
    //    for (int i = 0; i < 2; i++)
    //        for (int j = 0; j < 2; j++) {
    //            cout << DWeight[i][j] << " ";
    //            if (j == 1) cout << endl;
    //        }

    //���ƻ�����Ϣ
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

    //��ʼ��MGraph,������·���ɵ��ͼ���ڽӱ�
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

    //��ʼ�� G2
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
                    // n1,n2�ֱ����������˽ڵ�ı��
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

int getlinenum(char name[])//�õ������·��Ӧ���Ǽ���ͷ��� 
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
	p=&head[i];//pʼ��ָ��ͷ���
	q=p;
	while(q!=NULL)
	{
		if(q==p)//qΪͷ���
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
			cout<<endl<<"ע�����·Ϊ����!!!"<<endl;
		}
		q=q->next;
		
	}
	cout<<endl;
}

void start_ui()
{
	system("cls");
    cout<<endl<<endl<<endl<<endl<<endl; 
    cout<<"                     ���������·��ѯ���滮ϵͳ��ӭ�� ��                    "<<endl;
    cout<<".............................................................................."<<endl;
    cout<<".............................................................................."<<endl;
    cout<<".............................................................................."<<endl;
    cout<<".......                     ����1.>������Ϣ��ѯ                        ......."<<endl;
    cout<<".......                     ����2.>���·����ѯ                        ......."<<endl;
    cout<<".......                     ����3.>���ٻ��˲�ѯ                        ......."<<endl;
    cout<<".......                     ����4.>����                                ......."<<endl;
    cout<<".......                     ����0.>�˳�ϵͳ                            ......."<<endl;
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
	    cout<<"                             �������Ϣ��ѯ��"<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".............................................................................."<<endl;
	    cout<<".......                     ����1.>����·���Ʋ�ѯ                      ......."<<endl;
	    cout<<".......                     ����2.>��վ�����Ʋ�ѯ                      ......."<<endl;
	    cout<<".......                     ����3.>��ѯ��·������Ϣ                    ......."<<endl;
	    cout<<".......                     ����0.>�ص���һ���˵�                      ......."<<endl;
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
					cout<<"����"<<i+1<<".  >"<<p->name<<endl; 
				}
				cout<<"����0.  >������һ���˵�"<<endl;
	        	cout<<endl<<endl<<endl<<"�������Ӧ���ֲ�ѯ�����·��Ϣ:";
	        	int c;
	        	cin>>c;
	        	if(c>=1&&c<=LineCnt)
	        	{
	        		PrintLine(c-1);
	        		printf("�밴Enter��������һ���˵�");
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
	   			cout<<"            >>>>>>>>>������Ҫ��ѯ��վ������";
	   			char name[20];
	   			cin>>name;
	   			StationNode *p;
	   
	     		//�ҵ�����Ϊname�Ľ�� 
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
	   			if(strcmp(p->name,name))cout<<"վ�㲻�������������룡"<<endl;
	   			else
	   			{
	   				cout<<"��վ�����ڵĵ�����·�У�"<<endl;
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
	                printf("�밴Enter��������һ���˵�");
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
				cout<<"��ѯ��·���˵������Ϣ"<<endl;
				cout<<"��������·����:";
				char name[20];
				cin>>name;
				if(getlinenum(name)==-1)
				{
					cout<<"վ�㲻�������������룡"<<endl;
				}
				else
				{
					WayNode *p;
					p=G1.vertice[getlinenum(name)].first;
					cout<<name<<"�ɻ���:"<<endl;
					while(p!=NULL)
					{
						cout<<head[p->way].name<<endl;
						p=p->nextway;
					}
					printf("�밴Enter��������һ���˵�");
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
	int frontway[20];//��¼��·��ǰһ����· 
	int change_ci[10];//���������תʮ�� 
	for(int i=0;i<10;i++)
	change_ci[i]=-1;
	int b,j=0;
	int  *u=&b;
	for(int i=0;i<MaxLineNum;i++)
	{visited[i]=0;
	frontway[i]=-1;//������ǰ���� 
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
	//printf("��С���˷�ʽ��·Ϊ��\n");
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
	cout << "                             ��˴������ٲ�ѯ��" << endl;
	int nodes_locate[20];//��λ�������·�� 
	int j=1;
	char name_s[20];
	while(1)
	{
			//վ����������·���Ϊ���� 
			printf("���������վ������"); 
			cin>>name_s;
			StationNode *p;
			
					//�ҵ�����Ϊname�Ľ�� 
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
			if(sign==0)cout<<"վ�㲻�������������룡"<<endl;
			else
			{
				//cout<<"��վ�����ڵĵ�����·�У�"<<endl;
				StationNode *q;
				q=p->h;
				nodes_locate[0]=getlinenum(q->name);
				//PrintLine(nodes_locate[0]);//Ҫע�� 
				 int i=0;//j����ڵ�λ����·������±� 
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

	int nodee_locate[20];//��λ�յ�����·�� 
	int m=1;
	char name_e[20];
			while(1)
			{
				printf("�������յ�վ������");			
				//վ����������·���Ϊ���� 
				cin>>name_e;
				StationNode *p;
			
					//�ҵ�����Ϊname�Ľ�� 
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
				if(sign==0)cout<<"վ�㲻�������������룡"<<endl;
				else
				{
					//cout<<"��վ�����ڵĵ�����·�У�"<<endl;
					StationNode *q;
					q=p->h;
					nodee_locate[0]=getlinenum(q->name);
					//PrintLine(nodee_locate[0]);//Ҫע��
					 int i=0;//j����ڵ�λ����·������±�
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
	}//һ��վ�����ͬʱλ�ڶ����·�� 
	for(int i=0;i<j;i++)
	{
		for(int n=0;n<m;n++)
		{
			if(change_time(nodes_locate[i],nodee_locate[n],name_s,name_e,G1)==min)
			DFSTraverse(nodes_locate[i],nodee_locate[n],name_s,name_e,G1,min+1);
			printf("\n");
		}
	}
			printf("�밴Enter��������һ���˵�");
		    getch();
		    system("cls");
}

void DFSTraverse(int s_way,int e_way,char start[],char end[],MGraph &G,int n){
	if(s_way==e_way){
		printf("���軻�ˣ�\n");
		change_way(start,end,s_way);
		int t=0,sum=distance(start,end,e_way);
		printf("%s\n",end);
		GetPay(start,end);
		printf("\n");
		t=sum/SubwaySpeed/60;//
		int hh = t / 60;
    	int mm = t - hh * 60;
    	if (hh == 0) 
		cout << "��ʱΪ��" << mm << "����";
    	else { cout << "��ʱΪ��" << hh << "Сʱ" << mm << "����"; }
		printf("\t\t\t�����ܾ���Ϊ��0��\n");
		
		return; 
	} 
	TreeNode Way[MaxLineNum];//�ٽ���·���� 
	struct WayNode *p;
	for(int i=0;i<MaxLineNum;i++)
	{
		for(int j=0;j<15;j++)
		{
			Way[i].pointer[j]=NULL;
		}
	}//pointerָ��ȫ����ʼΪnull 
	for(int i=0;i<MaxLineNum;i++)
	{
		p=G.vertice[i].first;
		Way[i].way_locate=i;
		int j=0;
		while(p!=NULL)
		{
			Way[i].pointer[j]=&Way[p->way];//��·����·֮������ 
			p=p->nextway;
			j++;
		}
		Way[i].next_number=j; 
	}
	//TreeNode�е�front����û���õ�

	int deep;
	 struct TreeNode *q[10];
	 q[0]=&Way[s_way];
	//int change[10][n];//���ʮ��ת��·�� 
	printf("Ϊ���滮���»���������·��\n\n");
	for(int i=0;i<q[0]->next_number;i++)
	{	
		deep=2;//��һ��ѭ�� 
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
		 	deep=3;//�ڶ���ѭ�� 
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
					}//������ѭ��	
				}//������ѭ�� 
			}//������ѭ�� 
		}//�ڶ���ѭ�� 
	}//��һ��ѭ��
	//ÿһ�ر�ʾ����һ�� 
}


void best_way(char start[], int change[], char end[], int n) {
    // change�����ʾ˳�򻻳˵Ľڵ�
    InitChangeWalk();
    char *front_station[10];
    front_station[0] = start;
    front_station[n] = end;
    int sum = 0;
    int walk_dis = 0;
    for (int i = 0; i < n - 1; i++) {
        StationNode *p = &head[change[i]];  //�ȳ�ʼ��Ϊ�ʼ���ڵ���·
        int num = 0, flag = 0;
        int min;
        char *zhuan[5];
        int zh[5];
        while (p) {
            p = p->next;
            int c = 0;
            while (p->number[c] != -1) {
                if (p->number[c] == change[i + 1])  //��Ϊ������i+1����ѭ��������n-1
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
    if (hh == 0) cout << "��ʱΪ��" << mm << "����";
    // 6.26
    else {
        cout << "��ʱΪ��" << hh << "Сʱ";
        if (mm != 0) cout << mm << "����";
    }
    printf("\t\t\t�����ܾ���Ϊ��%d��\n", walk_dis);
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

//��ȡ���˲��о��룬��������ڣ��򷵻�0
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
	int sum=0;//sum����һ����·����Ȩֵ 

	int circle=0;//�����·�Ƿ�Ϊ�� 
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
				p=p->front;//�����׽��ָ������ָ��ָ��ͷ���ʱֱ��������ֹ�������Ϣ 
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
//����վ�����ƻ��վ�����,δ�ҵ��򷵻�-1
int GetSiteNumber(char name[]) {
    for (int i = 0; i < StationNumber; i++) {
        if (strcmp(pos[i].name, name) == 0) { return i; }
    }
    return -1;
}

//ͨ��վ����Ż�ȡname
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

//���Сֵ
int Min(int a, int b) { return a < b ? a : b; }

//��ȡstartΪ��㣬endΪ�յ�����·��
void Dijkstra(char start[], char end[]) {
    GetPay(start, end);
    memset(dist, 0x3f, sizeof(dist));
    int s = GetSiteNumber(start);
    int e = GetSiteNumber(end);
    int mindis, dis;
    //��ʼ��
    // dist[i]��ʾs��i����С����
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
    //    cout<<dist[GetSiteNumber("������ѧ԰")]+DWeight[GetSiteNumber("������")][GetSiteNumber("������ѧԺ")]<<endl;
    cout << endl << endl;
    cout << "Ϊ���滮��[" << start << "]��[" << end << "]�������·Ϊ��" << endl << endl;
    Queue Q;
    Traceback(s, e, Q);
    // cout << endl << "Show Queue" << endl;
    // Display_Queue(Q);
    cout << endl << endl;
    cout << "���廻����Ϣ���£�" << endl;
    cout << "�� [" << start << "] ����--> ";
    int WalkDistance = 0;
    if (Q.length == 2) {
        int u, t;
        De_Queue(Q, u);
        De_Queue(Q, t);
        int line0 = DLine[u][t];
        cout << "���{" << GetLineName(line0) << "}" << endl;
    } else {
        int preLine = infinity;  //��¼ǰһվ������·
        int nowLine = infinity;  //��¼����������·
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
                    cout << "���{" << GetLineName(preLine) << "} --> ";
                    cout << "��[" << GetSiteName(t_n) << "]���� --> ";
                }
            }
            preLine = nowLine;
            t_p = t_n;
        }
        // 6.26
        cout << "���{" << GetLineName(preLine) << "} --> ";
    }

    cout << "����Ŀ��վ[" << end << "]" << endl << endl;
    cout << "���о���Ϊ" << WalkDistance << "��" << endl << endl;
    int Sum_Time = ceil((dist[e] * 1.0 / SubwaySpeed + WalkDistance * 1.0 / WalkSpeed) / 60);  //����ʱ����λ����
    // cout << Sum_Time << endl;
    int hh = Sum_Time / 60;
    int mm = Sum_Time - hh * 60;
    if (hh == 0) cout << "��ʱ��ԼΪ" << mm << "����" << endl;
    // 6.26
    else {
        cout << "��ʱΪ��" << hh << "Сʱ";
        if (mm != 0) cout << mm << "����";
    }
}
//������·�� vԴ�㣬i�յ�
void Traceback(int v, int i, Queue &Q) {
    // Դ������յ�ʱ�����ҳ�ȫ��·��
    if (v == i) {
        En_Queue(Q, i);
        cout << "[" << GetSiteName(i) << "]";
        return;
    }
    Traceback(v, Prev[i], Q);
    cout << "-->[" << GetSiteName(i) << "]";
    En_Queue(Q, i);
}

//���·����ѯui
void Fast_Path_UI() {
    system("cls");
    cout << endl << endl << endl << endl << endl;
    cout << "                             �����·����ѯ��" << endl;
    cout << ".............................................................................." << endl;
L1:
    cout << "            >>>>>>>>>��������ʼվ������";
    char start[20];
    cin >> start;
    StationNode *p;

    //�ҵ�����Ϊstart�Ľ��
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
        cout << "վ�㲻�������������룡" << endl;
        goto L1;
    }

L2:
    cout << "            >>>>>>>>>�������յ�վ������";
    char end[20];
    cin >> end;

    //�ҵ�����Ϊend�Ľ��
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
        cout << "վ�㲻�������������룡" << endl;
        goto L2;
    }
    Dijkstra(start, end);
    cout << endl;
    printf("�밴Enter��������һ���˵�");
    getch();
}
//�ж϶����Ƿ�Ϊ��
bool Is_NULL(Queue Q) { return (Q.length == 0); }

//���
void En_Queue(Queue &Q, int &e) {
    Q.data[Q.length] = e;
    Q.length++;
}

//����
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
    //��ʼ��
    // dist[i]��ʾs��i����С����
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
    mindis = dist[e];  //��start��end�����·������

    int pay;
    mindis = ceil(mindis * 1.0 / 1000);  //��λ����ת�������ȡ��
    // 6����(��)��3Ԫ��6-12����(��)4Ԫ��12-22����(��)5Ԫ��22-32����(��)6Ԫ��32��������ÿ��1Ԫ�ɳ�20���
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

    cout << "��[" << start << "]��[" << end << "]��Ʊ��Ϊ " << pay << "Ԫ";
}
