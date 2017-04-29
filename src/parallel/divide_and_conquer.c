/*
 ============================================================================
 Name        : data_set.c
 Author      : Aydin Homay
 Version     :
 Copyright   : @ Grid Computing Course FEUP 2017
 Description : To generate data set and w/r from/to file
 ============================================================================
 */

//#include <errno.h>
//#include <limits.h>
//#include <time.h>
#include <assert.h>

#include "../../src/tools/closest_pair_tools.h"
#include "../../src/tools/closest_pair_mpi_tools.h"

//#define DEBUG_HIGH
//#define DEBUG_MEDIUM

void mpiFinish(void);
void trivalSearch(long np, point* points, distance* solution);
void multiSearch(long nTotalPoints, point* points, distance* solution);
void searchBlocks(long np1, long np2, point* block1, point* block2, distance* bestsolution);

int randInit;

/*
int main(int argc, char** argv)
{

	point* points;
	distance solution;
	double elapsedTime;

	points = NULL;

	mpiInit(argc, argv);

	if( prepareMPIComm() )
	{


		if(mpi_id == 0)
		{
			printf("Starting search ...");
		}


			multiSearch(np, points, &solution);


		if(mpi_id == 0){
			printf("Completed Search and found closest points at [%g, %g] , [%g, %g] with a distance of [%g]\n", mpi_id\
							, solution.a.x ,solution.a.y, solution.b.x, solution.b.y
							, solution.d);
			printf("Operation took %f seconds \n", elapsedTime);
			free(points);
		}

	} else {

		}

	mpiFinish();
	exit(EXIT_SUCCESS);
}
*/

void mpiFinish(void){
	MPI_Finalize();
}

//Follow solution on
// http://en.wikipedia.org/wiki/Closest_pair_of_points_problem
//
// How it works
// * Find the x separating Value
// * Create two working subset of points that reside in the region around X +- minDistance
// * Order each subset by the y value its points
// * Start a search with the lowest y value in the left set over all points in the right that are in a y distance of +- minDistance
void searchBlocks(long np1, long np2, point* block1, point* block2, distance* bestsolution)
{
	float xSep;
	double minD;
	double newMinD;
	double d;
	long nL, nR;
	point *L, *R;
	point* pt;
	point *lp, *rp;
	long i, j;

	assert( (np1 > 0) && "Block has no elements!" );
	assert( (np2 > 0) && "Block has no elements!" );
	assert( (block1 != NULL ) && "Illegal Block reference" );
	assert( (block2 != NULL ) && "Illegal Block reference" );

	xSep = ( block1[np1-1].x + block2[0].x ) / 2.0;
	minD = bestsolution->d;

	//Get number of candidates on the left and on the right side of the separation
	nL = 0;
	pt = &block1[np1-1];
	while( (pt->x > (xSep-minD)) && (pt != block1) ){
		nL++;
		pt--;
	}

	nR = 0;
	pt = &block2[0];
	while( (pt->x < (xSep+minD)) && (pt != &block2[np2])){
		nR++;
		pt++;
	}

	printf("Found xSep [%g] , minD [%g], nL [%d], nR [%d]\n", xSep, minD, nL, nR);

	//Create subset of left and right points , and sort it
	L = (point*) malloc(sizeof(point) * nL);
	R = (point*) malloc(sizeof(point) * nR);

	if( (L == NULL) || (R == NULL) ){
		printf("Out of Memory!");
		MPI_Abort(mpi_comm, 23);
		exit(EXIT_FAILURE);
	}

	memcpy(L, &block1[np1 - nL], sizeof(point) * nL);
	memcpy(R, &block2[0], sizeof(point) * nR);

	qsort(L, nL, sizeof(point), cmp_x );
	qsort(R, nR, sizeof(point), cmp_y );


#ifdef DEBUG_MEDIUM
	char b[2000];
	sprintf(b,"--- Candidate points on node [%d] ---\n", mpi_id );
	sprintf(&b[strlen(b)],"Left Side \n" );
		for(i=0; i < nL; i++){
		 sprintf( &b[strlen(b)], "[%g, %g]\n", L[i].x, L[i].y );
		}
	sprintf(&b[strlen(b)],"Right Side \n" );
			for(i=0; i < nR; i++){
			 sprintf( &b[strlen(b)], "[%g, %g]\n", R[i].x, R[i].y );
			}
	sprintf(&b[strlen(b)], "--- END ---\n");
	printf(b); fflush(stdout);
#endif

	//Start Search: For each Point on the Left calculate its distance to candidates on the right

	newMinD = minD;

	for(i = 0; i < nL; i++)
	{
		//printf("Test L %d\n",i);
		lp = &L[i];
		rp = &R[0];
		j = 0;

		//All points lower than test region
		//Skip all points that are lower than the test region
		while( ((lp->y - minD) > rp->y) && (j < nR) ){
			rp++;
			j++;
		}

		//Test all points that are not higher than the test region
		while( ((lp->y + minD) > rp->y) && (j < nR)){
			//Check distance

			d = dist(&lp, &rp);
			d = sqrt(d);

			//printf("Cmp [%g,%g] with [%g,%g], d[%g]\n",lp->x, lp->y, rp->x, rp->y, d);

			if(d < newMinD){
				memcpy(&(bestsolution->a), lp, sizeof(point));
				memcpy(&(bestsolution->b), rp, sizeof(point));
				bestsolution->d = d;
			}
			rp++;
			j++;
		}
	}

	printf("Found closest points to be [%g, %g] [%g, %g] distance [%g]\n"\
			, bestsolution->a.x, bestsolution->a.y \
			, bestsolution->b.x, bestsolution->b.y \
			, bestsolution->d);
}


//Follow solution on
// http://en.wikipedia.org/wiki/Closest_pair_of_points_problem
void multiSearch(long nTotalPoints, point* points, distance* solution)
{

	int i;
	long np;
	int height, depth;
	int parent;
	point* localPoints;
	distance localSolution;
	distance remoteSolution;
	int zeroOffset;
	long nlocalPoints;
	long localPointOffset;



	np = nTotalPoints / mpi_size;
	zeroOffset = nTotalPoints % mpi_size;
	localPointOffset = mpi_id * np + zeroOffset;


	if(mpi_id == 0)
	{
		//Sort points by their x value
		qsort(points, nTotalPoints, sizeof(point), cmp_x);

		localPoints = points;
		MPI_Bcast( &localPoints[0], nTotalPoints, PointType, 0, mpi_comm);

		//MPI_Scatter(&points[zeroOffset], np, PointType, &localPoints[localPointOffset] , np, PointType, 0, mpi_comm);
		nlocalPoints = np + zeroOffset;
		localPointOffset = 0;
	}
	else
	{
		localPoints = malloc(sizeof(point) * nTotalPoints );
		if(localPoints == NULL){
			printf("Out of Memory! Cant locate buffer for local points!");
			exit(EXIT_FAILURE);
		}

		MPI_Bcast( &localPoints[0], nTotalPoints, PointType, 0, mpi_comm);
		//MPI_Scatter(NULL, np, PointType, &localPointOffset[localPointOffset] , np, PointType, 0, mpi_comm);
		nlocalPoints = np;
	}


#ifdef DEBUG_HIGH
	char b[2000];
	sprintf(b,"--- Received Points , Node [%d] ---\n", mpi_id );
		for(i=0; i < nTotalPoints; i++){
		 sprintf( &b[strlen(b)], "[%g, %g]\n", localPoints[i].x, localPoints[i].y );
		}
	sprintf(&b[strlen(b)], "--- END ---\n");
	printf(b); fflush(stdout);
#endif

	trivalSearch(nlocalPoints, &localPoints[localPointOffset], &localSolution);
	printf("[%d] Closest pair in points [%d - %d] has a distance of [%g] \n", mpi_id, localPointOffset, localPointOffset + nlocalPoints - 1, localSolution.d);

	//Start binary Tree data collection
	height = log(mpi_size)/log(2);
	depth = 0;

	/*
	if(mpi_id == 0){
		printf("Collect :[%d] [%d] <- [%d]\n",depth, mpi_id, 1 );
		MPI_Recv(&remoteSolution, 1, DistanceType, 1, 100, mpi_comm, MPI_STATUS_IGNORE );
		printf("[%d] New best Solution [%g]\n", mpi_id, localSolution.d); fflush(stdout);
	} else {
		printf("Send :[%d] [%d] -> [%d] , best Solution [%g]\n", depth, mpi_id, 0, localSolution.d); fflush(stdout);
		MPI_Send(&localSolution, 1, DistanceType, 0, 100, mpi_comm);
		printf("Finished Send\n"); fflush(stdout);
	}
	*/


	while(depth<height){
		parent = mpi_id - (mpi_id % (2<<depth)) ;
		if(mpi_id == parent){

			//Get Solution of remove block
			MPI_Recv(&remoteSolution, 1, DistanceType, mpi_id + (1<<depth), MPI_ANY_TAG, mpi_comm, MPI_STATUS_IGNORE );

			if(remoteSolution.d < localSolution.d){
				memcpy(&localSolution, &remoteSolution, sizeof(distance));
			}

			printf("[%d] Joining blocks [%d] <- [%d] , points [%d-%d] [%d-%d]\n", mpi_id, mpi_id, mpi_id + (1<<depth) \
											,localPointOffset, localPointOffset + nlocalPoints - 1\
											, zeroOffset + np*(mpi_id + (1<<depth)), zeroOffset + np*(mpi_id + (1<<depth)) + (1<<depth)*np - 1\
											 ) ; fflush(stdout);

			searchBlocks(nlocalPoints, (1<<depth)*np, &localPoints[localPointOffset], &localPoints[zeroOffset + np*(mpi_id + (1<<depth)) ], &localSolution);

			printf("[%d] Joining blocks [%d] <- [%d] , points [%d-%d] [%d-%d] , best Solution [%g]\n", mpi_id, mpi_id, mpi_id + (1<<depth) \
								,localPointOffset, localPointOffset + nlocalPoints - 1\
								, zeroOffset + np*(mpi_id + (1<<depth)), zeroOffset + np*(mpi_id + (1<<depth)) + (1<<depth)*np - 1\
								, localSolution.d ) ; fflush(stdout);

			//Adapt nlocalPoints
			nlocalPoints += (1<<depth)*np;
		} else {
			//printf("Send :[%d] [%d] -> [%d] , best Solution [%g]\n", depth, mpi_id, parent, localSolution.d); fflush(stdout);
			MPI_Send(&localSolution, 1, DistanceType, parent, depth, mpi_comm);
			depth = height; //Exit after this
		}
		depth++;
	}


//	if(mpi_id == 0){
//		printf("Completed Search and found closest points at [%g, %g] , [%g, %g] with a distance of [%g]\n", mpi_id\
//				, localSolution.a.x ,localSolution.a.y, localSolution.b.x, localSolution.b.y
//				, localSolution.d);
//	} else {
//		//printf("Node [%d] Best solution found is [%g] ...\n", mpi_id, localSolution.d);
//	}
	memcpy(solution, &localSolution, sizeof(distance));
}
