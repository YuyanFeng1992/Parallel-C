#include "sys/time.h"
#include "stdio.h"
#include "string.h"
#include <mpi.h>
#include <stdlib.h>
static int MAXROW;
static int MAXCOL;
static int LIFE_NUM,GENERATION_NUM;
static int DEAD=0;
static int ALIVE=1;
// global variables
    char ans;
    int count;
    double starttime, endtime;
    int size, rank, s, rown;
    int row, col, dest;

    int *map = NULL;
    int *newmap = NULL;
    int *allmap = NULL;
    int *newallmap = NULL;
    MPI_Request requests[4];
    MPI_Status status[4];


FILE * hw1;
void outputMap(int *);
int init(int argc, char* argv[]);


int main(int argc,char *argv[])
{
    if(argc!=5)
    {
        printf("life_game life_num generation_num  map_row map_col\n");
        return -1;
    }

    if (init(argc, argv) == 0) {
        /* outputMap(allmap); */
        starttime=MPI_Wtime();
        for (count=0; count<GENERATION_NUM;count++)
        {
        #pragma omp parallel for
        for (row = 0; row < MAXROW; row++)
        {
            #pragma omp parallel for
            for (col = 0; col < MAXCOL; col++)
            {
                int nei = 0, c, r;
                for (r = row - 1; r <= row + 1; r++)
                    for (c = col - 1; c <= col + 1; c++)
                    {
                        if (r < 0 || r >= MAXROW || c < 0 || c >= MAXCOL)
                            continue;
                        if (allmap[r * MAXCOL + c] == ALIVE)
                            nei++;
                    }
                if (allmap[row * MAXCOL + col] == ALIVE)
                    nei--;
                switch (nei)
                {
                    case 0:
                    case 1:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                        newallmap[row * MAXCOL + col] = DEAD;
                        break;
                    case 2:
                        newallmap[row * MAXCOL + col] = allmap[row * MAXCOL + col];
                        break;
                    case 3:
                        newallmap[row * MAXCOL + col] = ALIVE;
                        break;
                }
            }
        }
        for (row = 0; row < MAXROW; row++)
            for (col = 0; col < MAXCOL; col++)
                allmap[row * MAXCOL + col] = newallmap[row * MAXCOL + col];
        }
        endtime=MPI_Wtime();
         printf("Time taken = %lf seconds\n", endtime-starttime);
        hw1 = fopen("/mnt/homeapps/home/uabcls10/hw1/hw1.txt","a+"); 
        fprintf(hw1,"GENERATION_NUM = %d", GENERATION_NUM);
        fprintf(hw1,"     LIFE_NUM = %d", LIFE_NUM);
        fprintf(hw1,"     PROBLEM_SIZE = %d\n", MAXROW);
        fprintf(hw1,"Time taken = %lf seconds\n", endtime-starttime);
        fclose(hw1); 
        /* outputMap(allmap); */
        MPI_Finalize();
        return 0;
    }

    /* starttime=gettime(); */
    starttime=MPI_Wtime();

    for (count=0; count<GENERATION_NUM;count++)
    {
        // data left
        //
        int reqn = 0;
        if (rank == 0) {
            MPI_Isend(&map[(rown)*MAXCOL], MAXCOL, MPI_INT, rank + 1, 1, MPI_COMM_WORLD, &requests[reqn++]);
            MPI_Irecv(&map[(rown+1)*MAXCOL], MAXCOL, MPI_INT, rank + 1, 1, MPI_COMM_WORLD, &requests[reqn++]);
        } else if (rank == size - 1) {
            MPI_Irecv(map, MAXCOL, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &requests[reqn++]);
            MPI_Isend(&map[MAXCOL], MAXCOL, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &requests[reqn++]);
        } else {
            MPI_Irecv(map, MAXCOL, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &requests[reqn++]);
            MPI_Isend(&map[(rown)*MAXCOL], MAXCOL, MPI_INT, rank + 1, 1, MPI_COMM_WORLD, &requests[reqn++]);
            MPI_Irecv(&map[(rown+1)*MAXCOL], MAXCOL, MPI_INT, rank + 1, 1, MPI_COMM_WORLD, &requests[reqn++]);
            MPI_Isend(&map[MAXCOL], MAXCOL, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &requests[reqn++]);
        }
        MPI_Waitall(reqn, requests, status);

        #pragma omp parallel for
        for (row = 1; row < rown+1; row+=1)
        {
            #pragma omp parallel for
            for (col = 0; col < MAXCOL; col++)
            {
                int nei = 0, c, r;
                for (r = row - 1; r <= row + 1; r++)
                    for (c = col - 1; c <= col + 1; c++)
                    {
                        if (c < 0 || c >= MAXCOL)
                            continue;
                        if (map[r * MAXCOL + c] == ALIVE)
                            nei++;
                    }
                if (map[row * MAXCOL + col] == ALIVE)
                    nei--;
                switch (nei)
                {
                    case 0:
                    case 1:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                        newmap[row * MAXCOL + col] = DEAD;
                        break;
                    case 2:
                        newmap[row * MAXCOL + col] = map[row * MAXCOL + col];
                        break;
                    case 3:
                        newmap[row * MAXCOL + col] = ALIVE;
                        break;
                }
            }
        }
        for (row = 1; row < rown+1; row+=1)
            for (col = 0; col < MAXCOL; col++)
                    map[row * MAXCOL + col] = newmap[row * MAXCOL + col];
    }
    if (rank == 0) {
        /* endtime=gettime(); */
        endtime=MPI_Wtime();
         printf("Time taken = %lf seconds\n", endtime-starttime);
        hw1 = fopen("/mnt/homeapps/home/uabcls10/hw1/hw1.txt","a+"); 
        fprintf(hw1,"GENERATION_NUM = %d", GENERATION_NUM);
        fprintf(hw1,"     LIFE_NUM = %d", LIFE_NUM);
        fprintf(hw1,"     PROBLEM_SIZE = %d\n", MAXROW);
        fprintf(hw1,"Time taken = %lf seconds\n", endtime-starttime);
        fclose(hw1); 
        memcpy(allmap, &map[MAXCOL], rown * MAXCOL * sizeof(int));
        for (dest = 1; dest < size; dest++) {
            MPI_Recv(&allmap[dest*rown*MAXCOL], rown*MAXCOL, MPI_INT, dest, 1, MPI_COMM_WORLD, status);
        }
        /* outputMap(allmap); */
    }
    else {
        MPI_Send(&map[MAXCOL], rown * MAXCOL, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}

int init(int argc, char* argv[])
{
    LIFE_NUM= atoi(argv[1]);
    GENERATION_NUM= atoi(argv[2]);
    MAXROW= atoi(argv[3]);
    MAXCOL= atoi(argv[4]);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    rown = MAXROW / size;

    map = (int*)malloc(sizeof(int) * (rown+2) * MAXCOL);
    newmap = (int*)malloc(sizeof(int) * (rown+2) * MAXCOL);
    for (row = 0; row < rown+2; row++) {
        for (col = 0; col < MAXCOL; col++) {
            map[row*MAXCOL+col] = DEAD;
            newmap[row*MAXCOL+col] = DEAD;
        }
    }
    if (rank == 0) {
        int i;
        allmap = (int*)malloc(sizeof(int) * MAXROW * MAXCOL);
        newallmap = (int*)malloc(sizeof(int) * MAXROW * MAXCOL);
        for (row = 0; row < MAXROW; row++) {
            for (col = 0; col < MAXCOL; col++) {
                allmap[row*MAXCOL+col] = DEAD;
            }
        }
        puts("Game of life");

        for(i=0;i<LIFE_NUM;i++)
        {
            row=rand()%MAXROW;
            col=rand()%MAXROW;

            if (0 <= row && row < MAXROW && 0 <= col && col < MAXCOL)
            {
                allmap[row*MAXCOL + col] = ALIVE;
            }
        }
        if (size == 1)
            return 0;
        memcpy(&map[MAXCOL], &allmap[0], rown * MAXCOL * sizeof(int));
        for (dest = 1; dest < size; dest++) {
            MPI_Send(&allmap[dest*rown*MAXCOL], rown * MAXCOL, MPI_INT, dest, 1, MPI_COMM_WORLD);
        }
        /* outputMap(allmap); */
    } else {
        MPI_Recv(&map[MAXCOL], rown * MAXCOL, MPI_INT, 0, 1, MPI_COMM_WORLD, status);
    }
    return 1;
}

void outputMap(int *map)
{
    int row, col,i;

    printf("%20c", ' ');
    for(i=0;i<=MAXCOL+1;i++)
        printf("#");
    for (row = 0; row < MAXROW; row++)
    {
        printf("\n%20c#", ' ');
        for (col = 0; col < MAXCOL; col++)
            if (map[row * MAXCOL + col] == ALIVE)
                putchar('*');
            else
                putchar('-');
        printf("#");
    }
    printf("\n%20c", ' ');
    for(i=0;i<=MAXCOL+1;i++)
        printf("#");
    printf("\n");

}

/*
    Name: Yuyan Feng
*/    

