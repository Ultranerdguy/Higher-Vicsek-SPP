#include "HDVSimulation.hpp"
#include <cstdlib>
#include <cstddef>
#include "particle.h"

// Dimension of the simulation
int dim = 2;

void HDV::Simulation::SetOutputDirectory(std::filesystem::path outdir) noexcept
{
  m_outputDirectory = std::move(outdir);
}
std::filesystem::path const& HDV::Simulation::GetOutputDirectory() const noexcept
{
  return m_outputDirectory;
}
void HDV::Simulation::SetOutputFilename(std::string name) noexcept
{
  m_outputFilename = std::move(name);
}
std::string const& HDV::Simulation::GetOutputFilename() const noexcept
{
  return m_outputFilename;
}
void HDV::Simulation::SetDimensions(unsigned int dimensions) noexcept
{
  m_dimensionCount = dimensions;
}
unsigned int HDV::Simulation::GetDimensions() const noexcept
{
  return m_dimensionCount;
}
void HDV::Simulation::SetParticleCount(unsigned int particleCount) noexcept
{
  m_particleCount = particleCount;
}
unsigned int HDV::Simulation::GetParticleCount() const noexcept
{
  return m_particleCount;
}
void HDV::Simulation::SetSearchRadius(double searchRadius) noexcept
{
  m_searchRadius = searchRadius;
}
double HDV::Simulation::GetSearchRadius() const noexcept
{
  return m_searchRadius;
}
void HDV::Simulation::SetSpeed(double speed) noexcept
{
  m_speed = speed;
}
double HDV::Simulation::GetSpeed() const noexcept
{
  return m_speed;
}
void HDV::Simulation::SetRandomRotation(double randomRotation) noexcept
{
  m_randomRotation = randomRotation;
}
double HDV::Simulation::GetRandomRotation() const noexcept
{
  return m_randomRotation;
}
void HDV::Simulation::SetRandomPeriod(unsigned int randomPeriod) noexcept
{
  m_randomPeriod = randomPeriod;
}
unsigned int HDV::Simulation::GetRandomPeriod() const noexcept
{
  return m_randomPeriod;
}
void HDV::Simulation::SetRecordPeriod(unsigned int recordPeriod) noexcept
{
  m_recordPeriod = recordPeriod;
}
unsigned int HDV::Simulation::GetRecordPeriod() const noexcept
{
  return m_recordPeriod;
}

int HDV::Simulation::Run()
{
  dim = m_dimensionCount;
  /*
  Variable definitions
  len: Size of the system
  rad: Search radius around a particle
  vel: Velocity of a particle
  tStep: Time between steps in the simulation
  ticks: Total number of steps in the simulation
  count: Number of particles in the simulation
  PI: The number
  */
  #define len 1
  #define rad m_searchRadius
  #define vel m_speed
  #define tStep 1.0
  #define randStart m_randomRotation
  #define avrCount m_recordPeriod
  #define randReduct 0.01
  #define ticks (randStart*avrCount*(1/randReduct))+600
  #define count m_particleCount
  #define PI 3.141592653589793

  // Iterator variables
  int i;
  unsigned int j;
  int step;
  // For counting things
  int counter;
  // Switch pointer
  void *swPtr;
  // Main loop for the system
  // Increments dimensionality indefinitely
  // Inital RNG seed
  srand(static_cast<unsigned int>(time(nullptr)));

  double randFac = 0.0;

  // Create a file to store the data for this dimension
  char fileName[13];
  sprintf(fileName, "anvData%d.csv", m_dimensionCount);
  FILE *fanv = fopen(fileName, "w");

  // Make an array of ptcl types for the simulation
  ptcl *swarm = (ptcl*)malloc(sizeof(ptcl)*count);

  // Initiate values for this sim
  for (i=0; i<count; i++)
  {
    ptclInit(&swarm[i], len);
  }

  /*
  avr: used for finding the average 'average normalised velocity'
  velSum: Space to sum the velocity vectors of the Particles
  xSum: Space to add up the velocity vectors of particles surrounding the
      current particle of interest
  spare1/2: Used as spare memory slots for swapping data
  */
  double avr = 0;
  double *velSum = (double*)malloc(sizeof(double)*m_dimensionCount);
  double *xSum = (double*)malloc(sizeof(double)*m_dimensionCount);
  double *spare1;
  double *spare2;

  // Start the random rotation factor high, to be reduced over the course of
  // the simulation
  randFac = randStart;

  // Carry out sim
  for (step=0; randFac>=0; step++)
  {
    // Set the sum of velocity vectors to 0
    setAll(velSum, 0, m_dimensionCount);

    // Finding the new direction vector for each particle
    for (i=0; i<count; i++)
    {
      // Add the particles' current velocity vector to the velSum vector
      spare1 = resolveCart(swarm[i].curPhi);
      spare2 = arrMult(spare1, vel, m_dimensionCount);
      arrAdd(velSum, spare2, m_dimensionCount);
      free(spare1);
      free(spare2);

      // Prepare to sum velocity vectors then find the average
      counter = 0;
      setAll(xSum, 0, m_dimensionCount);

      // The loop that checks for all particles within range
      for (j=0; j<count; j++)
      {
        if ((i==j) || dist(&swarm[i], &swarm[j], len, rad)==1)
        {
          counter++;
          spare1 = resolveCart(swarm[j].curPhi);
          arrAdd(xSum, spare1, m_dimensionCount);
          free(spare1);
        }
      }

      // Normalises the sum of these vectors
      for (j=0; j<m_dimensionCount; j++) {
        xSum[j] /= counter;
      }

      // Convert cart to polar
      free(swarm[i].newPhi);
      swarm[i].newPhi = resolvePolar(xSum);

      // Randomly adjust the direction vector a little according to randFac
      randAdj(&swarm[i], randFac);

    }
    // Set up the particles for the next sim tick
    for (i=0; i<count; i++)
    {
      swPtr = swarm[i].curPhi;
      swarm[i].curPhi = swarm[i].newPhi;
      swarm[i].newPhi = (double*)swPtr;

      spare1 = resolveCart(swarm[i].curPhi);
      spare2 = arrMult(spare1, tStep*vel, m_dimensionCount);
      arrAdd(swarm[i].loc, spare2, m_dimensionCount);
      free(spare1);
      free(spare2);

      fixLoc(&swarm[i], len);
    }

    // Output the data once the particles have had time to 'settle'
    if (step > 500) {
      if (step%avrCount>=100) {
        avr += norm(velSum)/(vel*count);
      }
      // Output data. The format is
      // Current Random Factor, average anv
      // This makes it much easier to plot the data, as both x and y values
      // are provided
      if (step%avrCount==0) {
        avr /= avrCount-100;
        fprintf(fanv, "%.2f, %f\n", randFac, avr);
        printf("%dD:, %d, %.2f: %f\n", m_dimensionCount, step, randFac, avr);
        avr = 0;
        randFac -= randReduct;
      }
    }
/*
    fprintf(fanv, "%f\n", norm(velSum)/(vel*count));
    printf("%dD: %d, %d: %f\n", dim, itr+1, step, norm(velSum)/(vel*count));
*/
  }

  // Once done simulating in this dimension, clear ptcl memory and other
  // dimension dependent variables memory
  for (i=0; i<count; i++)
  {
    ptclDecon(&swarm[i]);
  }
  free(velSum);
  free(xSum);
  fclose(fanv);
  

  return 0;
}