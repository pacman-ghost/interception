using System ;
using System.IO ;
using System.Runtime.InteropServices ;
using System.Windows.Forms ;
using System.Diagnostics ;
using System.Text ;

namespace MouseInterception
{
    static class Program
    {

        public const string APP_NAME = "udMeeces" ;

        private static string mBaseDir ;
        private static AppConfig mAppConfig = null ;
        private static DebugConfig mDebugConfig = null ;
        private static MouseDll mMouseDll = null ;

        private static MainForm mMainForm = null ;

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

            // initialize
            bool hasConsole = AttachConsole( ATTACH_PARENT_PROCESS ) ;

            try
            {
                // load the app config
                string fname = getAppRelativePath( "config.xml" ) ; // FIXME! this s.b. in the user's AppData folder
                mAppConfig = new AppConfig( fname ) ;

                // load the debug config
                fname = getAppRelativePath( "debug.xml" ) ; // FIXME! make this configurable
                mDebugConfig = new DebugConfig( fname ) ;

                // initialize
                if ( hasConsole )
                    System.Console.WriteLine( "" ) ;
                mMouseDll = new MouseDll( new MouseDll.callbackDelegate(onCallback) ) ;
            }
            catch( Exception xcptn )
            {
                string errorMsg = String.Format( "Startup error: {0}" , xcptn.Message ) ;
                if ( hasConsole )
                {
                    System.Console.WriteLine( errorMsg ) ;
                    System.Windows.Forms.SendKeys.SendWait( "{ENTER}" ) ;
                }
                else
                    showErrorMsg( errorMsg ) ;
                return ;
            }

            // run the main program
            if ( hasConsole )
            {
                // run in console mode
                try
                {
                    int exitFlag = 0 ;
                    Console.CancelKeyPress += delegate( object sender , ConsoleCancelEventArgs e )
                    {
                        // Ctrl+C was pressed - tell the main loop to stop
                        exitFlag = 1 ;
                        e.Cancel = true ;
                    } ;
                    // run the main loop (this will block until exitFlag is set)
                    mMouseDll.runMainLoop( out exitFlag ) ;
                }
                catch( Exception xcptn )
                {
                    // NOTE: We get here if the main loop throws an unhandled exception.
                    Debug.Assert( false ) ;
                    onCallback( MouseDll.CBTYPE_FATAL_ERROR , Utils.toUtf8(xcptn.Message) ) ;
                }
                System.Windows.Forms.SendKeys.SendWait( "{ENTER}" ) ;
            }
            else
            {
                // run with a GUI
                Application.EnableVisualStyles() ;
                Application.SetCompatibleTextRenderingDefault( false ) ;
                mMainForm = new MainForm() ;
                Application.Run( mMainForm ) ;
            }
        }

        public static void onCallback( int callbackType , IntPtr pCallbackMsg )
        {
            // handle the callback
            string callbackMsg = (pCallbackMsg != null) ? Utils.fromUtf8(pCallbackMsg) : ""  ;
            switch( callbackType )
            {
                case MouseDll.CBTYPE_STARTED:
                case MouseDll.CBTYPE_STOPPED:
                    //System.Console.WriteLine( callbackMsg ) ;
                    break ;
                case MouseDll.CBTYPE_FATAL_ERROR:
                    string errorMsg = String.Format( "FATAL ERROR: {0}" , callbackMsg ) ;
                    System.Console.WriteLine( errorMsg ) ;
                    if ( Program.mainForm != null )
                        Program.showErrorMsg( errorMsg ) ;
                    Application.Exit() ;
                    break ;
                default:
                    Debug.Assert( false ) ;
                    System.Console.WriteLine( String.Format( "UNKNOWN CALLBACK {0}: {1}" , callbackType , callbackMsg ) ) ;
                    break ;
            }
        }

        public static string getAppRelativePath( string relPath )
        {
            // return the full application-relative path
            string path = System.IO.Path.Combine( mBaseDir , relPath );
            return System.IO.Path.GetFullPath( path );
        }

        public static void showInfoMsg( string msg ) { MessageBox.Show(msg,APP_NAME,MessageBoxButtons.OK,MessageBoxIcon.Information) ; }
        public static void showWarningMsg( string msg ) { MessageBox.Show(msg,APP_NAME,MessageBoxButtons.OK,MessageBoxIcon.Warning) ; }
        public static void showErrorMsg( string msg ) { MessageBox.Show(msg,APP_NAME,MessageBoxButtons.OK,MessageBoxIcon.Error) ; }

        public static AppConfig appConfig { get { return mAppConfig ; } }
        public static DebugConfig debugConfig { get { return mDebugConfig ; } }
        public static MouseDll mouseDll { get { return mMouseDll ; } }
        public static MainForm mainForm { get { return mMainForm ; } }

    }
}
