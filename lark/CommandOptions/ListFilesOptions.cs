using System.Collections.Generic;
using CommandLine;
using CommandOptions;

namespace ListFiles
{
  public class ListFilesOptions : BaseOptions
  {
    [Value(0, MetaName = "Files/Directories", HelpText = "Files to show information about (current directory is used by default if no files/directories are given.")]
    public IEnumerable<string> Files { get; set; }

    [Option('l', "long-format", HelpText = "use a long listong format")]
    public bool LongFormat { get; set; }

    [Option('a', "all", HelpText = "Do not ignore entries starting with a .")]
    public bool All { get; set; }
  }
}