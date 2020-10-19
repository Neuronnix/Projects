namespace Main
{

  public static class Utils
  {
    public static bool IsComment(string cmd)
    {
      return cmd[0] == '#';
    }

    public static string  RemoveComment(string args)
    {
      //Split string into two after the first '#'
      //Separates between the command and comment, if there is one.

      return args.Split('#',2)[0];
    }
  }
}