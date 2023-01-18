#include <iostream>
#include "HDVCmdParser.hpp"

// Fail test by default
int RETURN_CODE = -1;

char** ParseValidArgument(char**, char**);
char** ParseMultiArgument(char**, char**);

int main(int argc, char** argv)
{
  // Define arguments
  HDV::CmdParser::Argument args[]
  {
    {
      "example_valid"
      , ParseValidArgument
      ,"Parse a valid argument"
    }
    ,{
      "example_multiple"
      , ParseMultiArgument
      ,"Parse multiple arguments"
    }
  };

  // Parse arguments if there are any
  HDV::CmdParser parser;
  if (argc > 1)
  {
    for (auto& a : args) parser.RegisterArgument(a);

    try
    {
      parser.ParseArguments(argv+1, argv+argc);
    }
    catch (std::runtime_error& e)
    {
      if (std::string_view(argv[1]) == "--example_invalid")
      {
        // Error thrown in specific test case. Valid
        RETURN_CODE = 0;
      }
      std::cerr << e.what() << '\n';
    }
  }
  else
  {
    // If no arguments, return success
    RETURN_CODE = 0;
  }

  return RETURN_CODE;
}

// Test case definitions

char** ParseValidArgument(char** begin, char**)
{
  if (std::string_view(*begin) == "--example_valid")
  {
    // If reached by specific test, return success
    RETURN_CODE = 0;
  }
  return begin+1;
}

char** ParseMultiArgument(char** begin, char**)
{
  if (std::string_view(begin[0]) == "--example_multiple"
    && std::string_view(begin[1]) == "arg1"
    && std::string_view(begin[2]) == "arg2")
  {
    // If reached by specific test, return success
    RETURN_CODE = 0;
  }
  return begin+3;
}