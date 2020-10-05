namespace Command
{
  interface ICommand
  {
    //So every command will have parse options?
    //
    public void Run(string cmd);
  }
}