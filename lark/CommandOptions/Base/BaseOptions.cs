using CommandLine;
using System;

namespace CommandOptions
{
  public abstract class BaseOptions
  {
    [Option('v', "verbose", Default = false, HelpText = "Show verbose output")]
    public bool Verbose { get; set;}

  }
}