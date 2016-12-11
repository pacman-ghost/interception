using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;

namespace MouseInterception
{
    public partial class MainForm : Form
    {

        private Thread mMouseDllThread ;
        private int mExitFlag ;

        public MainForm()
        {
            // initialize the MainForm
            InitializeComponent() ;
        }

        private void MainForm_Load( object sender , EventArgs e )
        {
        }

        private void MainForm_Shown( object sender , EventArgs e )
        {
            // start the main loop
            mMouseDllThread = new Thread(
                delegate() {
                    try
                    {
                        mExitFlag = 0 ;
                        Program.mouseDll.runMainLoop( out mExitFlag ) ; 
                    }
                    catch( Exception xcptn )
                    {
                        // NOTE: We get here if the main loop throws an unhandled exception.
                        Debug.Assert( false ) ;
                        Program.onCallback( MouseDll.CBTYPE_FATAL_ERROR , Utils.toUtf8(xcptn.Message) ) ;
                    }
                    Invoke( (MethodInvoker)delegate { Close() ; } ) ; // nb: close the main window
                }
            ) ;
            mMouseDllThread.Start() ;
        }

        private void MainForm_FormClosing( object sender , FormClosingEventArgs e )
        {
            // stop the main loop
            if ( mExitFlag == 0 )
            {
                // NOTE: Setting this flag tells the main loop to end. We cancel the request to close the form, and wait
                //  for the call to runMainLoop() to return and the worker thread to finish, then we close the form.
                mExitFlag = 1 ;
                e.Cancel = true ;
            }
        }
    }
}
