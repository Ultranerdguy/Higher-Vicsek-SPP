#include <filesystem>
#include <string>

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
    void SetSearchRadius(double searchRadius) noexcept;
    double GetSearchRadius() const noexcept;
    void SetSpeed(double speed) noexcept;
    double GetSpeed() const noexcept;
    void SetRandomRotation(double randomRotation) noexcept;
    double GetRandomRotation() const noexcept;
    void SetRandomPeriod(unsigned int randomPeriod) noexcept;
    unsigned int GetRandomPeriod() const noexcept;
    void SetRecordPeriod(unsigned int recordPeriod) noexcept;
    unsigned int GetRecordPeriod() const noexcept;

    int Run();

  private:
    std::filesystem::path m_outputDirectory = std::filesystem::current_path();
    std::string m_outputFilename = "HDVResults.csv";
    unsigned int m_dimensionCount = 2;
    unsigned int m_particleCount = 100;
    double m_searchRadius = 0.1;
    double m_speed = 0.01;
    double m_randomRotation = 0.05;
    unsigned int m_randomPeriod = 100;
    unsigned int m_recordPeriod = 100;
  };
}