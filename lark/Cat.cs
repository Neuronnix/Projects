
using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using static Main.Utils;
using Cat;
using CommandLine;

namespace Main
{
  class Cat : Command<CatOptions>
  {
    //class for handling Cat.
    
    public Cat() : base("cat")
    {
      
    }
   
    public override void ParseOpts(CatOptions opts)
    {
      foreach (var filename in opts.Files)
      {
        if (filename == "-")
        {
          //Read from stdin and output. Pass opts as dependency
          ReadFromStdIn();
        }
        else
        {
          PrintFile(filename, opts);
        }
      }
    }

    public override void HandleParseError(IEnumerable<Error> errs)
    {
      //Arrive here if parsing errors.
    }
    private void PrintFile(string file, CatOptions opts)
    {
      // System.Console.WriteLine("==================================");
      // System.Console.WriteLine($"printing {file}");
      // System.Console.WriteLine("==================================");

      var lines = ReadLinesFromFile(file);
      foreach (var line in lines)
      {
        //TODO: use opts here to check how to print.
        Console.Write(line);
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
    private void Fail(string msg)
    {
      Console.WriteLine($"{cmdname}: {msg}");
    }
  
  }
}