using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using ShellSpace;

namespace lark
{
    class Program
    {
        static int Main(string[] args)
        {
            //Introductory message
            //Parse command line options
            //give shell prompt and loop.
            Shell sh = new Shell(args);
            //Parse command line options.
            string file;
            if (args.Length > 0)
            {
                Console.WriteLine(args[0]);
                file = args[0];
                //parse args here.
            }
            else
            {
                file = "sample.txt";
                //just go straight to input loop.
            }

            
            return 0;
        }

        

    }
}
