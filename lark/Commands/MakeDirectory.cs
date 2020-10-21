using System;
using System.Collections.Generic;
using CommandLine;
using MakeDirectory;

namespace Main
{
  public class MakeDirectory : Command<MakeDirectoryOptions>
  {
    public MakeDirectory() : base("mkdir") { }

    public override void ParseOpts(MakeDirectoryOptions opts)
    {
      
    }
    public override void HandleParseError(IEnumerable<Error> errs)
    {
      
    }

  }
}
