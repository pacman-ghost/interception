using System ;
using System.Runtime.InteropServices ;
using System.Windows.Forms ;

namespace MouseInterception
{
    static class Program
    {
        [DllImport( "kernel32.dll" )]
        static extern bool AttachConsole( int processId ) ;
        private const int ATTACH_PARENT_PROCESS = -1 ;

        [STAThread]
        static void Main( string[] args )
        {

            if ( args.Length > 0 )
            {
                // run in console mode
                AttachConsole( ATTACH_PARENT_PROCESS ) ;
                System.Console.WriteLine( "\nInitialized the C# console." ) ; // FIXME!
                MouseDll mouseDll = new MouseDll( true ) ;
                System.Windows.Forms.SendKeys.SendWait( "{ENTER}" ) ;
                Application.Exit() ;
            }
            else
            {
                // run with a GUI
                MouseDll mouseDll = new MouseDll( false ) ;
                Application.EnableVisualStyles() ;
                Application.SetCompatibleTextRenderingDefault( false ) ;
                Application.Run( new MainForm() ) ;
            }
        }
    }
}
