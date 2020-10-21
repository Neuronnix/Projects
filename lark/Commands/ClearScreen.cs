using System;

namespace Main
{
  public class ClearScreen : IRunner
  {
    public void Run(string[] args)
    {
      Console.Clear();
    }
  }
}