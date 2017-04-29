#ifndef CLOSEST_PAIR_MPI_TOOLS_H_
#define CLOSEST_PAIR_MPI_TOOLS_H_


#include "mpi/mpi.h"
#include "../../src/tools/closest_pair_tools.h"
//Types
typedef struct { point a; point b; double d; } distance;
extern MPI_Datatype PointType;
extern MPI_Datatype DistanceType;

//Global Variables
extern long np;
extern int mpi_size, mpi_id;
extern MPI_Group mpi_group;
extern MPI_Comm mpi_comm;

//Functions
char eqal(point a, point b);
void mpiInit(int argc, char** argv);
char prepareMPIComm(void);
void trivalSearch(long np, point* points, distance* solution);

double tack(void);


#endif // end of CLOSEST_PAIR_MPI_TOOLS_H_
