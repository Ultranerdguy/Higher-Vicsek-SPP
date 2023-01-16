#ifndef _HDV_APPLICATION_HPP_
#define _HDV_APPLICATION_HPP_

#include <functional>
#include <vector>
#include <filesystem>

#include "HDVCmdParser.hpp"

namespace HDV
{
  class Application
  {
  public:
    Application();

    void ParseArguments(int argc, char** argv);

    int RunMain();

    std::filesystem::path const& GetOutputDirectory() const noexcept;
    std::string_view GetOutputFilename() const noexcept;
    unsigned int GetDimensions() const noexcept;
    unsigned int GetParticleCount() const noexcept;
    double GetSearchRadius() const noexcept;
    double GetSpeed() const noexcept;
    double GetRandomRotation() const noexcept;
    unsigned int GetRandomPeriod() const noexcept;
    unsigned int GetRecordPeriod() const noexcept;

  private:
    // Parser setup
    CmdParser m_parser;
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

    std::function<int()> m_main{[this](){return RunApp();}};

    // All arguments, for referencing by "--help"
    std::vector<CmdParser::Argument> m_arguments;

    // Variables
    std::filesystem::path m_outputDirectory;
    std::string m_outputFilename;
    unsigned int m_dimensionCount = 2;
    unsigned int m_particleCount = 100;
    double m_searchRadius = 0.1;
    double m_speed = 0.01;
    double m_randomRotation = 0.05;
    unsigned int m_randomPeriod = 100;
    unsigned int m_recordPeriod = 100;
  };
}

#endif // #ifndef _HDV_APPLICATION_HPP_