using System;
using CommandLine;
using CommandOptions;

namespace PrintWorkingDirectory
{
  public class PrintWorkingDirectoryOptions : BaseOptions
  {
    [Option('P', "physical", HelpText = "Print the physical directory, without any symbolic links")]
    public bool AvoidSymbolicLinks { get; set; }
  }
}