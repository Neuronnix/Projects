using System;
using CommandLine;
using CommandOptions;

namespace MakeDirectory
{
  public class MakeDirectoryOptions : BaseOptions
  {
    [Option('a', "physical", HelpText = "Print the physical directory, without any symbolic links")]
    public bool AvoidSymbolicLinks { get; set; }
  }
}