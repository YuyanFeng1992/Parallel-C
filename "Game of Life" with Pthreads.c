#include "sys/time.h"
#include "stdio.h"
#include "string.h"
#include "pthread.h"
static int MAXROW;
static int MAXCOL;
static int LIFE_NUM,GENERATION_NUM;
static int THREAD_NUM;
static int DEAD=0;
static int ALIVE=1;
static int map[10000][10000];
static int newmap[10000][10000];
FILE * hw1;
//void init();
int neighbors(int, int);
//void outputMap();
void copyMap();
void* Thread_work(void* rank);
double gettime(void) ;
void refreshpixel(int row,int col);
void transdemtion(int a, int *row,int *col);

double gettime(void) {
    struct timeval tval;
    
    gettimeofday(&tval, NULL);

    return( (double)tval.tv_sec + (double)tval.tv_usec/1000000.0 );
}
void transdemtion(int a, int *row,int *col)
{
    *row = a/MAXCOL;
    *col = a%MAXCOL;
}
void* Thread_work(void* rank) {
    long my_rank = (long) rank;
    int i ,j;
    int x,y;
    int total = MAXROW*MAXCOL;
    int workload = total/THREAD_NUM;
    int start_pixel = my_rank*workload;
    
    
    for(i=0;i<GENERATION_NUM;i++)
    {
        for(j=0;j<workload;j++)
        {
            transdemtion(start_pixel+j,&x,&y);

            refreshpixel(x,y);
        }
        for(j=0;j<workload;j++)
        {
            transdemtion(start_pixel+j,&x,&y);

            copyPixel(x,y);
        }

    }
        
    return NULL;
}  /* Thread_sum */
int main(int argc,char **argv)
{
    int row, col;
    char ans;
    long count;
    long       thread;  /* Use long in case of a 64-bit system */
    pthread_t* thread_handles;
    double starttime, endtime;


    if(argc!=6)
    {
        printf("./test life_num generation_num  map_row map_col threadcount\n");
        return -1;	
    }	

    LIFE_NUM= atoi(argv[1]);	
    GENERATION_NUM= atoi(argv[2]);
    MAXROW= atoi(argv[3]);
    MAXCOL= atoi(argv[4]);
    THREAD_NUM = atoi(argv[5]);
    thread_handles = (pthread_t*) malloc (THREAD_NUM*sizeof(pthread_t)); 
    starttime=gettime();

    for (thread = 0; thread < THREAD_NUM; thread++)  
        pthread_create(&thread_handles[thread], NULL,
            Thread_work, (void*)thread);  

    for (thread = 0; thread < THREAD_NUM; thread++) 
        pthread_join(thread_handles[thread], NULL); 


    endtime=gettime(); 
    //outputMap();
    printf("Time taken = %lf seconds\n", endtime-starttime);
    hw1 = fopen("/mnt/homeapps/home/uabcls10/hw1/hw1.txt","a+");
    fprintf(hw1,"GENERATION_NUM = %d", GENERATION_NUM);
    fprintf(hw1,"     LIFE_NUM = %d", LIFE_NUM);
    fprintf(hw1,"     THREAD_NUM = %d", THREAD_NUM);
    fprintf(hw1,"     PROBLEM_SIZE = %d\n", MAXROW);
    fprintf(hw1,"Time taken = %lf seconds\n", endtime-starttime);
    fclose(hw1); 
    return 0;
}
void refreshpixel(int row,int col)
{
    switch (neighbors(row, col))
    {
        case 0:
        case 1:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        newmap[row][col] = DEAD;
        break;
        case 2:
        newmap[row][col] = map[row][col];
        break;
        case 3:
        newmap[row][col] = ALIVE;
        break;
    }
}
/*void init()
{
  int row, col;
  int i;
  for (row = 0; row < MAXROW; row++)
    for (col = 0; col < MAXCOL; col++)
      map[row][col] = DEAD;
  puts("Game of life");
  
  for(i=0;i<LIFE_NUM;i++)
  {
    row=rand()%MAXROW;
    col=rand()%MAXROW;
   
    if (0 <= row && row < MAXROW && 0 <= col && col < MAXCOL)
    {
      map[row][col] = ALIVE;
 	}
  }
}*/

int neighbors(int row, int col)
{
  int count = 0, c, r;
  for (r = row - 1; r <= row + 1; r++)
  for (c = col - 1; c <= col + 1; c++)
  {
    if (r < 0 || r >= MAXROW || c < 0 || c >= MAXCOL)
      continue;
    if (map[r][c] == ALIVE)
      count++;
  }
  if (map[row][col] == ALIVE)
    count--;
  return count;
}

/*void outputMap()
{
  int row, col,i;
 
  printf("%20c", ' ');
  for(i=0;i<=MAXCOL+1;i++)
  printf("#");
  for (row = 0; row < MAXROW; row++)
  {
    printf("\n%20c#", ' ');
    for (col = 0; col < MAXCOL; col++)
      if (map[row][col] == ALIVE)
        putchar('*');
      else
        putchar('-');
    printf("#");    
  }
  printf("\n%20c", ' ');
  for(i=0;i<=MAXCOL+1;i++)
  printf("#");
  printf("\n");

}*/

void copyPixel(int row,int col)
{
    map[row][col] = newmap[row][col];
}
void copyMap()
{
  int row, col;
  for (row = 0; row < MAXROW; row++)
    for (col = 0; col < MAXCOL; col++)
      map[row][col] = newmap[row][col];
}  
/*
	Name: Yuyan Feng
*/    
