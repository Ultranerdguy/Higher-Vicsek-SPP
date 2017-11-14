#include "particle.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.141452653589

extern int dim;

double norm(double *arr) {
  double sum = 0;
  int i;
  for (i=0; i<dim; i++) {
    sum += pow(arr[i], 2);
  }
  return sqrt(sum);
}

void printDArray(double *array, int size) {
  printf("Printing array\n");
  int i;
  for (i=0; i<size; i++) {
    printf("%f, ", array[i]);
  }
  printf("\n");
}

void printPtcl(ptcl *main)
{
  int i;
  for (i=0; i<dim; i++)
  {
    printf("%f, ", main->loc[i]);
  }
  printf("\n");
}

void ptclInit(ptcl *main, double len) {
  main->loc = malloc(sizeof(double)*dim);
  main->curPhi = malloc(sizeof(double)*(dim-1));
  main->newPhi = malloc(sizeof(double)*(dim-1));

  int i;
  for (i=0; i<dim; i++) {
    main->loc[i] = ((double)rand()/RAND_MAX)*len;
  }
  for (i=0; i<dim-1; i++) {
    main->newPhi[i] = 1;
    if (i==(dim-2)) {
      main->curPhi[i] = ((double)rand()/RAND_MAX)*2*PI;
      continue;
    }
    main->curPhi[i] = ((double)rand()/RAND_MAX)*PI;
  }
}

void ptclDecon(ptcl *main) {
  free(main->newPhi);
  free(main->curPhi);
  free(main->loc);
}

void fixLoc(ptcl *main, double len)
{
  int i;
  for (i=0; i<dim; i++)
  {
    while (main->loc[i] < 0) { main->loc[i] += len;}
    while (main->loc[i] >= len) { main->loc[i] -= len;}
  }
}

int dist(ptcl *main, ptcl *other, double len, double rad) {
  double distance = 0;
  int i;
  double dx;
  for (i=0; i<dim; i++) {
    dx = fmin(fabs(main->loc[i]-other->loc[i]), len - fabs(main->loc[i]-other->loc[i]));
    if (dx > rad) {
      return 0;
    } else {
      distance += pow(dx, 2);
    }
  }
  if (sqrt(distance) < rad) {
    return 1;
  } else {
    return 0;
  }
}

double *resolveCart(double *angles) {
  double *coords = malloc(sizeof(double)*dim);
  int i;
  int j;
  for (i=0; i<dim; i++) {
    coords[i] = 1;
    for (j=0; j<dim-1; j++) {
      if (j>i) {
        continue;
      } else if (j==i) {
        coords[i] *= cos(angles[j]);
      } else {
        coords[i] *= sin(angles[j]);
      }
    }
  }
  return coords;
}

double *resolvePolar(double *pos) {
  double *angles = malloc(sizeof(double)*(dim-1));
  int i;
  double sqSum = pow(pos[dim-1], 2);
  for (i=dim-2; i>=0; i--) {
    if (i==(dim-2)) {
      angles[dim-2] = atan2(pos[dim-1], pos[dim-2]);
      sqSum += pow(pos[dim-2], 2);
    } else {
      sqSum += pow(pos[i], 2);
      angles[i] = acos(pos[i]/sqrt(sqSum));
    }
  }
  return angles;
}

void randAdj(ptcl *main, double randFac) {
  double *randArr = malloc(sizeof(double)*(dim-1));
  int i;
  int j;
  for (i=0; i<dim-1; i++) {
    if (i==0) {
      randArr[0] = ((double)rand()/RAND_MAX)*randFac-(randFac/2);
    } else if (i==(dim-2)) {
      randArr[dim-2] = ((double)rand()/RAND_MAX)*2*PI;
    } else {
      randArr[i] = ((double)rand()/RAND_MAX)*PI;
    }
  }
  if (cos(main->newPhi[0])==0) {
    main->newPhi[0] += 0.001;
  }
  double *curDir = resolveCart(main->newPhi);
  double *randDir = resolveCart(randArr);
  double *newDir = malloc(sizeof(double)*dim);

  for (i=0; i<dim; i++) {
    newDir[i] = 0;
    for (j=0; j<dim; j++) {
      if (j==0) {
        newDir[i] += curDir[i]*randDir[j];
      } else if (i==0) {
        newDir[i] -= curDir[j]*randDir[j];
      } else {
        newDir[i] += randDir[j]*(curDir[0]-1)*curDir[i]*curDir[j]/
            (1-pow(curDir[0], 2));
        if (i==j) {
          newDir[i] += randDir[j];
        }
      }
    }
  }
  free(randArr);
  randArr = NULL;
  free(main->newPhi);
  main->newPhi = resolvePolar(newDir);
  free(newDir);
  newDir = NULL;
  free(curDir);
  curDir = NULL;
  free(randDir);
  randDir = NULL;
}

double dot(double *arr1, double *arr2, int size) {
  int i;
  double sum = 0;
  for (i=0; i<size; i++) {
    sum += arr1[i]*arr2[i];
  }
  return sum;
}

void arrAdd(double *main, double *other, int size) {
  int i;
  for (i=0; i<size; i++) {
    main[i] += other[i];
  }
}

void setAll(double *main, double val, int size)
{
  int i;
  for (i=0; i<size; i++)
  {
    main[i] = val;
  }
}

double *arrMult(double *main, double val, int size)
{
  int i;
  double *newArr = malloc(sizeof(double)*size);
  for (i=0; i<size; i++)
  {
    newArr[i] = main[i] * val;
  }
  return newArr;
}
