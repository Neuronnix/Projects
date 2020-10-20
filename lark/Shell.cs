using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using CommandLine;
using static Main.Utils;
namespace Main
{
  public class Shell : IBaseParser<ShellOptions>
  {
    private string Prompt {get;  set; }
    private Dictionary<String, Type> Commands = new Dictionary<string, Type>
    {
      {"cat", typeof(Cat)}
    };
    private Dictionary<String, String> Aliases;
    private Dictionary<String, String> Env;
    
    public Shell(string[] args)
    {
      // ((Shell)(this)).Parse(args);
      ((IBaseParser<ShellOptions>)(this)).Parse(args);
    }

    public void ParseOpts(ShellOptions opts)
    {
      //do nothing yet no options to parse.
      InitShell();
      WelcomeMessage();
      Run();
      Terminate();
    }

    public void HandleParseError(IEnumerable<Error> errs)
    {
      //handle errors.
    }

    private void InitShell()
    {
      //Ideally you would want to read run commands
      //And maybe also read prompt from environment variabes.
      Prompt = ">";
    }

    private void WelcomeMessage()
    {
      Console.WriteLine("Welcome to sh#, the C# interactive shell!");
    }

    private void PrintPrompt()
    {
      Console.Write(Prompt);
    }
    private void Run()
    {
      string input = null;
      for ( ; ; )
      {
        PrintPrompt();
        input = Console.ReadLine();
        if (input == "exit" || input == null) break;
        Execute(input);
      }

    }

    private void Terminate()
    {
      //Do whatever needs to be done to shut down, release resources, close files or whatever.
      Console.WriteLine("logout");
    }

    private void InvalidCommand(string cmd)
    {
      Console.WriteLine($"sh#: {cmd}: Command not found.");
    }
    
    private void Execute(string input)
    {
      string cmd = RemoveComment(input);
      Type cmdType;
      
      //Now know that cmd is not comment. Split up args.
      string[] args = cmd.Split(' ');
      //cmd name is the first word.
      string cmdName = args[0];
      if (string.IsNullOrWhiteSpace(cmdName))
      {
        return;
      }
      else if (Commands.TryGetValue(cmdName, out cmdType))
      {
        var cmdInstance = (IRunner)Activator.CreateInstance(cmdType);
        //Use Skip to pass ignore the first word which is the command, so it doesn't get parsed.
        cmdInstance.Run(args.Skip(1).ToArray());
      }
      else
      {
        InvalidCommand(cmdName);
      }
    }
    

    private void RunCommand(Type cmdType, string[] args)
    {
      Console.WriteLine("In Run Command");
      dynamic cmdInstance = Activator.CreateInstance(cmdType);
      cmdInstance.Run(args); //will this work?
    }
   
    public void Start()
    {

    }
  }
}