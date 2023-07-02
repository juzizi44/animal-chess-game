#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define N 8 //设置棋盘大小
FILE *fp;
/*--------------------------------------------全局变量------------------------------------------------------*/

CONSOLE_CURSOR_INFO cci;//隐藏光标
int chess1[N][N];//甲方的数据棋盘
int chess2[N][N];//乙方的数据棋盘
char board[N*2+1][N*4+1];//真实输出棋盘
int p=N/2,q=N/2;//数据棋盘的坐标（乙方初始坐标）
int rest1[8]={0,2,2,2,2,2,2,2};//甲方剩余棋子数目
int rest2[8]={0,2,2,2,2,2,2,2};//乙方实际剩余棋子数目
int rest2_2[8]={0,2,2,2,2,2,2,2};//乙方棋盘外剩余的数目（显示在棋盘下方信息提示区域）
char name[100];//文件名

/*--------------------------------------------用到的函数------------------------------------------------------*/

void setcursorpos(unsigned x, unsigned y);//控制台定位输出字符位置
void hidecursor();//隐藏光标
void print_board();//输出真实棋盘
void creat_chess1();//生成甲方棋盘
void press_key();//读取按键信息
void refresh_rest2();//刷新棋盘下方信息提示区域
int connectt(int b);//关联动物字母和其对应的数字
int judge(int a);//判断乙方的棋盘外是否还有该剩余棋子
void fight();//落子后动物进行战斗
void replay();//复盘函数

/*-------------------------------------------------------------------------------------------------------------*/
int main()
{
    /*注释中，甲方代表敌方，乙方代表我方*/
    int choice;
    printf("\t\t\t\t\t动物棋小游戏\n\n");
    printf("游戏规则:     您与计算机各自拥有14枚动物棋，涉及的动物有象 (E)、虎 (T)、狮 (L)、狼 (W)、\n\
           狗 (D)、猫 (C) 和鼠 (M) 它们拥有各自的等级，等级按此顺序从高到底。游戏开始时，\n\
           对战的双方各有每种动物两只；在棋盘的下方显示您每种动物的数量，敌方的棋子随机地\n\
           排放在棋盘上某些方格内，对应的方格中显示一个‘●’字符，您不知相应的方格中是何\n\
           种动物。您可使用上、下、左、右键移动黑色方块到所需的方格中，按下代表动物的字母\n\
           键将该动物落子到该方格中。如果敌方的动物与您方的动物处于相邻的方格中，则高等级\n\
           的动物会消灭低等级的动物，但鼠会消灭象。若左右两边同时有敌方动物，则先与左侧动\n\
           物进行对战，再与右侧动物对战。被消灭的动物从棋盘中消失，如果是您的动物被敌方消\n\
           灭，则棋盘下方相应动物的数量被更新。您方也可以将黑色方块移动到己方棋子所在的方\n\
           格，按下空格键选中该棋子，然后使用上、下、左、右键移动到合适的方格中后，再次按\n\
           下空格键将该棋子重新落子到目标方格中。以这样的方式一直进行游戏，直到一方的动物\n\
           被另一方完全消灭，则还有剩余棋子的一方获胜。如果最后双方均只有一枚棋子剩余，而\n\
           且是相同的动物，则平局。\n\n");
    printf("模式1：复盘\n模式2：新一轮游戏\n\n");
    printf("请输入选择模式的序号：");
    scanf("%d",&choice);
    if(choice==2)//选择进行新一轮游戏
    {
        system("cls");
        time_t seconds;
        seconds=time(NULL);
        int m=-1;
        while(seconds)//根据时间建一个文件
        {
            name[++m]=seconds%10+'0';
            seconds/=10;
        }
        name[++m]='.';
        name[++m]='t';
        name[++m]='x';
        name[++m]='t';
        name[++m]='\0';
        fp=fopen(name,"w");
        srand((unsigned)time(NULL));
        setcursorpos(0, 0);
        print_board();//输出真实棋盘
        refresh_rest2();
        creat_chess1();//生成甲方棋盘
        setcursorpos(4*q+2, 2*p+1);//将光标移到键盘中央
        while(1)
        {
            press_key();//读取按键信息
            //判断结果
            if(rest1[1]==0&&rest1[2]==0&&rest1[3]==0&&rest1[4]==0
               &&rest1[5]==0&&rest1[6]==0&&rest1[7]==0&&rest2[1]==0
               &&rest2[2]==0&&rest2[3]==0&&rest2[4]==0&&rest2[5]==0
               &&rest2[6]==0&&rest2[7]==0)
            {
                printf("\n\n\n平局");printf("您的游戏记录在%s中",name);break;
            }
            else if(rest1[1]==0&&rest1[2]==0&&rest1[3]==0&&rest1[4]==0
                    &&rest1[5]==0&&rest1[6]==0&&rest1[7]==0)
            {
                printf("\n\n\n恭喜您赢了");printf("您的游戏记录在%s中",name);break;
            }
            else if(rest2[1]==0&&rest2[2]==0&&rest2[3]==0&&rest2[4]==0&&
                    rest2[5]==0&&rest2[6]==0&&rest2[7]==0)
            {
                printf("\n\n\n很遗憾您输了");printf("您的游戏记录在%s中",name);break;
            }
        }
        setcursorpos(0,24);
        printf("                                              ");
        setcursorpos(0,25);
        printf("                                              ");
        printf("\n游戏结束");
        fclose(fp);
    }
    if(choice==1)
    {
        printf("请输入文件名：\n");
        scanf("%s",name);
        fp=fopen(name,"r");
        system("cls");
        hidecursor();
        replay();
        fclose(fp);
    }
}
void refresh_rest2()//刷新棋盘下方信息提示区域
{
    setcursorpos(0,18);
    printf("我方棋盘外剩余棋子数目：\n");
    printf("e:%d t:%d l:%d w:%d d:%d c:%d m:%d",rest2_2[1],rest2_2[2],
           rest2_2[3],rest2_2[4],rest2_2[5],rest2_2[6],rest2_2[7]);
}

void setcursorpos(unsigned x, unsigned y)//控制台定位输出字符位置
{
    COORD pos;
    pos.X=x;
    pos.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
void print_board() //输出真实棋盘
{
    int i,j;
     for(i = 0; i < N*2+1; i++)
    {
        for(j = 0; j < N*4+1; j++)
        {
            if(i == 0 && j == 0)
                     board[i][j]='a';//"┏";
            else if(i == 0 && j == (N*4+1)-1)
                     board[i][j] = 'b';//"┓";
            else if(i == (N*2+1)-1 && j == (N*4+1)-1)
                     board[i][j] = 'c';//"┛";
            else if(i == (N*2+1)-1&&j == 0)
                     board[i][j]='d';//"┗";
            else if(i == 0 && j % 4 == 0)
                     board[i][j]='e';//"┯";
            else if(i == (N*2+1)-1 && j % 4 == 0)
                     board[i][j] = 'f';//"┷";
            else if(i % 2 == 0 && j == 0)
                     board[i][j] = 'g';//"┠";
            else if(i % 2 == 0 && j == (N*4+1)-1)
                     board[i][j] = 'h';//"┨";
            else if((i == 0 || i == (N*2+1)-1) && (j/2) % 2 != 0)
                     board[i][j] = 'i';//"━";
            else if(i != 0 && i != (N*2+1)-1 && i % 2 == 0 && (j/2) % 2 != 0)
                     board[i][j] = 'j';//"—";
            else if(i % 2 != 0 && (j == 0 || j == (N*4+1)-1))
                     board[i][j] = 'k';//"┃";
            else if(i % 2 != 0 && j != 0 && j != (N*4+1)-1 && j % 4 == 0)
                     board[i][j] = 'l';//"│";
            else if(i != 0 && j != 0 && i != (N*2+1)-1 && j != (N*4+1)-1 && i % 2 == 0 && j % 4 == 0)
                     board[i][j] = 'm';//"┼"
        }
    }
   for(i = 0; i < N*2+1; i++)
    {
        for(j = 0; j < N*4+1; j++)
        {
            switch(board[i][j])
            {
                case 'a' : printf("┏"); break;
                case 'b' : printf("┓"); break;
                case 'c' : printf("┛"); break;
                case 'd' : printf("┗"); break;
                case 'e' : printf("┯"); break;
                case 'f' : printf("┷"); break;
                case 'g' : printf("┠"); break;
                case 'h' : printf("┨"); break;
                case 'i' : printf("━"); break;
                case 'j' : printf("—"); j++; break;//'—'占两个字符
                case 'k' : printf("┃"); break;
                case 'l' : printf("│"); break;
                case 'm' : printf("┼"); break;
                case ' ' : printf(" "); break;
                default  : printf("%c", board[i][j]);
            }
        }
        printf("\n");
    }
}
void creat_chess1()//生成甲方棋盘
{
    srand((unsigned)time(NULL));
    int x,y,i,j;
    for(i=1;i<=14;i++)//循环14次随机生成甲方的棋子（一共14个棋子）
    {
        //生成0-8之间的随机数不包含8
        x=rand()%N;//横坐标
        y=rand()%N;//纵坐标
        if(chess1[x][y]==0)//如果生成的坐标发生了重复，则i--重新进行该次循环
        {
            chess1[x][y]=(i+1)/2;
        }
        else i--;
    }
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            if(chess1[i][j]!=0)
            {
                setcursorpos(4*j+2, 2*i+1);//甲方棋子在真实棋盘中的坐标
                fprintf(fp,"%d %d %d\n",i,j,chess1[i][j]);
                printf("●");//输出甲方棋子
            }
        }
    }
}
void hidecursor()//隐藏光标
{
    static char bFirst=1;
    if(bFirst){
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
        bFirst = 0;
    }
    CONSOLE_CURSOR_INFO c=cci;
    c.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
}
void press_key()//读取按键信息
{
    hidecursor();
    setcursorpos(4*q+2,2*p+1);//显示出来光标现在的位置
    printf("▇");
    int input;
    input=getch();//输入按键
    if(input==0xE0)//0xE0代表输入的是控制键
    {
        input=getch();//方向键需要getch（）两次

        //先处理移动后原来棋盘格中的棋子状态

        if(chess1[p][q]!=0)//如果原来棋盘格中是甲方棋子
        {
            setcursorpos(4*q+2,2*p+1);
            printf("●");
        }
        else if(chess2[p][q]!=0)//如果原来棋盘格中是乙方棋子
        {
            setcursorpos(4*q+2,2*p+1);
            printf("▇");
        }
        else if(chess1[p][q]==0&&chess2[p][q]==0)//如果原来棋盘格中没有棋子
        {
            setcursorpos(4*q+2,2*p+1);
            printf("  ");
        }
        switch(input)//改变数据棋盘的坐标
        {
            case 72 : p--; break;//上
            case 80 : p++; break;//下
            case 75 : q--; break;//左
            case 77 : q++; break;//右
        }
        //如果遇到了棋盘边界
        if(p==-1) p++;
        else if(p==N) p--;
        else if(q==-1) q++;
        else if(q==N) q--;
    }
    if(input==32)//输入空格（拾取棋子）
    {

        if(chess2[p][q]!=0)
        {
            int t,input2;
            t=chess2[p][q];
            chess2[p][q]=0;
            setcursorpos(4*q+2,2*p+1);
            printf("  ");
            fprintf(fp,"1 %d %d %d\n",p,q,chess2[p][q]);//将拾取棋子操作计入文件
            input2=getch() ;
            while(input2!=32)//输入方向键
            {
                setcursorpos(4*q+2,2*p+1);
                printf("▇");

                input2=getch();
                if(input2==0xE0)
                {
                    input2=getch();
                    if(chess1[p][q]!=0)
                    {
                        setcursorpos(4*q+2,2*p+1);
                    printf("●");
                    }
                    else if(chess2[p][q]!=0)
                    {
                        setcursorpos(4*q+2,2*p+1);
                        printf("▇");
                    }
                    else if(chess1[p][q]==0&&chess2[p][q]==0)
                    {
                        setcursorpos(4*q+2,2*p+1);
                        printf("  ");
                    }
                    switch(input2)
                    {
                        case 72 : p--; break;//上
                        case 80 : p++; break;//下
                        case 75 : q--; break;//左
                        case 77 : q++; break;//右
                    }
                    if(p==-1) p++;
                    else if(p==N) p--;
                    else if(q==-1) q++;
                    else if(q==N) q--;
                }
            }
            //再遇到空格，放下棋子，跳出循环
            chess2[p][q]=t;
            fprintf(fp,"2 %d %d %d\n",p,q,chess2[p][q]);//将原棋子放下操作计入文件
            setcursorpos(4*q+2,2*p+1);
            printf("▇");
            fight();
            refresh_rest2();
        }

    }
    else if(chess2[p][q]==0&&chess1[p][q]==0)//判断该位置是否可以下棋
    {
        int T;
        T=connectt(input);//关联动物的字母和其对应的数字
        if(judge(T))//这个字母有余量
        {
            rest2_2[T]--;
            chess2[p][q]=T;
            fprintf(fp,"3 %d %d %d\n",p,q,chess2[p][q]);//将新棋子计入文件
            setcursorpos(4*q+2,2*p+1);
            printf("▇");
            fight();
            refresh_rest2();
        }
    }
}

int connectt(int b)//关联动物的字母和其对应的数字
{
    int c;
    if(b==101) c=1;//大象
    if(b==116) c=2;//老虎
    if(b==108) c=3;//狮子
    if(b==119) c=4;//狼
    if(b==100) c=5;//狗
    if(b==99) c=6;//猫
    if(b==109) c=7;//鼠
    return c;
}
int judge(int a)//判断乙方是否还有该剩余棋子
{
    if(rest2_2[a]-1>=0) return 1;
    else return 0;
}
void fight()//甲乙双方棋子进行对战
{
    setcursorpos(0,24);
    printf("                                              ");
    setcursorpos(0,25);
    printf("                                              ");
    if(q-1>=0)//与左边棋子对战
    {
        if(chess1[p][q-1]!=0)
        {
            int g;
            g=chess1[p][q-1]-chess2[p][q];
            if(g<0&&g!=-6)
            {
                rest2[(chess2[p][q])]--;
                chess2[p][q]=0;
                setcursorpos(0,24);
                printf("您被左边敌方吃掉了                    ");
                setcursorpos(4*q+2, 2*p+1);
                printf("  ");
                fprintf(fp,"6 %d %d %d\n",p,q,chess2[p][q]);//将乙方战况计入文件
            }
            else if(g>0||g==-6)
            {
                rest1[(chess1[p][q-1])]--;
                chess1[p][q-1]=0;
                setcursorpos(4*(q-1)+2, 2*p+1);
                printf("  ");
                setcursorpos(0,24);
                printf("左边敌方被吃掉了                    ");
                setcursorpos(4*q+2, 2*p+1);
                fprintf(fp,"4 %d %d %d\n",p,q-1,chess1[p][q-1]);//将甲方左边战况计入文件

            }
            else if(g==0)//若两个棋子相同则同归于尽
            {
                rest2[(chess2[p][q])]--;
                chess2[p][q]=0;
                setcursorpos(4*q+2, 2*p+1);
                printf("  ");
                rest1[(chess1[p][q-1])]--;
                chess1[p][q-1]=0;
                setcursorpos(0,24);
                printf("和左边敌方同归于尽啦                ");
                setcursorpos(4*(q-1)+2, 2*p+1);
                printf("  ");
                setcursorpos(4*q+2, 2*p+1);
                fprintf(fp,"6 %d %d %d\n",p,q,chess2[p][q]);//将乙方战况计入文件
                fprintf(fp,"4 %d %d %d\n",p,q-1,chess1[p][q-1]);//将甲方左边战况计入文件
            }
        }
    }
    if(chess2[p][q]!=0)//如果我方棋子没有被左边的吃掉
    {
        if(q+1>=0)//与右边棋子对战
        {
            if(chess1[p][q+1]!=0)//如果左右两边棋子不相等
            {
                int g;
                g=chess1[p][q+1]-chess2[p][q];
                if(g<0&&g!=-6)
                {
                    rest2[(chess2[p][q])]--;
                    chess2[p][q]=0;
                    setcursorpos(0,25);
                    printf("您被右边敌方吃掉了                       ");
                    setcursorpos(4*q+2, 2*p+1);
                    printf("  ");
                    fprintf(fp,"6 %d %d %d\n",p,q,chess2[p][q]);//将乙方战况计入文件
                }
                else if(g>0||g==-6)
                {
                    rest1[(chess1[p][q+1])]--;
                    chess1[p][q+1]=0;
                    setcursorpos(4*(q+1)+2, 2*p+1);
                    printf("  ");
                    setcursorpos(0,25);
                    printf("右边敌方被吃掉了                    ");
                    setcursorpos(4*q+2, 2*p+1);
                    fprintf(fp,"5 %d %d %d\n",p,q+1,chess1[p][q+1]);//将甲方右边战况计入文件
                }
                else if(g==0)
                {
                    rest2[(chess2[p][q])]--;
                    chess2[p][q]=0;
                    setcursorpos(4*q+2, 2*p+1);
                    printf("  ");
                    rest1[(chess1[p][q+1])]--;
                    chess1[p][q+1]=0;
                    setcursorpos(0,25);
                    printf("您和右边敌方同归于尽啦                 ");
                    setcursorpos(4*(q+1)+2, 2*p+1);
                    printf("  ");
                    setcursorpos(4*q+2, 2*p+1);
                    fprintf(fp,"6 %d %d %d\n",p,q,chess2[p][q]);//将乙方战况计入文件
                    fprintf(fp,"5 %d %d %d\n",p,q+1,chess1[p][q+1]);//将甲方右边战况计入文件
                }
            }
        }
    }
}
void replay()//复盘函数
{
    int i,j,l,x,y,z;
    print_board();
    setcursorpos(0, 20);
    printf("按任意键进行下一步");
    for(i=0;i<14;i++)
    {
        fscanf(fp,"%d%d%d",&x,&y,&z);
        setcursorpos(4*y+2, 2*x+1);

        if(z==1) printf("e");//大象
            if(z==2) printf("t");//老虎
            if(z==3) printf("l");//狮子
            if(z==4) printf("w");//狼
            if(z==5) printf("d");//狗
            if(z==6) printf("c");//猫
            if(z==7) printf("m");//鼠
    }
    while(fscanf(fp,"%d%d%d%d",&l,&x,&y,&z)>0)
    {
        getch();
        if(l==1)
        {
            setcursorpos(4*y+2, 2*x+1);
            printf("  ");
        }
        if(l==2||l==3)
        {

            setcursorpos(4*y+2, 2*x+1);
            if(z==1) printf("e");//大象
            if(z==2) printf("t");//老虎
            if(z==3) printf("l");//狮子
            if(z==4) printf("w");//狼
            if(z==5) printf("d");//狗
            if(z==6) printf("c");//猫
            if(z==7) printf("m");//鼠
        }
        if(l==4||l==5)
        {
            if(z==0)
            {
                setcursorpos(4*y+2, 2*x+1);
                printf("  ");
            }
        }
        if(l==6)
        {
            if(z==0)
            {
                setcursorpos(4*y+2, 2*x+1);
                printf("  ");
            }
        }
    }
    setcursorpos(0, 20);
    printf("复盘结束               ");
}

