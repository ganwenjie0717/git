#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//定义图书信息结构体
typedef struct
{
    char bno[20];//书号
    char title[50];//书名
    char sno[20];//借阅人学号
    int bStatus;//借阅状态，0表示未借出，1表示借出
} Book;
//定义图书信息的结点结构
typedef struct node
{
    Book book;
    struct node* next;
}ListNode, * LinkList;
//初始化图书信息表
LinkList InitList()
{
    LinkList L;
    L = (ListNode*)malloc(sizeof(ListNode));//为头结点分配内存空间
    if (L == NULL)
    {
        printf("分配空间失败，创建单链表不成功!");
        return NULL;
    }
    L->next = NULL;
    return L;
}
//创建图书信息表
LinkList CreatListL()
{
    Book book;
    LinkList L = InitList();
    ListNode *s, *r;
    char inputBno[20];//用于临时存储输入的图书编号
    int continueInput = 1;//控制输入的标志，为0时结束输入
    r = L;//r初值指向头结点
    while (continueInput)
    {
        scanf("%19s %49s", inputBno,&book.title);
        if  (strcmp(inputBno, "NULL") == 0)
            continueInput = 0;//当输入的书号为NULL时，修改结束标志
        else
        {
            strcpy(book.bno, inputBno);//将临时编号复制到 book.bno 中
            strcpy(book.sno, "NULL");//默认未借出，借阅人学号为NULL
            book.bStatus = 0;//默认借阅状态为0（未借出）
            s = (ListNode*)malloc(sizeof(ListNode));
            if (s == NULL)
            {
                printf("分配空间失败！");
                exit(1);//若分配内存空间失败，则退出程序
            }
            s->book = book;//为新结点添加数据
            r->next = s;//将新结点插入到链表末尾
            r = s;
        }
    }
    r->next = NULL;//单链表的最后一个指针为空
    return L;
}
//输出图书信息表
void PrintList(LinkList L)
{
    ListNode *p;
    p = L->next;
    while (p != NULL)
    {
        printf("%s\t%-17s\t%s\t%d\n", p->book.bno,
               p->book.title, p->book.sno, p->book.bStatus);
        p = p->next;       
    }
}
//查找图书（按书名查找，返回指向目标图书结点的指针）
ListNode* LocateListp(LinkList L, const char* bname)
{
    ListNode *p = L->next;//从链表的第一个实际数据元素开始查找
    while (p != NULL)//若找到匹配的书号，则返回该结点的指针
    {
        if(strcmp(p->book.title,bname) == 0)
           return p;
        p = p->next;//查找下一个结点
    }
    return NULL;
}
//借阅图书
void BorrowBook(LinkList L)
{
    char sno[20];
    char title[50];
    ListNode *foundNode = NULL;
    printf("欢迎借阅图书！请输入您的学号：");
    scanf("%19s", sno);
    while (1)
    {
        printf("请输入要借阅的书名：");
        scanf("%49s", title);
        foundNode = LocateListp(L, title);//根据书名查找图书
        if (foundNode == NULL)
           printf("图书不存在，请重新输入书名！\n");
        else if (foundNode->book.bStatus == 1)
           printf("图书已被借出，请重新输入书名！\n");
        else
        {  //图书可借阅，记录借阅学生的学号并修改借阅状态
            strcpy(foundNode->book.sno, sno);
            foundNode->book.bStatus = 1;
            printf("借阅成功！\n");
            break;
        }
        
    }
}
//归还图书
void ReturnBook(LinkList L)
{
    char sno[20];
    char title[50];
    ListNode *foundNode = NULL;
    printf("开始归还图书，请输入您的学号：");
    scanf("%19s", sno);
    while(1)
    {
        printf("请输入要归还的书名：");
        scanf("%49s", title);
        foundNode = LocateListp(L, title);
        if(foundNode == NULL)
           printf("图书不存在，请重新输入书名！\n");
        else   if(strcmp(foundNode->book.sno, sno)!=0||foundNode->book.bStatus !=1)
           printf("您没有借阅这本书，请检查学号和书名后重新输入！\n");
        else
        {
            strcpy(foundNode->book.sno,"NULL");
            foundNode->book.bStatus = 0;
            printf("图书归还成功!\n");
            break;
        }
        printf("请重新输入您的学号：");
        scanf("%19s", sno);   
    }
}
int main()
{
    LinkList A;
    ListNode *p;
    int ilocate = 1;
    char bname[20];
    printf("请依次输入图书的书号和书名(以书号和书名为NULL结束输入):\n");
    A = CreatListL();
    printf("\n\t\t\t\t图书信息表\n");
    printf("----------------------------------------------\n");
    printf("书号\t\t\t书名\t\t借阅人\t借阅状态\n");
    printf("----------------------------------------------\n");
    PrintList(A);
    printf("----------------------------------------------\n");
    printf("\n");
    printf("请输入需要查找的图书名称：");
    do
    {
        scanf("%19s", bname);
        p = LocateListp(A, bname);
        if (p != NULL)
        {
            printf("您查找的图书信息如下：\n");
            printf("%s\t%s\t\t%s\t%d\n", p->book.bno,
                p->book.title, p->book.sno, p->book.bStatus);
            ilocate = 0;    
        }
        else
           printf("您查找的图书不存在！请重新输入正确的书名: ");
    }while (ilocate);
    printf("\n");
    BorrowBook(A);
    printf("\n\t\t\t\t图书信息表\n");
    printf("---------------------------------------------\n");
    printf("书号\t\t\t书名\t\t借阅人学号\t借阅状态\n");
    printf("---------------------------------------------\n");
    PrintList(A);
    printf("---------------------------------------------\n");
    printf("\n");
    ReturnBook(A);
    printf("\n\t\t\t\t图书信息表\n");
    printf("---------------------------------------------\n");
    printf("书号\t\t\t书名\t\t借阅人学号\t借阅状态\n");
    printf("---------------------------------------------\n");
    PrintList(A);
    printf("---------------------------------------------\n");
    return 0;
}