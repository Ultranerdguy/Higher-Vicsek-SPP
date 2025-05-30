#ifndef _HDV_SIMULATION_HPP_
#define _HDV_SIMULATION_HPP_

#include <filesystem>
#include <vector>
#include <string>

#include "HDVParticle.hpp"

namespace HDV
{
  class Simulation
  {
  public:
    Simulation() = default;

    void SetOutputDirectory(std::filesystem::path outdir) noexcept;
    std::filesystem::path const& GetOutputDirectory() const noexcept;
    void SetOutputFilename(std::string name) noexcept;
    std::string const& GetOutputFilename() const noexcept;
    void SetDimensions(unsigned int dimensionCount) noexcept;
    unsigned int GetDimensions() const noexcept;
    void SetParticleCount(unsigned int particleCount) noexcept;
    unsigned int GetParticleCount() const noexcept;
    void SetSearchRadius(HDV::Float searchRadius) noexcept;
    HDV::Float GetSearchRadius() const noexcept;
    void SetSpeed(HDV::Float speed) noexcept;
    HDV::Float GetSpeed() const noexcept;
    void SetRandomRotationStart(HDV::Float randomRotationStart) noexcept;
    HDV::Float GetRandomRotationStart() const noexcept;
    void SetRandomRotationEnd(HDV::Float randomRotationEnd) noexcept;
    HDV::Float GetRandomRotationEnd() const noexcept;
    void SetRandomRotationStep(HDV::Float randomRotationStep) noexcept;
    HDV::Float GetRandomRotationStep() const noexcept;
    void SetRandomPeriod(unsigned int randomPeriod) noexcept;
    unsigned int GetRandomPeriod() const noexcept;
    void SetRecordPeriod(unsigned int recordPeriod) noexcept;
    unsigned int GetRecordPeriod() const noexcept;

    int Run();
    int RunSim();

  private:
    // Config variables
    std::filesystem::path m_outputDirectory = std::filesystem::current_path();
    std::string m_outputFilename = "HDVResults.csv";
    unsigned int m_dimensionCount = 2;
    unsigned int m_particleCount = 100;
    HDV::Float m_searchRadius = 0.1f;
    HDV::Float m_speed = 0.01f;
    HDV::Float m_randomRotationStart = 0.05f;
    HDV::Float m_randomRotationEnd = m_randomRotationStart;
    HDV::Float m_randomRotationStep = 0.05f;
    unsigned int m_randomPeriod = 100;
    unsigned int m_recordPeriod = 100;

    // Simulation variables
    HDV::RandomGenerator m_generator;
    std::vector<HDV::Particle> m_particles;
  };
}

#endif // #ifndef _HDV_SIMULATION_HPP_

