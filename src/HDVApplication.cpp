#include "HDVApplication.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include "hdv_config.h"

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

HDV::Simulation const& HDV::Application::GetSimulation() const noexcept
{
  return m_simulation;
}

HDV::Simulation& HDV::Application::GetSimulation() noexcept
{
  return m_simulation;
}

void HDV::Application::CreateArguments()
{
  m_arguments.reserve(12);
  m_arguments.emplace_back(CmdParser::Argument{
    "help"
    ,[this](auto ... args){return ParseArg_help(args...);}
    ,"Display help."
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "version"
    ,[this](auto ... args){return ParseArg_version(args...);}
    ,"Display the program version."
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "config"
    ,[this](auto ... args){return ParseArg_config(args...);}
    ,"Specify a config file to load settings from.\n\tConfig file takes the same format as command line arguments, with one argument per line."
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "output-directory"
    ,[this](auto ... args){return ParseArg_output_directory(args...);}
    ,"Specify the output directory for simulation results. Defaults to current working directory."
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "output-name"
    ,[this](auto ... args){return ParseArg_output_name(args...);}
    ,"Spcify the name for the output file. Defaults to \"HDV_<dimensions>.csv\"."
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "dimensions"
    ,[this](auto ... args){return ParseArg_dimensions(args...);}
    ,"Specify the number of dimensions to simulate in. Defaults to 2."
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "particle-count"
    ,[this](auto ... args){return ParseArg_particle_count(args...);}
    ,"Specify the number of particles to simulate. Defaults to 100."
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "search-radius"
    ,[this](auto ... args){return ParseArg_search_radius(args...);}
    ,"Specify the search radius of each particle. Defaults to 0.1."
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "speed"
    ,[this](auto ... args){return ParseArg_speed(args...);}
    ,"Specify the speed of each particle (distance per tick). Defaults to 0.01."
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "random-rotation"
    ,[this](auto ... args){return ParseArg_random_rotation(args...);}
    ,"Specify the maximum amount of random rotation to apply to each particle. Defaults to 0.05."
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "random-period"
    ,[this](auto ... args){return ParseArg_random_period(args...);}
    ,"Specify the number of ticks between applications of random rotation. Defaults to 100."
  });
  m_arguments.emplace_back(CmdParser::Argument{
    "record-period"
    ,[this](auto ... args){return ParseArg_record_period(args...);}
    ,"Specify the number of steps between each record of values. Defaults to 100."
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
  m_simulation.SetOutputDirectory(outDir);
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
  m_simulation.SetOutputFilename(*begin);
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
  unsigned int dimensionCount{};
  if (!(dimStrm >> dimensionCount))
  {
    throw std::runtime_error(std::string("Unable to read dimensions: ") + *begin);
  }
  m_simulation.SetDimensions(dimensionCount);
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
  unsigned int particleCount{};
  if (!(particleCountStrm >> particleCount))
  {
    throw std::runtime_error(std::string("Unable to read particle count: ") + *begin);
  }
  m_simulation.SetParticleCount(particleCount);
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
  double searchRadius{};
  if (!(searchRadiusStrm >> searchRadius))
  {
    throw std::runtime_error(std::string("Unable to read search radius: ") + *begin);
  }
  m_simulation.SetSearchRadius(searchRadius);
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
  double speed{};
  if (!(speedStrm >> speed))
  {
    throw std::runtime_error(std::string("Unable to read speed: ") + *begin);
  }
  m_simulation.SetSpeed(speed);
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
  double randomRotation{};
  if (!(randomRotationStrm >> randomRotation))
  {
    throw std::runtime_error(std::string("Unable to read random rotation: ") + *begin);
  }
  m_simulation.SetRandomRotation(randomRotation);
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
  unsigned int randomPeriod{};
  if (!(randomPeriodStrm >> randomPeriod))
  {
    throw std::runtime_error(std::string("Unable to read random period: ") + *begin);
  }
  m_simulation.SetRandomPeriod(randomPeriod);
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
  unsigned int recordPeriod{};
  if (!(recordPeriodStrm >> recordPeriod))
  {
    throw std::runtime_error(std::string("Unable to read record period: ") + *begin);
  }
  m_simulation.SetRecordPeriod(recordPeriod);
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
  return m_simulation.Run();
}