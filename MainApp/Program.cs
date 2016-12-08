using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace MouseInterception
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {

            // FIXME!
            MouseDll mouseDll = new MouseDll() ;

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault( false );
            Application.Run( new MainForm() );
        }
    }
}
