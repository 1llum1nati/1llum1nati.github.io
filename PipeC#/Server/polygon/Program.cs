using System;
using System.IO;
using System.IO.Pipes;
using System.Threading;
//using System.Linq;
//using System.Collections.Generic;

namespace PipeApplication1
{
    class ProgramPipeTest
    {

        public void ThreadStartServer()
        {
            // Create a name pipe
            using (NamedPipeServerStream pipeStream = new NamedPipeServerStream("mytestpipe"))
            {
                Console.WriteLine("[Server] Pipe created {0}", pipeStream.GetHashCode());

                // Wait for a connection
                pipeStream.WaitForConnection();
                Console.WriteLine("[Server] Pipe connection established");
                FileStream file = new FileStream("/home/r3pl1c4nt/Projects/1.txt", FileMode.Append);
                
                using (StreamReader sr = new StreamReader(pipeStream))
                {
                    // We read a line from the pipe and print it together with the current time
                    StreamWriter sw = new StreamWriter(file);
                    sw.WriteLine(sr.ReadLine());
                    sw.Dispose();
                }
            }

            //Console.WriteLine("Connection lost");
        }
        /*static AutoResetEvent autoResetEvent = new AutoResetEvent(true);
        public static void First()
        {
            autoResetEvent.WaitOne();
            using (StreamWriter sw = new StreamWriter("/home/r3pl1c4nt/Projects/1.txt", true))
            {
                sw.WriteLine("A");
            }
            autoResetEvent.Set();
        }
        public static void Second()
        {
            autoResetEvent.WaitOne();
            using (StreamReader sr = new StreamReader("/home/r3pl1c4nt/Projects/1.txt", true))
            {
                while(!sr.EndOfStream)
                    Console.WriteLine(sr.ReadLine());
            }
            autoResetEvent.Set();
        }
        public static void Third()
        {

        }*/

        static void Main(string[] args)
        {
            
            // To simplify debugging we are going to create just one process, and have two tasks
            // talk to each other. (Which is a bit like me sending an e-mail to my co-workers)

            ProgramPipeTest Server = new ProgramPipeTest();

            Thread ServerThread = new Thread(Server.ThreadStartServer);

            ServerThread.Start();
            /*Thread thread1 = new Thread(() => First());
            Thread thread2 = new Thread(() => Second());
            Thread thread3 = new Thread(() => Third());
            //thread1.Start();

            //thread2.Start();*/



        }
    }
}