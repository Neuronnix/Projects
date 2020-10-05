using System;
using System.Collections.Generic;
using Command;
using CatCommand;
using System.Text.RegularExpressions;

namespace ShellSpace
{
  public class Shell
  {
    private string prompt {get;  set; }
    private Dictionary<String, ICommand> Commands = new Dictionary<string, ICommand>
    {
      {"cat", new Cat()}
    };
    
    public Shell(string[] args)
    {
      WelcomeMessage();
      ParseOpts(args);
      InitShell();
      Run();
    }

    private void InitShell()
    {
      //Ideally you would want to read run commands
      //And maybe also read prompt from environment variabes.
      prompt = ">";
      //Populate Commands Dictionary?
    }

    private void PrintPrompt()
    {
      Console.Write(prompt);
    }
    private void Run()
    {
      string input = null;
      do
      {
        PrintPrompt();
        input = Console.ReadLine();
        Execute(input);
      } while (input != "exit");
    }

    private void InvalidCommand(string cmd)
    {
      Console.WriteLine($"sh#: {cmd}: Command not found.");
    }
    
    private bool IsComment(string cmd)
    {
      return cmd[0] == '#';
    }
    
    private void Execute(string input)
    {
      var result = (Regex.Match(input, @"^([\w\-]+)"));
      string cmd = result.Value;
      if (string.IsNullOrWhiteSpace(cmd) || IsComment(cmd))
      {
        return;
      }
      else if (Commands.ContainsKey(cmd))
      {
        Commands[cmd].Run(input);
      }
      else
      {
        InvalidCommand(cmd);
      }
      //if input is a valid command
      //Check if comment

    }
    private void WelcomeMessage()
    {
      Console.WriteLine("Welcome to sh#, the C# interactive shell!");
    }
    private void ParseOpts(string[] args)
    {
      //Parse command line options passed to the Shell.
    }
    public void Start()
    {

    }
  }
}