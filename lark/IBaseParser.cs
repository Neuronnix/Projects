using System.Collections.Generic;
using CommandLine;

namespace Main
{
  public interface IBaseParser<T>
  {
    public void Parse(string[] args)
    {
      Parser.Default.ParseArguments<T>(args)
                .WithParsed(ParseOpts)
                .WithNotParsed(HandleParseError);
    }

    public void ParseOpts(T opts);
    public void HandleParseError(IEnumerable<Error> errs);
  }
}