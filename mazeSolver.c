#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 800
#define ROW 31                                  // enter row and column number of maze !
#define COLUMN 44

int totalScore = 0;
int flag = 1;                                   
char maze[ROW][COLUMN];
char visited[ROW][COLUMN];

typedef struct cell{
    int x;
    int y;
}CELL;

typedef struct stack{

    CELL *location;
    int top;
}STACK;

CELL startPoint,endPoint;

void initStack(STACK *s);
int isEmpty(STACK *s);
int isFull(STACK *s);
int pop(STACK *s , int x, int y);
void push(STACK *s, int x, int y);
void getMaze(char *fileName);
void initVisited(void);
void generateApple(int appleCount);
void printMaze(char m[ROW][COLUMN]);
void waitALittleBit(void);
void findPath(STACK *s, int x, int y);

int main(){
    
    STACK *s=(STACK*)malloc(sizeof(STACK));
    initStack(s);
    getMaze(" ");           // enter the path of maze file !
    printMaze(maze);
    printf("\n");
    initVisited();
    generateApple(10);
    findPath(s, startPoint.x, startPoint.y);
    
    return 0;
}

void initStack(STACK *s){
    s->top = 0;
    s->location = (CELL*)malloc(sizeof(CELL)*MAX);
}

int isEmpty(STACK *s){
    if(s->top == 0){
        return 1;
    }
    else
        return 0;
}

int isFull(STACK *s){
    if(s->top == MAX){
        return 1;
    }else {
        return 0;
    }
}

int pop(STACK *s , int x, int y){
    if(isEmpty(s)){
        return -1;
    }else{
        maze[x][y] = ' ';                     //remove '*' mark thus we can see current path on screen
        --s->top;
        return 1;
    }
}

void push(STACK *s, int x, int y){
    if(isFull(s)){
        printf("stack is full");
    }else{
        s->top ++;
        s->location[s->top].x=x;                
        s->location[s->top].y=y;                
        maze[x][y]='*';                         //visited coordinate is marked as '*'
        visited[x][y] = '*';
    }
    flag =1;
}

void getMaze(char *fileName){

    int i,j;
    FILE *myfp;
    myfp = fopen(fileName, "r");                        

    if (myfp == NULL) {
        printf("Dosya açılamadı!\n");
        exit(1);
    }else{
        while (!feof (myfp)){
            for(i=0;i<ROW;i++){
                fread(maze[i],1,COLUMN+1,myfp);
            }
        }
        fclose (myfp);
    }
    for(i=0;i<ROW;i++){
        for(j=0;j<COLUMN;j++){
            if(maze[i][j] == 'b'){                          
                startPoint.x = i;
                startPoint.y = j;
            }
        }
    }
}

void initVisited(){
    int i,j;
    
    for (i = 0; i < ROW; ++i) {
        for (j = 0; j < COLUMN; ++j) {
            if (maze[i][j] == '+' || maze[i][j] == '-' || maze[i][j] == '|') {          
                visited[i][j] = '0';
            } else if (maze[i][j] == 'b') {     
                startPoint.x = i;
                startPoint.y = j;
                visited[i][j] = 'b';
            }else if (maze[i][j] == 'c'){       
                endPoint.x = i;
                endPoint.y = j;
                visited[i][j] = ' ';
            }else {
                visited[i][j] = ' ';  
            }
        }
    }
}


void generateApple(int appleCount){             //it genarerates apples and places them randomly on the maze
    srand((unsigned)time(NULL));                
    int i,j,k=0;
    
    while(k < appleCount){
        i = rand() % ROW-2;
        j = rand() % COLUMN-2;
        if(visited[i][j] == ' '){
            maze[i][j] = 'o';
            k++;
        }
    }
}

void printMaze(char m[ROW][COLUMN]){
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COLUMN;j++){
            printf("%c", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Total Score : %d \n",totalScore);
}


void waitALittleBit(){                                      //to watch the path-finding process slowly
    int i,j;
    for(i=0;i<500000;i++){
        for(j=0;j<200;j++){
          //  k=0;
        }
    }
    system("clear");
}

void findPath(STACK *s, int x, int y){                      //deep first search
    
    push(s,x,y);                                            //the visited coordinate is pushed to the stack
    printMaze(maze);                                        
    waitALittleBit();
    
    if((x == endPoint.x) && (y == endPoint.y)){
        printf("\n ÇIKIŞA ULAŞILDI \n");
   //     printf("\n Total Score : %d \n",totalScore);
         exit(1) ;
    }
    
    if(visited[x+1][y] == ' '){                             //south
        if(maze[x+1][y] == 'o'){totalScore += 10;}          //gets 10 points if its collect an apple
        findPath(s,x+1,y);
    }
    if(visited[x][y-1] == ' '){                             //west
        if(maze[x][y-1] == 'o'){totalScore += 10;}
        findPath(s,x,y-1);
    }
    if(visited[x-1][y] == ' '){                             //north
        if(maze[x-1][y] == 'o'){totalScore += 10;}
        findPath(s,x-1,y);
    }
    if(visited[x][y+1] == ' '){                             //east
        if(maze[x][y+1] == 'o'){totalScore += 10;}
        findPath(s,x,y+1);
    }

    if(visited[x][y]== '*'){                                //if it encounter a dead-end, it pops visited cells from stack till find an unvisited cell
        if(flag == 1){totalScore -=5;flag =0;}              //lose 5 point for per dead-end
        pop(s,x,y);                                         
        printMaze(maze);
        return ;
    }
}
