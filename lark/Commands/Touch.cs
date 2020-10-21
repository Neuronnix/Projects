using System;
using System.Collections.Generic;
using CommandLine;
using Touch;

namespace Main
{
  public class Touch : Command<TouchOptions>
  {
    public Touch() : base("touch") { }

    public override void ParseOpts(TouchOptions opts)
    {
      
    }
    public override void HandleParseError(IEnumerable<Error> errs)
    {
      
    }

  }
}
