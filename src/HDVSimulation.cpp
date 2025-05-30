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
void HDV::Simulation::SetSearchRadius(HDV::Float searchRadius) noexcept
{
  m_searchRadius = searchRadius;
}
HDV::Float HDV::Simulation::GetSearchRadius() const noexcept
{
  return m_searchRadius;
}
void HDV::Simulation::SetSpeed(HDV::Float speed) noexcept
{
  m_speed = speed;
}
HDV::Float HDV::Simulation::GetSpeed() const noexcept
{
  return m_speed;
}
void HDV::Simulation::SetRandomRotationStart(HDV::Float randomRotationStart) noexcept
{
  m_randomRotationStart = randomRotationStart;
}
HDV::Float HDV::Simulation::GetRandomRotationStart() const noexcept
{
  return m_randomRotationStart;
}
void HDV::Simulation::SetRandomRotationEnd(HDV::Float randomRotationEnd) noexcept
{
  m_randomRotationEnd = randomRotationEnd;
}
HDV::Float HDV::Simulation::GetRandomRotationEnd() const noexcept
{
  return m_randomRotationEnd;
}
void HDV::Simulation::SetRandomRotationStep(HDV::Float randomRotationStep) noexcept
{
  m_randomRotationStep = randomRotationStep;
}
HDV::Float HDV::Simulation::GetRandomRotationStep() const noexcept
{
  return m_randomRotationStep;
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
  #define randStart m_randomRotationStart
  #define avrCount m_recordPeriod
  #define randReduct 0.01
  #define ticks (randStart*avrCount*(1/randReduct))+600
  #define count m_particleCount
  #define PI 3.141592653589793

  // Iterator variables
  unsigned int i;
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

int HDV::Simulation::RunSim()
{
  // Create and setup particles
  m_particles.clear();
  m_particles.reserve(m_particleCount);
  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles.emplace_back(HDV::Particle(m_dimensionCount, m_generator));
  }

  auto randomRotation = m_randomRotationStart;
  auto randomStep = m_randomRotationStep; 
  // Adjust step to go from end to start
  if ((m_randomRotationEnd-randomRotation) * randomStep < 0)
  {
    randomStep = -m_randomRotationStep;
  }
  // Add an extra step  to randomEnd to include it (also helps when m_randomRotationStart == m_randomRotationEnd)
  auto randomEnd = m_randomRotationEnd + randomStep;

  auto const radius2 = (m_searchRadius*m_searchRadius);
  // Mainloop. Expression takes care of random rotation increasing or decreasing towards randomEnd
  unsigned int counter = 0;
  while ((randomEnd-randomRotation) * randomStep > 0)
  {
    for (unsigned int i = 0; i < m_particleCount; ++i)
    {
      m_particles[i].ResetNextDirection();
      for (unsigned int j = i+1; j < m_particleCount; ++j)
      {
        if (Distance2(m_particles[i], m_particles[j]) < radius2)
        {
          m_particles[i].AddNextDirection(m_particles[j].GetCurrentDirection());
        }
      }
    }

    for (auto& particle : m_particles)
    {
      particle.SwapDirections();
      particle.Wobble(randomRotation);
      particle.Step(m_speed);
      particle.Wrap();
    }
    
    ++counter;

    if ((counter % m_randomPeriod) == 0)
    {
      randomRotation += randomStep;
    }

    if ((counter % m_recordPeriod) == 0)
    {
      // Log
    }
  }
  
  return 0;
}

