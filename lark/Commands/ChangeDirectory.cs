using System;
using System.Collections.Generic;
using CommandLine;
using ChangeDirectory;

namespace Main
{
  public class ChangeDirectory : Command<ChangeDirectoryOptions>
  {
    public ChangeDirectory() : base("cd") { }

    public override void ParseOpts(ChangeDirectoryOptions opts)
    {
      
    }
    public override void HandleParseError(IEnumerable<Error> errs)
    {
      
    }

  }
}
