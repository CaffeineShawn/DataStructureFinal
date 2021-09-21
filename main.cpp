#include "Header.h"

int main(){
    Genealogy G = NULL, p, tmp, newG;
    char operation = '\0';
    char name[99] = "\0";
      
    GUIPrint();
    
    while(operation!= 'E'){
        printf("Choose Your Operation:");
        scanf("\n%c", &operation);
        fflush(stdin);
        switch (operation){
            case 'C'://创建家庭并输入创建人的信息
                DeleteMember(G);
                printf("\n===================CreatingFamily===================\n");
                NewMember(G);
                break;

            case 'S':
                printf("Please input the name that you want to find：");
                gets(name);
                p=SearchMember(G, name);
                if(!p)
                    CouldNotFoundMember(p);
                else
                    printf("Successfully found\n");
                    InfoOutput(p);
                break;

            case 'D':
                printf("Please input the name of the member you want to expell from the genealogy：");
                gets(name);
                               p = SearchMember(G, name);
                               if(!p)
                                   CouldNotFoundMember(p);
                               else{
                                   DeleteMember(p);
                                   if(!p->Parent){
                                       free(G);
                                       G = NULL;
                                   }
                                   else{
                                       free(p);
                                       p = NULL;
                                   }
                                   EditedSucc();
                               }
                               break;

            case 'M':
                printf("Please input the name of the member you want to modify info:");
                gets(name);
                p = SearchMember(G, name);
                if(!p)
                    CouldNotFoundMember(p);
                else
                    ModifyInformation(p);
                break;

            case 'A':
                printf("Who do you want to add descendants:");
                gets(name);
                p = SearchMember(G, name);
                if(!p)
                    CouldNotFoundMember(p);
                else
                    NewMember(p);
                break;

            case 'J':
                printf("Please input the member's name:");
                gets(name);
                p = SearchMember(G, name);
                if(!p){
                    CouldNotFoundMember(p);
                    break;
                }
                printf("Please input another one's:");
                gets(name);
                tmp = SearchMember(G, name);
                if(!tmp){
                    CouldNotFoundMember(p);
                    break;
                }
                RelationshipJudgment(p, tmp);
                break;


            case 'O':
                if(!G)
                    printf("There is no members in the family yet.\n\n\n");
                else{
                    printf("\n=======================Family=======================\n");
                    OutGenealogy(G);
                    
                }break;

            case 'I':
                printf("Please input the name of the member you want to inquire:");
                gets(name);
                p = SearchMember(G, name);
                if(!p)
                    CouldNotFoundMember(p);
                else
                    RelationalQuery(p);
                break;


            case 'E':
                printf("\n========================EXIT========================\n");
                break;
            case 'H':
                GUIPrint();
                break;
            case '\0':
                continue;
            case 'B':
                newG = TransformtoLevelOrder(G);
                PrintAsTree(newG);
                break;
                
                
            case 'T':
                printf("Please input the member's name:");
                gets(name);
                p = SearchMember(G, name);
                if(!p){
                    CouldNotFoundMember(p);
                    break;
                }
                printf("Please input another one's:");
                gets(name);
                tmp = SearchMember(G, name);
                if(!tmp){
                    CouldNotFoundMember(p);
                    break;
                }
                RelationshipJudgment(p, tmp);
                break;
                
                
            default:
                printf("Wanna get some help?Input 'H' for more information.\n");
                break;
        }
    }
return 0;

}
