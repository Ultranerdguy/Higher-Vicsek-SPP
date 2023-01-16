#include <iostream>
#include "HDVApplication.hpp"

int RETURN_CODE = -1;

int main(int argc, char** argv)
{
  std::filesystem::current_path("D:\\Users\\Sam Clifton\\source\\repos\\Higher-Vicsek-SPP\\tests\\");
  HDV::Application app;
  app.ParseArguments(argc-1, argv+1);

  std::string_view arg;
  if (argc == 1) // Successful build, no arguments
  {
    RETURN_CODE = 0;
  }
  else arg = argv[1];
  if (arg == "--output-directory" && app.GetOutputDirectory() == std::filesystem::path("ExampleDir/Sub1/Sub2/"))
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--output-name" && app.GetOutputFilename() == "ExampleOutput")
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--dimensions" && app.GetDimensions() == 4)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--particle-count" && app.GetParticleCount() == 200)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--search-radius" && app.GetSearchRadius() == 5)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--speed" && app.GetSpeed() == 6)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--random-rotation" && app.GetRandomRotation() == 3)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--random-period" && app.GetRandomPeriod() == 11)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--record-period" && app.GetRecordPeriod() == 13)
  {
    RETURN_CODE = 0;
  }
  else if (arg == "--config" && app.GetOutputFilename() == "ConfigFileout")
  {
    RETURN_CODE = 0;
  }
  return RETURN_CODE;
}