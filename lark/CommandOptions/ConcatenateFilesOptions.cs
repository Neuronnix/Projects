using System.Collections.Generic;
using CommandLine;
using CommandOptions;

namespace ConcatenateFiles
{
  class ConcatenateFilesOptions : BaseOptions
  {
    [Value(0, MetaName = "InputFiles", HelpText = "Files to concat and print out")]
    public IEnumerable<string> Files {get; set; }

    [Option('E', "show-ends", Default = false, HelpText = "display $ at end of each line")]
    public bool ShowEnds { get; set; }
  }
}