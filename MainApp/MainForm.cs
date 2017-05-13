/*
 * Copyright 2017 Taka Muraoka
 *
 * This file is part of the MouseInterception project.
 *
 * The MouseInterception project is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * The MouseInterception project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the MouseInterception project.  If not, see <http://www.gnu.org/licenses/>.
 */

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
