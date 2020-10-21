using System.Collections.Generic;
using CommandLine;
using ListFiles;

namespace Main
{
  public class ListFiles : Command<ListFilesOptions>
  {

    public ListFiles() : base("ls")
    {
      
    }
    public override void ParseOpts(ListFilesOptions opts)
    {
      
    }
    public override void HandleParseError(IEnumerable<Error> errs)
    {
      throw new System.NotImplementedException();
    }

  }
}