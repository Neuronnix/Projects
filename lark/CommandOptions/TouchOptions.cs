using System;
using System.Collections.Generic;
using CommandLine;
using CommandOptions;

namespace Touch
{
  public class TouchOptions : InputFilesOptions
  {
    // [Value(0, MetaName = "InputFiles", HelpText = "Input files")]
    // public IEnumerable<string> Files { get; set; }

    [Option('a', "access-time", HelpText = "Change only the access time")]
    public bool ChangeAccessTime { get; set; }

    [Option('c', "no-create", HelpText = "Do not create any files")]
    public bool NoCreateFiles { get; set; }
  }
}