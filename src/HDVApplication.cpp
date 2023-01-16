#include "HDVApplication.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <string>
#include <sstream>
#include "hdv_config.h"
#include "particle.h"

// Dimension of the simulation
int dim = 2;

HDV::Application::Application()
{
  CreateArguments();
  RegisterArguments();
}

void HDV::Application::ParseArguments(int argc, char** argv)
{
  m_parser.ParseArguments(argv, argv+argc);
}

int HDV::Application::RunMain()
{
  return m_main();
}

std::filesystem::path const& HDV::Application::GetOutputDirectory() const noexcept
{
  return m_outputDirectory;
}
std::string_view HDV::Application::GetOutputFilename() const noexcept
{
  return m_outputFilename;
}
unsigned int HDV::Application::GetDimensions() const noexcept
{
  return m_dimensionCount;
}
unsigned int HDV::Application::GetParticleCount() const noexcept
{
  return m_particleCount;
}
double HDV::Application::GetSearchRadius() const noexcept
{
  return m_searchRadius;
}
double HDV::Application::GetSpeed() const noexcept
{
  return m_speed;
}
double HDV::Application::GetRandomRotation() const noexcept
{
  return m_randomRotation;
}
unsigned int HDV::Application::GetRandomPeriod() const noexcept
{
  return m_randomPeriod;
}
unsigned int HDV::Application::GetRecordPeriod() const noexcept
{
  return m_recordPeriod;
}

void HDV::Application::CreateArguments()
{
  m_arguments.reserve(12);
  m_arguments.emplace_back(CmdParser::Argument{
    "help"
    ,"Display help."
    ,[this](auto ... args){return ParseArg_help(args...);}
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "version"
    ,"Display the program version."
    ,[this](auto ... args){return ParseArg_version(args...);}
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "config"
    ,"Specify a config file to load settings from.\n\tConfig file takes the same format as command line arguments, with one argument per line."
    ,[this](auto ... args){return ParseArg_config(args...);}
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "output-directory"
    ,"Specify the output directory for simulation results. Defaults to current working directory."
    ,[this](auto ... args){return ParseArg_output_directory(args...);}
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "output-name"
    ,"Spcify the name for the output file. Defaults to \"HDV_<dimensions>.csv\"."
    ,[this](auto ... args){return ParseArg_output_name(args...);}
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "dimensions"
    ,"Specify the number of dimensions to simulate in. Defaults to 2."
    ,[this](auto ... args){return ParseArg_dimensions(args...);}
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "particle-count"
    ,"Specify the number of particles to simulate. Defaults to 100."
    ,[this](auto ... args){return ParseArg_particle_count(args...);}
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "search-radius"
    ,"Specify the search radius of each particle. Defaults to 0.1."
    ,[this](auto ... args){return ParseArg_search_radius(args...);}
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "speed"
    ,"Specify the speed of each particle (distance per tick). Defaults to 0.01."
    ,[this](auto ... args){return ParseArg_speed(args...);}
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "random-rotation"
    ,"Specify the maximum amount of random rotation to apply to each particle. Defaults to 0.05."
    ,[this](auto ... args){return ParseArg_random_rotation(args...);}
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "random-period"
    ,"Specify the number of ticks between applications of random rotation. Defaults to 100."
    ,[this](auto ... args){return ParseArg_random_period(args...);}
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "record-period"
    ,"Specify the number of steps between each record of values. Defaults to 100."
    ,[this](auto ... args){return ParseArg_record_period(args...);}
  });
}

void HDV::Application::RegisterArguments()
{
  for (auto& arg : m_arguments)
  {
    m_parser.RegisterArgument(arg);
  }
}

// Parser functions
char** HDV::Application::ParseArg_help(char** begin, char** end)
{
  m_main = [this](){return RunHelp();};
  return begin+1;
}
char** HDV::Application::ParseArg_version(char** begin, char** end)
{
  m_main = [this](){return RunVersion();};
  return begin+1;
}
char** HDV::Application::ParseArg_config(char** begin, char** end)
{
  char* arg = *begin;
  ++begin; // Skip the current setting
  // Check for parameters
  if (begin == end
  || !(*begin))
  {
    throw std::runtime_error(std::string("Missing parameter: ") + arg);
  }
  std::vector<char*> arguments;
  std::vector<char> allLines;
  {
    // Only open the file for as long as we need it
    // Check for valid file
    std::ifstream inConfig(*begin);
    if (!inConfig)
    {
      throw std::runtime_error(std::string("Invalid file name: ") + *begin);
    }

    // For each line in the config, add to allLines and track the start
    std::string word;
    while (inConfig >> std::quoted(word))
    {
      allLines.insert(allLines.end(), word.begin(), word.end());
      allLines.push_back('\0');
    }
    if (!allLines.empty())
    {
      arguments.push_back(allLines.data());
      for (auto& c : allLines)
      {
        if (c == '\0') arguments.push_back((&c) + 1);
      }
      arguments.pop_back(); // remove trailing '\0'
    }
  }

  ParseArguments(arguments.size(), arguments.data());
  return begin+1;
}
char** HDV::Application::ParseArg_output_directory(char** begin, char** end)
{
  char* arg = *begin;
  ++begin; // Skip the current setting
  // Check for parameters
  if (begin == end
  || !(*begin))
  {
    throw std::runtime_error(std::string("Missing parameter: ") + arg);
  }
  std::filesystem::path outDir(*begin);
  outDir.remove_filename();
  m_outputDirectory = outDir;
  return begin+1;
}
char** HDV::Application::ParseArg_output_name(char** begin, char** end)
{
  char* arg = *begin;
  ++begin; // Skip the current setting
  // Check for parameters
  if (begin == end
  || !(*begin))
  {
    throw std::runtime_error(std::string("Missing parameter: ") + arg);
  }
  m_outputFilename = *begin;
  return begin+1;
}
char** HDV::Application::ParseArg_dimensions(char** begin, char** end)
{
  char* arg = *begin;
  ++begin; // Skip the current setting
  // Check for parameters
  if (begin == end
  || !(*begin))
  {
    throw std::runtime_error(std::string("Missing parameter: ") + arg);
  }
  std::stringstream dimStrm(*begin);
  if (!(dimStrm >> m_dimensionCount))
  {
    throw std::runtime_error(std::string("Unable to read dimensions: ") + *begin);
  }
  return begin+1;
}
char** HDV::Application::ParseArg_particle_count(char** begin, char** end)
{
  char* arg = *begin;
  ++begin; // Skip the current setting
  // Check for parameters
  if (begin == end
  || !(*begin))
  {
    throw std::runtime_error(std::string("Missing parameter: ") + arg);
  }
  std::stringstream particleCountStrm(*begin);
  if (!(particleCountStrm >> m_particleCount))
  {
    throw std::runtime_error(std::string("Unable to read particle count: ") + *begin);
  }
  return begin+1;
}
char** HDV::Application::ParseArg_search_radius(char** begin, char** end)
{
  char* arg = *begin;
  ++begin; // Skip the current setting
  // Check for parameters
  if (begin == end
  || !(*begin))
  {
    throw std::runtime_error(std::string("Missing parameter: ") + arg);
  }
  std::stringstream searchRadiusStrm(*begin);
  if (!(searchRadiusStrm >> m_searchRadius))
  {
    throw std::runtime_error(std::string("Unable to read search radius: ") + *begin);
  }
  return begin+1;
}
char** HDV::Application::ParseArg_speed(char** begin, char** end)
{
  char* arg = *begin;
  ++begin; // Skip the current setting
  // Check for parameters
  if (begin == end
  || !(*begin))
  {
    throw std::runtime_error(std::string("Missing parameter: ") + arg);
  }
  std::stringstream speedStrm(*begin);
  if (!(speedStrm >> m_speed))
  {
    throw std::runtime_error(std::string("Unable to read speed: ") + *begin);
  }
  return begin+1;
}
char** HDV::Application::ParseArg_random_rotation(char** begin, char** end)
{
  char* arg = *begin;
  ++begin; // Skip the current setting
  // Check for parameters
  if (begin == end
  || !(*begin))
  {
    throw std::runtime_error(std::string("Missing parameter: ") + arg);
  }
  std::stringstream randomRotationStrm(*begin);
  if (!(randomRotationStrm >> m_randomRotation))
  {
    throw std::runtime_error(std::string("Unable to read random rotation: ") + *begin);
  }
  return begin+1;
}
char** HDV::Application::ParseArg_random_period(char** begin, char** end)
{
  char* arg = *begin;
  ++begin; // Skip the current setting
  // Check for parameters
  if (begin == end
  || !(*begin))
  {
    throw std::runtime_error(std::string("Missing parameter: ") + arg);
  }
  std::stringstream randomPeriodStrm(*begin);
  if (!(randomPeriodStrm >> m_randomPeriod))
  {
    throw std::runtime_error(std::string("Unable to read random period: ") + *begin);
  }
  return begin+1;
}
char** HDV::Application::ParseArg_record_period(char** begin, char** end)
{
  char* arg = *begin;
  ++begin; // Skip the current setting
  // Check for parameters
  if (begin == end
  || !(*begin))
  {
    throw std::runtime_error(std::string("Missing parameter: ") + arg);
  }
  std::stringstream recordPeriodStrm(*begin);
  if (!(recordPeriodStrm >> m_recordPeriod))
  {
    throw std::runtime_error(std::string("Unable to read record period: ") + *begin);
  }
  return begin+1;
}

// Run types
int HDV::Application::RunHelp()
{
  std::size_t maxNameSize = 0;
  std::size_t maxDescriptionSize = 0;
  for (auto& arg : m_arguments)
  {
    maxNameSize = std::max(maxNameSize, arg.name.size());
    maxDescriptionSize = std::max(maxDescriptionSize, arg.description.size());
  }
  auto const& delimiter = m_parser.GetDelimiter();
  maxNameSize += delimiter.size() + 1;

  std::cout << std::left << std::setw(maxNameSize) << "Name" << std::setw(maxDescriptionSize) << "Description" << '\n';
  for (auto& arg : m_arguments)
  {
    std::cout << std::left << std::setw(maxNameSize) << (delimiter + arg.name) << std::setw(maxDescriptionSize) << arg.description << '\n';
  }
  return 0;
}

int HDV::Application::RunVersion()
{
  std::cout << "HDV_" << HDV::VERSION::MAJOR << '.' << HDV::VERSION::MINOR << '.' << HDV::VERSION::PATCH << '\n';
  return 0;
}

int HDV::Application::RunApp()
{
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
  #define len 10
  #define rad 1
  #define vel 0.03
  #define tStep 1.0
  #define randStart 10
  #define avrCount 500
  #define randReduct 0.01
  #define ticks (randStart*avrCount*(1/randReduct))+600
  #define count 40
  #define PI 3.141592653589793

  // Iterator variables
  int i;
  int j;
  int step;
  // For counting things
  int counter;
  // Switch pointer
  void *swPtr;
  // Main loop for the system
  // Increments dimensionality indefinitely
  // Currently limits the dimensionality though
  while (dim <= 20)
  {
    // Inital RNG seed
    srand(static_cast<unsigned int>(time(nullptr)));

    double randFac = 0.0;

    // Create a file to store the data for this dimension
    char fileName[13];
    sprintf(fileName, "anvData%d.csv", dim);
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
    double *velSum = (double*)malloc(sizeof(double)*dim);
    double *xSum = (double*)malloc(sizeof(double)*dim);
    double *spare1;
    double *spare2;

    // Start the random rotation factor high, to be reduced over the course of
    // the simulation
    randFac = randStart;

    // Carry out sim
    for (step=0; randFac>=0; step++)
    {
      // Set the sum of velocity vectors to 0
      setAll(velSum, 0, dim);

      // Finding the new direction vector for each particle
      for (i=0; i<count; i++)
      {
        // Add the particles' current velocity vector to the velSum vector
        spare1 = resolveCart(swarm[i].curPhi);
        spare2 = arrMult(spare1, vel, dim);
        arrAdd(velSum, spare2, dim);
        free(spare1);
        free(spare2);

        // Prepare to sum velocity vectors then find the average
        counter = 0;
        setAll(xSum, 0, dim);

        // The loop that checks for all particles within range
        for (j=0; j<count; j++)
        {
          if ((i==j) || dist(&swarm[i], &swarm[j], len, rad)==1)
          {
            counter++;
            spare1 = resolveCart(swarm[j].curPhi);
            arrAdd(xSum, spare1, dim);
            free(spare1);
          }
        }

        // Normalises the sum of these vectors
        for (j=0; j<dim; j++) {
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
        spare2 = arrMult(spare1, tStep*vel, dim);
        arrAdd(swarm[i].loc, spare2, dim);
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
          printf("%dD:, %d, %.2f: %f\n", dim, step, randFac, avr);
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
    dim++;
  }

  return 0;
}