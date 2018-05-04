using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace teemo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            
            InitializeComponent();
            
            this.WindowState = FormWindowState.Minimized;
            
            notifyIcon.BalloonTipIcon = ToolTipIcon.Info;
            notifyIcon.BalloonTipText = "Application minimized ";

            notifyIcon.ShowBalloonTip(5000);
            this.WindowState = FormWindowState.Minimized;
            //this.Visible = false;
            this.ShowInTaskbar = false;
        }

        private void notifyIcon_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            this.WindowState = FormWindowState.Normal;
        }
    }
}
