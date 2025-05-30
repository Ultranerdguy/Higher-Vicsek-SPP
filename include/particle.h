#ifndef _HDV_PARTICLE_H_
#define _HDV_PARTICLE_H_

typedef struct ptcl {
	double *loc;
	double *curPhi;
	double *newPhi;
} ptcl;

// Debug function: Prints the contents of an 'array' of length 'size'
void printDArray(double *array, int size);
// Debug function: Print out the data of particle 'main'
void printPtcl(ptcl *main);

// Finds the normal of a given array
// Uses the external variable dim as the array size
double norm(double *arr);

// Initialises a ptcl.
// Uses 'len' as the size of the system the particle is in
// Sets the particles position in the system, and initial facing direction
void ptclInit(ptcl *main, double len);

// Resets a ptcl by freeing memory
void ptclDecon(ptcl *main);

// Forces the ptcl main to the periodic box of size len
void fixLoc(ptcl *main, double len);

// Finds the distance between two ptcl types, and determines if the second
// ptcl is in range of the first one
// Returns 1 if the two particles are closer than 'rad' and 0 otherwise
int dist(ptcl *main, ptcl *other, double len, double rad);

// Converts the angles in array 'angles' to euclidian coordinates
double *resolveCart(double *angles);

// Converts euclidean coordinates in array 'pos' into the angles of polar form
double *resolvePolar(double *pos);

// Given a particle, randomly adjusts the direction within the range
// of the randFac
void randAdj(ptcl *main, double randFac);

// Dot products two vectors together
double dot(double *arr1, double *arr2, int size);

// Adds array 'other' to array 'main', element by element
// Size is the size of the arrays
void arrAdd(double *main, double *other, int size);

// Sets all elements in the given array to the given variables
void setAll(double *main, double val, int size);

// Multiplies every element of main by val, and returns a new array
double *arrMult(double *main, double val, int size);

#endif // #ifndef PARTICLE_HEADER

