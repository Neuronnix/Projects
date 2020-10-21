using CommandLine;
using System.Collections.Generic;

namespace CommandOptions
{
  public abstract class InputFilesOptions : BaseOptions
  { 
    [Value(0, MetaName = "InputFiles", HelpText = "Input files")]
    public IEnumerable<string> Files { get; set; }
  }
}