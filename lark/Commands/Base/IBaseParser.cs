using System.Collections.Generic;
using CommandLine;

namespace Main
{
  public interface IBaseParser<T>
  {
    public void Parse(string[] args, Parser parser)
    {
      parser.ParseArguments<T>(args)
                .WithParsed(ParseOpts)
                .WithNotParsed(HandleParseError);
    }

    public void Parse(string[] args)
    {
      Parse(args, Parser.Default);
    }

    public void SetupParser(ParserSettings ps)
    {
      ps.EnableDashDash = true;
    }
    public void ParseOpts(T opts);
    public void HandleParseError(IEnumerable<Error> errs);
  }
}