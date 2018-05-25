# Parallel-C
#include "sys/time.h"
#include "stdio.h"
#include "string.h"
static int MAXROW;
static int MAXCOL;
static int LIFE_NUM,GENERATION_NUM;
static int DEAD=0;
static int ALIVE=1;
static int map[10000][10000];
static int newmap[10000][10000];
FILE * hw1;
void init();
int neighbors(int, int);
//void outputMap();
void copyMap();

double gettime(void) {
  struct timeval tval;

  gettimeofday(&tval, NULL);

  return( (double)tval.tv_sec + (double)tval.tv_usec/1000000.0 );
}
int main(int argc,char **argv)
{
  int row, col;
  char ans;
  int count;
  double starttime, endtime;
  
  char calife_num[10000];
  char cageneration_num[10000];
  char caMAXROW[10000];
  char caMAXCOL[10000];

  memset(calife_num,0,sizeof(calife_num));
  memset(cageneration_num,0,sizeof(cageneration_num));
  memset(caMAXROW,0,sizeof(caMAXROW));
  memset(caMAXCOL,0,sizeof(caMAXCOL));
  
  if(argc!=5)
  {
  	printf("life_game life_num generation_num  map_row map_col\n");
  	return -1;	
  }	
  
  strcpy(calife_num,argv[1]);
  strcpy(cageneration_num,argv[2]);
  strcpy(caMAXROW,argv[3]);
  strcpy(caMAXCOL,argv[4]);	
  
  LIFE_NUM= atoi(calife_num);	
  GENERATION_NUM= atoi(cageneration_num);
  MAXROW= atoi(caMAXROW);
  MAXCOL= atoi(caMAXCOL);
  
  init();
  starttime=gettime();
  for (count=0; count<GENERATION_NUM;count++)
  {
    //outputMap();
    for (row = 0; row < MAXROW; row++)
    {
      for (col = 0; col < MAXCOL; col++)
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
    }
    copyMap();
}
	endtime=gettime(); 
        printf("Time taken = %lf seconds\n", endtime-starttime);
        hw1 = fopen("/mnt/homeapps/home/uabcls10/hw1/hw1.txt","a+");
	fprintf(hw1,"GENERATION_NUM = %d", GENERATION_NUM);
	fprintf(hw1,"     LIFE_NUM = %d", LIFE_NUM);
	fprintf(hw1,"     PROBLEM_SIZE = %d\n", MAXROW);
        fprintf(hw1,"Time taken = %lf seconds\n", endtime-starttime);
        fclose(hw1); 
  return 0;
}

void init()
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
}

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

void copyMap()
{
  int row, col;
  for (row = 0; row < MAXROW; row++)
    for (col = 0; col < MAXCOL; col++)
      map[row][col] = newmap[row][col];
}  
/*
	Name: Yuyan Feng
	BlazerID: fengyuya
	Course section: CS 632
*/    
