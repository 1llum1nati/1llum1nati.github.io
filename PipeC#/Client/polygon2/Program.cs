using System;
using System.IO;
using System.IO.Pipes;
using System.Threading;

namespace PipeApplication1
{
    class ProgramPipeTest
    {

        public void ThreadStartClient(object obj)
        {
            // Ensure that we only start the client after the server has created the pipe
            ManualResetEvent SyncClientServer = (ManualResetEvent)obj;

            // Only continue after the server was created -- otherwise we just fail badly
            // SyncClientServer.WaitOne();

            using (NamedPipeClientStream pipeStream = new NamedPipeClientStream("mytestpipe"))
            {
                // The connect function will indefinately wait for the pipe to become available
                // If that is not acceptable specify a maximum waiting time (in ms)
                pipeStream.Connect();
                FileStream file = new FileStream("/home/r3pl1c4nt/Projects/2.txt", FileMode.Open);
                Console.WriteLine("[Client] Pipe connection established");
                using (StreamWriter sw = new StreamWriter(pipeStream))
                {
                    sw.AutoFlush = true;
                    StreamReader sr = new StreamReader(file);
                    sw.WriteLine(sr.ReadLine());
                    sr.Dispose();
                }
            }
        }

        static void Main(string[] args)
        {

            // To simplify debugging we are going to create just one process, and have two tasks
            // talk to each other. (Which is a bit like me sending an e-mail to my co-workers)

            ProgramPipeTest Client = new ProgramPipeTest();

            Thread ClientThread = new Thread(Client.ThreadStartClient);
            ClientThread.Start();
        }
    }
}