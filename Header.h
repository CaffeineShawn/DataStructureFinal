//
//  Header.h
//  ds
//
//  Created by Shaw Young on 2020/12/23.
//

#ifndef Header_h
#define Header_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#define OVERFLOW -1
#define ERROR -1
#define SUCCESS 1
#define OK 1
#define TRUE 1
#define FALSE 0
#define Status int

typedef struct Information{//成员的信息
char name[99];
char gender[10];
int age;
int gen;
char dear_name[10];
int degree = 0;
}Inf;


typedef struct Member{
  Inf data;
  struct Member *Child, *Brother, *Parent;
} Member, *Genealogy;


typedef struct LQNode {
  Genealogy  Member;
  struct LQNode  *next;
} LQNode, *QueuePtr;


typedef struct {
  QueuePtr  front;  // 队头指针
  QueuePtr  rear;   // 队尾指针
} LQueue; // 链队列类型

void InitQueue_LQ(LQueue &Q){ // 构造一个空队列 Q
  Q.front = Q.rear = NULL;
}

int InitGenealogy(Genealogy &G){
    G = (Genealogy)malloc(sizeof(Member));
    if(G == NULL) return OVERFLOW;
    G->Child = G->Brother = G->Parent = NULL;
    return OK;
}

Genealogy SearchMember(Genealogy G, char* name);

void NewMember(Genealogy &G){
    Genealogy p;
    InitGenealogy(p);
    char name[99], sex[10], dear[10];
    int age;
    printf("Please input the name of the member you want to insert：");
    gets(name);
    fflush(stdin);
    while(1){
        if(name[0] >= 'a' && name[0] <= 'z' || name[0] >= 'A' && name[0] <= 'Z'){
            if(SearchMember(G, name))
                printf("ERROR:Duplicated name was found.\n");
                
            else{
                strcpy(p->data.name, name);
                break;
            }
        }
        else{
            printf("Wrong name input(characters only).\n");
        }
            printf("Please input the name of the member you want to insert：");
            gets(name);
        
            
    }
    while(1){
        printf("Selecting %s's gender(male or female)：", name);
        gets(sex);
        if(!strcmp(sex, "male") || !strcmp(sex, "female"))
            break;
        else
            printf("Wrong gender input\n");
    }
    strcpy(p->data.gender, sex);
    while(1){
        printf("Please input the age of %s：", p->data.name);
        int flag = scanf("%d", &age);
        fflush(stdin);
        if(!flag)
            printf("Wrong age input\n");
        
        else{
        p->data.age = age;
        break;
        }
    }
    while(1){
        int flag = 0;
        printf("Please input the name %s's partner(Name or any non-characters for NULL):", p->data.name);
        scanf("\n%s", dear);
        fflush(stdin);
        for(int i = 0; i < strlen(dear); i++){//对输入的伴侣名字进行校验
            if((dear[i] >= 65 && dear[i] <= 90) || (dear[i] >= 97 && dear[i] <= 122))
                ;
            else {
                flag = -1;break;
            }
        }
        if(flag == -1){
            printf("%s has no partners.\n", p->data.name);
            strcpy(p->data.dear_name, "NULL");
        }
        else{
            strcpy(p->data.dear_name, dear);
        }
        break;
    }
    if(G){
        p->data.gen = G->data.gen+1;
        p->Brother = G->Child;//头插法插入孩子结点
        G->Child = p;
        p->Parent = G;
        G->data.degree++;
        printf("=================Added=Successfully=================\n\n\n");
    }
    else{
        p->data.gen = 1;
        G=p;
        printf("================Created=Successfully================\n\n\n");
    }
}

int QueueEmpty_LQ(LQueue Q){
  // 若队列 Q 为空，则返回TRUE，否则FALSE
  if(NULL==Q.front) return TRUE;
  else return FALSE;
}

void InfoOutput(Genealogy p){
    if(p)
        printf("%s's Info: Age:%d, Partner:%s, Gender:%s, Generation%d.\n", p->data.name, p->data.age, p->data.dear_name, p->data.gender, p->data.gen);
}

void CouldNotFoundMember(Genealogy p){
    printf("Search Unsuccessfully:Could not find member in the genealogy.\n\n\n");
}

void GUIPrint(){
    printf(" ===================================================\n");
    printf("||         System for Genealogy Management         ||\n");
    printf("||                                                 ||\n");
    printf("||             Choose Your Opration                ||\n");
    printf("||                                                 ||\n");
    printf("||                 A.AddMemInfo                    ||\n");
    printf("||                                                 ||\n");
    printf("||    C.CreateFamily        I.RelationInquire      ||\n");
    printf("||                                                 ||\n");
    printf("||    D.DeleteMember        J.MemRelationJudge     ||\n");
    printf("||                                                 ||\n");
    printf("||    E.EXIT                M.ModifyInfo           ||\n");
    printf("||                                                 ||\n");
    printf("||    O.OutGenealogy        S.SearchMember         ||\n");
    printf("||                                                 ||\n");
    printf(" ===================================================\n");
}

int DeQueue_LQ(LQueue &Q, Genealogy &e) {
     // 若队列Q非空，则删除队头元素，用 e 返回其值，并返回OK；否则返回ERROR
    LQNode *p;
    if(NULL==Q.front) return ERROR;
    p = Q.front;   // 指向队头结点
    e = p->Member;   // 保存队头结点的数据
    Q.front = p->next;
    if(Q.rear==p) Q.rear = NULL; // 队列只有一个结点，删去后队列变空
    free(p);                 // 释放被删队头结点
    return OK;
}

int EnQueue_LQ(LQueue &Q, Genealogy e){
   LQNode *p;
   p = (LQNode*)malloc(sizeof(LQNode)); // 申请新结点
   if(NULL==p) return OVERFLOW;
   p->Member = e;
   p->next = NULL;
   if(NULL==Q.front) Q.front = p; // e插入空队列
   else Q.rear->next = p; // e插入非空队列
   Q.rear = p; // 队尾指针指向新的队尾
   return OK;
}




Genealogy SearchMember(Genealogy G, char* name){
   Genealogy p = NULL;
   if(G){
    if(strcmp(G->data.name, name) == 0) return G;
    if(G->Child)
        p = SearchMember(G->Child, name);
    if(p) return p;
    if(G->Brother)
        p = SearchMember(G->Brother, name);
    if(p) return p;
   }
   return p;
}

void DeleteDescendants(Genealogy &G){
    if(G){
        DeleteDescendants(G->Child);
        DeleteDescendants(G->Brother);
        free(G);
        G = NULL;
    }
}
void DeleteMember(Genealogy &G){
    if(G){
        DeleteDescendants(G->Child);
        Genealogy p = G;
        if(p->Parent){
            p = p->Parent->Child;
            if(p->data.name == G->data.name)
                G->Parent->Child = G->Brother;
            else{
                while(strcmp(p->Brother->data.name, G->data.name) != 0)
                    p = p->Brother;
                p->Brother = G->Brother;
            }
        }
    }
}


int GenerationSearch(Genealogy G, char *name){
    if(!G){
        
        return 0;
    }
    int i = 1, j = 0;
    if(G->data.name == name)
        return 1;
    i += GenerationSearch(G->Child, name);
    j += GenerationSearch(G->Brother, name);
    return i > j? i : j;
}

void EditedSucc(){
    printf("=================EditedSuccessfully=================.\n");
}

void ModifyInformation(Genealogy &G){
    char operation = '\0';
    char name[99],sex[10];
    int age;
    Genealogy p;
        while(operation != 'E'){
        printf("What kind of %s's info you want to edit?(A.Age, E.EXIT, G.gender, N.Name):", G->data.name);
        scanf("%c", &operation);
        fflush(stdin);
        switch (operation){
            case 'A':
                while(1){
                printf("Please input the correct age:");
                    int flag = scanf("%d", &age);
                    fflush(stdin);
                    if(!flag)
                        printf("Wrong age input\n");

                    else{
                        G->data.age = age;
                        break;
                    }
                    
                break;
                }
                
            case 'E':
                printf("===============Exit=InformationModify===============\n\n\n");
                break;
            
            case 'G':
                while(1){
                    printf("Please input the correct gender(male or female):");
                    gets(sex);
                    if(!strcmp(sex, "male") || !strcmp(sex, "female") )
                        break;
                    else
                        printf("You've input an unsupported gender. Please try again.\n");
                    }
                    strcpy(G->data.gender, sex);
                    EditedSucc();
                break;

           
                
            case 'H':
                printf("What kind of info you want to edit?(A.Age, E.EXIT, G.gender, N.Name):");
                break;
                
            case 'N':
                while(1){
                    printf("Please input the correct name of the member :");
                    gets(name);
                    p = SearchMember(G, name);
                    if(!p){
                        strcpy(p->data.name, name);
                        break;
                    }
                    else
                        printf("ERROR:Duplicated name with existing members. \n");
                }
                EditedSucc();
                break;


            default :
                printf("Wrong operation. Input 'H' for help.\n");
                break;
            }
        }
}

void OutGenealogy(Genealogy G){ //层次遍历的方式输出家族所有成员信息
    LQueue Q;
    InitQueue_LQ(Q);
    Genealogy p = G;
    EnQueue_LQ(Q, p);
    while(DeQueue_LQ(Q, p) == OK){
        InfoOutput(p);
        if(p->Child){
            p = p->Child;
            EnQueue_LQ(Q, p);
            while(p->Brother){
                p = p->Brother;
                EnQueue_LQ(Q, p);
            }
        }
    }
    printf("===================End=of=Output====================\n\n\n");
}

void EndofInquiry(){
    printf("====================End=of=inquiry====================\n\n\n");
}

void RelationalQuery(Genealogy G){
    char section;
    int flag = 0;
    Genealogy p = G;
    Genealogy tmp;
    LQueue Q;
    InitQueue_LQ(Q);
    EnQueue_LQ(Q, p);
    printf("Please input the kind of relationship you want to inquire(A.descendants, B.Ancestors, C.cousins):");
    scanf("%c", &section);
    fflush(stdin);
    switch(section){
    case 'A':
        if(!p->Child){
            printf("%s got no descendants\n\n\n", p->data.name);
            break;
        }
        while(DeQueue_LQ(Q,p) == OK){
            if(flag != 0)
                InfoOutput(p);
            if((Q.front && Q.front->Member->data.gen > p->data.gen) || p->data.gen == G->data.gen ||(!Q.front && p->Child)){
                flag++;
                switch (flag){
                    case 1:
                        printf("His children's info are listed as follows:\n");break;
                    case 2:
                        printf("His grandchildren's info are listed as follows:\n");break;
                    case 3:
                        printf("His great grandchildren's info are listed as follows:\n");break;
                    default:
                        printf("More info about his far descendants are list as follows:\n");break;
                }
            }
            if(p->Child){
                p = p->Child;
                EnQueue_LQ(Q, p);
                while(p->Brother){
                    p = p->Brother;
                    EnQueue_LQ(Q, p);
                }
            }
        }
            EndofInquiry();
        break;

    case 'B':
        if(!p->Parent){
            printf("%s got no ancestors\n\n\n", p->data.name);
            break;
        }
        else{
            p = p->Parent;
            while(1){
                switch (flag){
                    case 0:
                        printf("His parents's info:");
                        InfoOutput(p);
                        break;
                    
                    case 1:
                        printf("His grandparents's info:");
                        InfoOutput(p);
                        break;
                    
                    case 2:
                        printf("His great grandparents's info:");
                        InfoOutput(p);
                        break;
                    
                    default:
                        printf("More info about his far ancestors:");
                        InfoOutput(p);
                        break;
                    
                }
                if(p->Parent){
                    p = p->Parent;
                    flag++;
                }
                else
                    break;
            }
        }
        EndofInquiry();
        break;

    case 'C':
        if(!p->Parent || !p->Parent->Parent || !p->Parent->Parent->Child->Brother){
            printf("%s got no cousins\n\n\n", p->data.name);
            break;
        }
        DeQueue_LQ(Q, p);
        p = p->Parent->Parent->Child;
            while(p){
                tmp = p->Child;
                while(tmp){
                    EnQueue_LQ(Q, tmp);
                    tmp = tmp->Brother;
                }
                p = p->Brother;
            }
        printf("His cousins info：\n");
        while(DeQueue_LQ(Q, p) == OK){
        if(p->data.name != G->data.name)
            InfoOutput(p);
        }
        EndofInquiry();
        break;

    default:
        printf("Please input the right form of relationship inquiry.\n\n\n");
        break;

    }
}

void OutofReach(){
    printf("The relationship between them is out of reach.\n\n\n");
}

void JudgedSucceed(){
    printf("============RelationshipJudgmentSucceed============\n\n\n");
}

int RelationshipJudgment(Genealogy a, Genealogy b){
    Genealogy p1 = a;
    Genealogy p2 = b;
    int flag = 0;
    if(a == b){
        printf("ERROR:They are the same person.\n\n\n");
        return ERROR;
    }
    
    else{
        if(p1->data.gen == p2->data.gen){
            
            if(p1->Parent == p2->Parent){
                if(!strcmp(p2->data.gender, "male"))
                    printf("%s is %s's brother.\n", p2->data.name, p1->data.name);
                else
                    printf("%s is %s's sister.\n", p2->data.name, p1->data.name);
            }
            else{
                printf("%s is %s's cousin.\n", p2->data.name, p1->data.name);
                
            }
            flag = 0;
            JudgedSucceed();
            return OK;
        }
        else{
            Genealogy tmp;
            if(p2->data.gen < p1->data.gen){
                tmp = p1;
                p1 = p2;
                p2 = tmp;
            }//p2是晚辈 p1是长辈

            switch ((p2->data.gen - p1->data.gen)) {
                case 1:
                    
                    if(!(strcmp(p2->Parent->data.name,  p1->data.name))){
                        if(!strcmp(p1->data.gender,"male"))
                            printf("%s is %s's father.\n",p1->data.name, p2->data.name);
                        else
                            printf("%s is %s's mother.\n",p1->data.name, p2->data.name);
                        if(!strcmp(p2->data.gender,"male"))
                            printf("%s is %s's son.\n", p2->data.name, p1->data.name);
                        else
                            printf("%s is %s's daughter.\n", p2->data.name, p1->data.name);
                        JudgedSucceed();
                        return OK;
                    }
                    else{
                        if(!(strcmp(p2->Parent->data.name, p1->data.name))){
                            if(!strcmp(p1->data.gender, "male"))
                                printf("%s is %s's uncle.\n", p1->data.name, p2->data.name);
                            else
                                printf("%s is %s's auntie.\n", p1->data.name, p2->data.name);


                            if(!strcmp(p2->data.gender, "male"))
                                printf("%s is %s's niece.\n", p2->data.name, p1->data.name);
                            else
                                printf("%s is %s's nephew.\n", p2->data.name, p1->data.name);
                            JudgedSucceed();
                            return OK;
                    }
                    break;
                case 2:
                        if(!(strcmp(p2->Parent->Parent->data.name, p1->data.name))){
                            if(!strcmp(p1->data.gender, "male"))
                                printf("%s is %s's grandpa.\n", p1->data.name, p2->data.name);
                            else
                                printf("%s is %s's grandma.\n", p1->data.name,  p2->data.name);
                            if(!strcmp(p2->data.gender, "male"))
                                printf("%s is %s's grandson.\n", p2->data.name, p1->data.name);
                            else
                                printf("%s is %s's granddaughter.\n", p2->data.name, p1->data.name);
                            JudgedSucceed();
                            return OK;
                        }
                        else{
                            if(!strcmp(p1->data.gender, "male"))
                                printf("%s's brother or cousin is %s's grandpa.\n", p1->data.name, p2->data.name);
                            else //if(!strcmp(p1->data.gender,"female"))
                                printf("%s's sister or cousin is %s's grandma.\n", p1->data.name, p2->data.name);
                            if(!strcmp(p2->data.gender, "male"))
                                printf("%s is %s's brother or cousin's grandson.\n", p2->data.name, p1->data.name);
                            else //if(!strcmp(p2->data.gender,"female"))
                                printf("%s is %s's brother or cousin's granddaughter.\n", p2->data.name, p1->data.name);
                            JudgedSucceed();
                            return OK;
                        }
                        break;
                        
                case 3:
                        if(!(strcmp(p2->Parent->Parent->Parent->data.name, p1->data.name))){
                            if(!strcmp(p1->data.gender, "male"))
                                printf("%s is %s's great grandpa.\n", p1->data.name, p2->data.name);
                            else
                                printf("%s is %s's great grandma.\n", p1->data.name, p2->data.name);
                            if(!strcmp(p2->data.gender, "male"))
                                    printf("%s is %s's great grandson.\n", p2->data.name, p1->data.name);
                                else
                                    printf("%s is %s's great granddaughter.\n", p2->data.name, p1->data.name);
                                JudgedSucceed();
                                return OK;
                            }
                            else{
                                if(!strcmp(p1->data.gender, "male"))
                                    printf("%s's brother or cousin is %s's great grandpa.\n", p1->data.name, p2->data.name);
                                else //if(!strcmp(p1->data.gender,"female"))
                                    printf("%s's sister or cousin is %s's great grandma.\n", p1->data.name, p2->data.name);
                                if(!strcmp(p2->data.gender, "male"))
                                    printf("%s is %s's brother or cousin's great grandson.\n", p2->data.name, p1->data.name);
                                else //if(!strcmp(p2->data.gender,"female"))
                                    printf("%s is %s's brother or cousin's great granddaughter.\n", p2->data.name, p1->data.name);
                                JudgedSucceed();
                                return OK;
                            }
                            break;
                        
                default:
                        printf("The relationship between %s and %s is way too far.\n", p2->data.name, p1->data.name);
                    break;
            }
            
        }
        
        }
    }
    return ERROR;
}

typedef struct {
    Genealogy ptr;
}ElemType;

typedef struct{
    ElemType *elem;
    int top;
    int size;
    int increment;
}GenealogyStack, GS;

void DestroyStack(GenealogyStack &gs){
    gs.elem = NULL;
}

Status InitGStack(GenealogyStack &gs, int size, int inc){
    if(gs.elem != NULL)DestroyStack(gs);
    gs.elem = (ElemType*)malloc(size*sizeof(ElemType));
    if(gs.elem == NULL)
        return OVERFLOW;
    gs.top = 0;
    gs.size = size;
    gs.increment = inc;
    return OK;
}

Status GSPush(GenealogyStack &gs, ElemType e){
    ElemType *newbase;
    if(gs.top >= gs.size){
        newbase = (ElemType*)realloc(gs.elem, (gs.size + gs.increment)*sizeof(ElemType));
        if(NULL == newbase)
            return OVERFLOW;
        gs.elem = newbase;
        gs.size = gs.increment;
        return OK;
    }
    gs.elem[gs.top++] = e;
    return OK;
}

Status isEmptyGS(GenealogyStack &gs){
    if(gs.top == 0) return TRUE;
    else return FALSE;
}

Status GSPop(GenealogyStack &gs, ElemType e){
    if(isEmptyGS(gs))
        return OVERFLOW;
    else{
        e = gs.elem[gs.top-1];
        gs.top--;
    }
    return OK;
}



/*
void maybeTheresATree(Genealogy G){
    char V_line[6] = "|    ";
    char B_line[6] = "|----";
    Genealogy root = G;
    GS gs;
    InitGStack(gs,100,1);
    ElemType e;
    e.ptr = G;
    GSPush(gs, e);
    while(!isEmptyGS(gs) ){
        GSPop(gs, e);
        while(e.ptr->Child){
            printf("%-5s%s", e.ptr->data.name, B_line);
            e.ptr->data.visited = 1;
            if(e.ptr->Child&&e.ptr->Child->data.visited == 0){
                e.ptr = e.ptr->Child;
                GSPush(gs, e);
            }
        }
        printf("%-5s", e.ptr->data.name);
        e.ptr->data.visited = 1;
        
        while(e.ptr->Brother){
            for(int i = 0; i < e.ptr->data.gen - root->data.gen; i++){
                printf("%s", V_line);
            }
            printf("%-5s\n", e.ptr->data.name);
            e.ptr = e.ptr->Brother;
        }
        GSPop(gs, e);
        if(e.ptr == root) break;
    }
    
}*/



Genealogy TransformtoLevelOrder(Genealogy &G){
    Genealogy newG;
    newG = (Genealogy)malloc(sizeof(G));
    int i = 0;
    LQueue Q;
    InitQueue_LQ(Q);
    Genealogy p = G;
    EnQueue_LQ(Q, p);
    while(DeQueue_LQ(Q, p) == OK){
        newG[i++] = *p;
        if(p->Child){
            p = p->Child;
            EnQueue_LQ(Q, p);
            while(p->Brother){
                p = p->Brother;
                EnQueue_LQ(Q, p);
            }
        }
    }
    return newG;
}

void PrintAsTree(Genealogy &newG){
    ;
    
}



#endif /* Header_h */
