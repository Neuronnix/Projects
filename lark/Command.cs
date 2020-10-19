using System.Collections.Generic;
using System;
using CommandLine;

namespace Main
{
  public abstract class Command<T> : IBaseParser<T>, IRunner
  {
    
    // public abstract void Run(string[] args);
    protected readonly string cmdname;

    public Command(string cmdname)
    {
      this.cmdname = cmdname;
    }

    public void Run(string[] args)
    {
      ((IBaseParser<T>)(this)).Parse(args);
    }
    

    public abstract void ParseOpts(T opts);
    public abstract void HandleParseError(IEnumerable<Error> errs);
  }
}