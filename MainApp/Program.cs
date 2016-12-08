using System ;
using System.IO ;
using System.Runtime.InteropServices ;
using System.Windows.Forms ;

namespace MouseInterception
{
    static class Program
    {
        private static string mBaseDir ;
        private static DebugConfig mDebugConfig ;

        [DllImport( "kernel32.dll" )]
        static extern bool AttachConsole( int processId ) ;
        private const int ATTACH_PARENT_PROCESS = -1 ;

        [STAThread]
        static void Main( string[] args )
        {
            // initialize
            mBaseDir = Application.StartupPath ;
            string baseDir = System.IO.Path.Combine( mBaseDir , "../../../_TEST_" ) ;
            if ( Directory.Exists( baseDir ) )
                mBaseDir = baseDir ;

            // load the debug config
            string fname = getAppRelativePath( "debug.xml" ) ; // FIXME! make this configurable
            mDebugConfig = new DebugConfig( fname ) ;

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

        public static string getAppRelativePath( string relPath )
        {
            // return the full application-relative path
            string path = System.IO.Path.Combine( mBaseDir , relPath );
            return System.IO.Path.GetFullPath( path );
        }

        public static DebugConfig debugConfig { get { return mDebugConfig ; } }

    }
}
