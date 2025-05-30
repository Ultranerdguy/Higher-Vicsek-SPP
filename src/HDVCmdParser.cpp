#include "HDVCmdParser.hpp"
#include <stdexcept>

void HDV::CmdParser::RegisterArgument(HDV::CmdParser::Argument& arg)
{
  m_arguments[arg.name] = arg;
}

void HDV::CmdParser::ParseArguments(char** begin, char** end)
{
  if (!begin)
  {
    return;
  }
  while (begin != end)
  {
    if (!(*begin)) continue;
    std::string_view arg(*begin);
    auto const delimiterSize = m_delimiter.size();
    if (arg.substr(0,delimiterSize) == m_delimiter)
    {
      auto itr = m_arguments.find(arg.substr(delimiterSize));
      if (itr != m_arguments.end())
      {
        begin = itr->second.parser(begin, end);
      }
      else
      {
        std::string errMessage("Invalid argument: ");
        errMessage += *begin;
        throw std::runtime_error(errMessage);
      }
    }
  }
}

std::string const& HDV::CmdParser::GetDelimiter() const
{
  return m_delimiter;
}

