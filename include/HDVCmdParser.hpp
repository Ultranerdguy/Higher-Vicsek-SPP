#ifndef _HDV_CMDPARSER_HPP_
#define _HDV_CMDPARSER_HPP_

#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>

namespace HDV
{
  class CmdParser
  {
  private:
  public:
    using ParserFunction = std::function<char** (char**,char**)>;
    struct Argument
    {
      std::string name;
      ParserFunction parser;
      std::string description;
    };
    
    CmdParser() = default;

    void RegisterArgument(Argument& arg);
    void ParseArguments(char** begin, char** end);
    
    std::string const& GetDelimiter() const;
  private:

    std::unordered_map<std::string_view, Argument> m_arguments;

    std::string m_delimiter = "--";
  };
}

#endif // #ifndef _HDV_SRC_CMDPARSER_HPP_

