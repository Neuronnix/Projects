using System;
using System.IO;
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
      string dir = opts.Directory;
      if (string.IsNullOrEmpty(dir))
      {
        //If empty it should change to HOME directory.
        Fail($"No directory given");
      }
      else if (!Directory.Exists(dir))
      {
        Fail($"{dir}: No such directory");
      }
      else
      {
        if (dir == "-")
        {
          //switch to previous dir, and change oldpwd to pwd.
        }
        else
        {
          try
          {
            Directory.SetCurrentDirectory(opts.Directory);
          }
          catch (System.Exception e)
          {
              System.Console.WriteLine(e.Message);
              Fail($"{dir}: Invalid directory");
          }
        }
      }
    }
    public override void HandleParseError(IEnumerable<Error> errs)
    {

    }

  }
}
