using System.Collections.Generic;
using System;
using CommandLine;

namespace Main
{
  public abstract class Command<T> : IBaseParser<T>, IRunner
  {
    
    // public abstract void Run(string[] args);
    protected readonly string cmdname;
    protected readonly Parser parser;
    protected readonly IBaseParser<T> thisAsBaseParser;

    public Command(string cmdname)
    {
      this.cmdname = cmdname;
      thisAsBaseParser = ((IBaseParser<T>)(this));
      parser = new Parser(thisAsBaseParser.SetupParser);
    }

    public void Run(string[] args)
    {
      thisAsBaseParser.Parse(args, parser);
    }

    public abstract void ParseOpts(T opts);
    public abstract void HandleParseError(IEnumerable<Error> errs);

    protected void Fail(string msg)
    {
      Console.WriteLine($"{cmdname}: {msg}");
    }
  }
}