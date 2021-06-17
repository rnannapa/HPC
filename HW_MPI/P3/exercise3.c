#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])  {
//if NPROCS==4 uncomment the line below
//int rank, new_rank, sendbuf, recvbuf, numtasks, ranks1[2]={0,1}, ranks2[2]={2,3};

//if NPROCS==8 uncomment the line below
//int rank, new_rank, sendbuf, recvbuf, numtasks, ranks1[4]={0,1,2,3}, ranks2[4]={4,5,6,7};

//if NPROCS==16 uncomment the line below
//int rank, new_rank, sendbuf, recvbuf, numtasks, ranks1[8]={0,1,2,3,4,5,6,7}, ranks2[8]={8,9,10,11,12,13,14,15};

//if NPROCS==32 uncomment the line below
int rank, new_rank, sendbuf, recvbuf, numtasks, ranks1[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}, ranks2[16]={16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};


MPI_Group  orig_group, new_group;
MPI_Comm   new_comm;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

if (numtasks != NPROCS) {
  printf("Number of processes must be equal to NPROCS=%d. Terminating.\n",NPROCS);
  MPI_Finalize();
  exit(0);
  }

sendbuf = rank;

/* Extract the original group handle */
MPI_Comm_group(MPI_COMM_WORLD, &orig_group);

/* Divide tasks into two distinct groups based upon rank */
if (rank < NPROCS/2) {
  MPI_Group_incl(orig_group, NPROCS/2, ranks1, &new_group);
  }
else {
  MPI_Group_incl(orig_group, NPROCS/2, ranks2, &new_group);
  }

/* Create new new communicator and then perform collective communications */
MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);
MPI_Allreduce(&sendbuf, &recvbuf, 1, MPI_INT, MPI_SUM, new_comm);

MPI_Group_rank (new_group, &new_rank);
printf("Global rank= %d, Group rank= %d, recvbuf= %d\n",rank,new_rank,recvbuf);

MPI_Finalize();
}

