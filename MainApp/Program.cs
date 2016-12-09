using System ;
using System.IO ;
using System.Runtime.InteropServices ;
using System.Windows.Forms ;

namespace MouseInterception
{
    static class Program
    {
        private static string mBaseDir ;
        private static AppConfig mAppConfig ;
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

            // load the app config
            string fname = getAppRelativePath( "config.xml" ) ; // FIXME! this s.b. in the user's AppData folder
            mAppConfig = new AppConfig( fname ) ;

            // load the debug config
            fname = getAppRelativePath( "debug.xml" ) ; // FIXME! make this configurable
            mDebugConfig = new DebugConfig( fname ) ;

            if ( args.Length > 0 )
            {
                // run in console mode
                AttachConsole( ATTACH_PARENT_PROCESS ) ;
                System.Console.WriteLine( "\n>>> Initialized the C# console." ) ; // FIXME!
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

        public static AppConfig appConfig { get { return mAppConfig ; } }
        public static DebugConfig debugConfig { get { return mDebugConfig ; } }

    }
}
