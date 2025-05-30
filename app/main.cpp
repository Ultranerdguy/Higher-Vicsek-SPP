#include "HDVApplication.hpp"

int main(int argc, char* argv[])
{
  HDV::Application app;
  app.ParseArguments(argc-1, argv+1); // Trim the program name
  return app.RunMain(); 
}

