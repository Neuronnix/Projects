
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
      //Command entry point
    }

    public override void HandleParseError(IEnumerable<Error> errs)
    {
      //Arrive here if parsing errors.
    }
    private void PrintFile(string[] args)
    {
      //Split comment and command.
      if (args.Length < 2 || string.IsNullOrWhiteSpace(args[1]))
      {
        //could also read from stdin too.
        Fail("no file name given.");
        return;
      }

      string file = args[1];
      
      var lines = ReadFrom(file);
      foreach (var line in lines)
      {
        Console.Write(line);
        if (!string.IsNullOrWhiteSpace(line))
        {
          var pause = Task.Delay(200);
          pause.Wait();
        }
      }
    }

    private IEnumerable<string> ReadFrom(string file)
    {
      string line;
      using (var reader = File.OpenText(file))
      {
        while ((line = reader.ReadLine()) != null)
        {
          foreach (var word in line.Split(' '))
          {
            yield return word + " ";
          }
          yield return Environment.NewLine;
        }
      }
    }

    private void Fail(string msg)
    {
      Console.WriteLine($"{cmdname}: {msg}");
    }
  
  }
}