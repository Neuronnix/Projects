using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using CommandLine;
using static Main.Utils;
namespace Main
{
  public class Shell : IBaseParser<ShellOptions>
  {
    private Dictionary<String, Type> Commands = new Dictionary<string, Type>
    {
      {"cat", typeof(ConcatenateFiles)},
      {"ls", typeof(ListFiles)},
      {"cls", typeof(ClearScreen)},
      {"pwd", typeof(PrintWorkingDirectory)},
      {"touch", typeof(Touch)},
      {"mkdir", typeof(MakeDirectory)},
      {"cd", typeof(ChangeDirectory)}
      //{"alias", typeof(SetAlias)}
      //{"set", typeof(SetAlias)}
      //{"env", typeof(SetAlias)}
    };
    private Dictionary<String, String> Aliases = new Dictionary<String, String>
    {
      {"clear", "cls"}
    };
    private Dictionary<String, String> Env = new Dictionary<String, String>
    {
      {"PS1", ">"}
    };

    private string Prompt { get; set; }
    
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
      //Do run commands or something like that
      //Get environment variables, home directory, current working directory, etc.
      //Set prompt.
      //Also get command history
      Prompt = Env["PS1"];
      var enVars = Environment.GetEnvironmentVariables();
      foreach (DictionaryEntry de in enVars)
      {
        System.Console.WriteLine($"{de.Key}={de.Value}");
      }
    }

    private void WelcomeMessage()
    {
      Console.WriteLine("Welcome to bash#, a mini bash-like shell made with C#.");
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
      //Shut down code in here. Save stuff, close resources, etc.
      Console.WriteLine("logout");
    }

    private void InvalidCommand(string cmd)
    {
      Console.WriteLine($"sh#: {cmd}: Command not found.");
    }
    

    private string[] ParseCommand(string input)
    {
      //Remove comments and split into arguments
      return RemoveComment(input).Split(' ');
    }
    private void Execute(string input)
    {
      string[] args = ParseCommand(input);
      //cmd name is the first word.
      string cmdName = args[0];
      if (string.IsNullOrWhiteSpace(cmdName))
      {
        return;
      }
      else if (Commands.TryGetValue(cmdName, out Type cmdType))
      {
        var cmdInstance = (IRunner)Activator.CreateInstance(cmdType);
        //Use Skip to pass ignore the first word which is the command, so it doesn't get parsed.
        cmdInstance.Run(args.Skip(1).ToArray());
      }
      else if (Aliases.TryGetValue(cmdName, out string alias))
      {
        System.Console.WriteLine($"Running alias {alias}");
        //command is an alias instead. Run as command.
        //TODO: Make quicker way to run alias, as using Execute will remove
        //comments twice when comments already been removed. (unless 
        //there's comments in the alias, but that wouldn't make sense anyway).
        Execute(alias);
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