#ifndef _HDV_APPLICATION_HPP_
#define _HDV_APPLICATION_HPP_

#include <functional>
#include <vector>
#include <filesystem>

#include "HDVCmdParser.hpp"
#include "HDVSimulation.hpp"

namespace HDV
{
  class Application
  {
  public:
    Application();

    void ParseArguments(int argc, char** argv);

    int RunMain();

    Simulation const& GetSimulation() const noexcept;
    Simulation& GetSimulation() noexcept;

  private:
    // Parser setup
    void CreateArguments();
    void RegisterArguments();

    // Parser callbacks
    char** ParseArg_help(char** begin, char** end);
    char** ParseArg_version(char** begin, char** end);
    char** ParseArg_config(char** begin, char** end);
    char** ParseArg_output_directory(char** begin, char** end);
    char** ParseArg_output_name(char** begin, char** end);
    char** ParseArg_dimensions(char** begin, char** end);
    char** ParseArg_particle_count(char** begin, char** end);
    char** ParseArg_search_radius(char** begin, char** end);
    char** ParseArg_speed(char** begin, char** end);
    char** ParseArg_random_rotation(char** begin, char** end);
    char** ParseArg_random_period(char** begin, char** end);
    char** ParseArg_record_period(char** begin, char** end);

    // Application runtime behaviours
    int RunApp();
    int RunVersion();
    int RunHelp();

    // Variables
    CmdParser m_parser;
    std::vector<CmdParser::Argument> m_arguments;
    std::function<int()> m_main{[this](){return RunApp();}};
    Simulation m_simulation;
  };
}

#endif // #ifndef _HDV_APPLICATION_HPP_