#include <iostream>
#include "HDVApplication.hpp"
#include "util.hpp"

int RETURN_CODE = -1;

int main(int argc, char** argv)
{
  HDV::Application app;
  app.ParseArguments(argc-1, argv+1);

  auto& sim = app.GetSimulation();

  std::string_view arg;
  if (argc == 1) // Successful build, no arguments
  {
    RETURN_CODE = 0;
  }
  else arg = argv[1];
  if (arg == "--output-directory" && sim.GetOutputDirectory() == std::filesystem::path("ExampleDir/Sub1/Sub2/"))
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--output-name" && sim.GetOutputFilename() == "ExampleOutput")
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--dimensions" && sim.GetDimensions() == 4)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--particle-count" && sim.GetParticleCount() == 200)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--search-radius" && sim.GetSearchRadius() == 5)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--speed" && sim.GetSpeed() == 6)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--random-rotation" 
    && sim.GetRandomRotationStart() == 3
    && sim.GetRandomRotationEnd() == 4
    && Equal(sim.GetRandomRotationStep(), 0.1,1e-5))
  {
    RETURN_CODE = 0;
  }
  
  else if (arg == "--random-period" && sim.GetRandomPeriod() == 11)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--record-period" && sim.GetRecordPeriod() == 13)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--config" && sim.GetOutputFilename() == "ConfigFileout")
  {
    RETURN_CODE = 0;
  }
  return RETURN_CODE;
}

