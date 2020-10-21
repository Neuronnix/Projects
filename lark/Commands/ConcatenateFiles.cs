
using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using static Main.Utils;
using ConcatenateFiles;
using CommandLine;
using System.Linq;

namespace Main
{
  class ConcatenateFiles : Command<ConcatenateFilesOptions>
  {
    //class for handling Cat.
    
    public ConcatenateFiles() : base("cat")
    {
      
    }
   
    public override void ParseOpts(ConcatenateFilesOptions opts)
    {
      if (!opts.Files.Any())
      {
        ReadFromStdIn();
        return;
      }

      foreach (var filename in opts.Files)
      {
        if (filename == "-")
        {
          System.Console.WriteLine("hyphen");
          //Read from stdin and output. Pass opts as dependency
          ReadFromStdIn();
        }
        else
        {
          if (File.Exists(filename))
          {
            PrintFile(filename, opts);
          }
          else if (Directory.Exists(filename))
          {
            Fail($"{filename}: is a directory");
          }
          else
          {
            Fail($"{filename}: No such file or directory");
          }
        }
      }
    }

    public override void HandleParseError(IEnumerable<Error> errs)
    {
      //Arrive here if parsing errors.
    }
    private void PrintFile(string file, ConcatenateFilesOptions opts)
    {
      // System.Console.WriteLine("==================================");
      // System.Console.WriteLine($"printing {file}");
      // System.Console.WriteLine("==================================");

      var lines = ReadLinesFromFile(file);
      foreach (var line in lines)
      {
        //TODO: use opts here to check how to print.
        Console.WriteLine(line);
      }
    }

    private IEnumerable<string> ReadLinesFromFile(string file)
    {
      string line;
      using (var reader = File.OpenText(file))
      {
        while ((line = reader.ReadLine()) != null)
        {
          yield return line;
        }
      }
    }

    private void ReadFromStdIn()
    {
      string input = null;
      while ((input = Console.ReadLine()) != null)
      {
        Console.WriteLine(input);
      }
    }
    
  
  }
}