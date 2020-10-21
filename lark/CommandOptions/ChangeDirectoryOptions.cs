using System.Collections.Generic;
using CommandLine;
using CommandOptions;

namespace ChangeDirectory
{
  public class ChangeDirectoryOptions : BaseOptions
  {
    private const string DirHelpText = "Directory to change to. If empty, then it will change to the directory in the HOME environment variable if one is defined, otherwise it will do nothing.";

    [Value(0, MetaName = "Dir", HelpText = DirHelpText)]
    public IEnumerable<string> Directory { get; set; }
  }
}