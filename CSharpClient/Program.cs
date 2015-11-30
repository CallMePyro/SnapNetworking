using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

public class SynchronousSocketClient
{

    public static void StartClient(string host, int port, string username, string password)
    {
        // Connect to a remote device.
        try
        {
            // Establish the remote endpoint for the socket.
            // This example uses port 11000 on the local computer.
            IPEndPoint remoteEP = new IPEndPoint(IPAddress.Parse(host), port);

            // Create a TCP/IP  socket.
            Socket sender = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            // Connect the socket to the remote endpoint. Catch any errors.
            try
            {
                sender.Connect(remoteEP);

                Console.WriteLine("Socket connected to {0}",
                    sender.RemoteEndPoint.ToString());

                // Encode the data string into a byte array.
                byte[] msg = Encoding.ASCII.GetBytes("John Smith");
                // Send the data through the socket.
                int bytesSent = sender.Send(msg);

                Thread.Sleep(1000);

                msg = Encoding.ASCII.GetBytes("  14   1Hello from C#firip!");
                sender.Send(msg);

                // Release the socket.
                sender.Shutdown(SocketShutdown.Both);
                sender.Close();

            }
            catch (ArgumentNullException ane)
            {
                Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
            }
            catch (SocketException se)
            {
                Console.WriteLine("SocketException : {0}", se.ToString());
            }
            catch (Exception e)
            {
                Console.WriteLine("Unexpected exception : {0}", e.ToString());
            }

        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }
    }

    public static int Main(String[] args)
    {
        Console.WriteLine("Enter the host: ");
        string host = Console.ReadLine();

        Console.WriteLine("Enter the port: ");
        int port = Int32.Parse(Console.ReadLine());

        Console.WriteLine("Enter your username: ");
        string username = Console.ReadLine();


        StartClient(host, port, username, "password");
        Console.WriteLine("connecting...");
        int t = Console.Read();
        return 0;


    }
}