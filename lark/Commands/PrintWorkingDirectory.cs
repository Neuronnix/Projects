using System;
using System.IO;
using System.Collections.Generic;
using CommandLine;
using PrintWorkingDirectory;

namespace Main
{
  public class PrintWorkingDirectory : Command<PrintWorkingDirectoryOptions>
  {
    public PrintWorkingDirectory() : base("pwd") {}

    public override void ParseOpts(PrintWorkingDirectoryOptions opts)
    {
      System.Console.WriteLine(Directory.GetCurrentDirectory());
    }
    public override void HandleParseError(IEnumerable<Error> errs)
    {
      
    }

  }
}
