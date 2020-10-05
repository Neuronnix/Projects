
using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using Command;

namespace CatCommand
{
  public class Cat : ICommand
  {
    //class for handling Cat.
    public void Run(string cmd)
    {
      var args = cmd.Split(' ');
      if (args.Length < 2)
      {
        //could also read from stdin too.
        Console.WriteLine("cat: no file name given.");
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
    private void ParseOpts(string cmd)
    {

    }
  }
}