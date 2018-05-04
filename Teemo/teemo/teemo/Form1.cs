using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Diagnostics;
using System.Runtime.InteropServices;
namespace teemo
{
    public partial class Form1 : Form
    {
        public Form1()
        {

            InitializeComponent();
            //this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Resize += Form1_Resize;
            listBox1.SelectionMode = SelectionMode.One;
            var driveInfo = System.IO.DriveInfo.GetDrives();
            foreach (var drive in driveInfo)
            {
                //string str = drive.ToString();
                
                listBox1.Items.Add(drive);
            }
        }
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool GetVolumeNameForVolumeMountPoint(string
            lpszVolumeMountPoint, [Out] StringBuilder lpszVolumeName,
            uint cchBufferLength);

        [DllImport("kernel32.dll")]
        static extern bool DeleteVolumeMountPoint(string lpszVolumeMountPoint);

        [DllImport("kernel32.dll")]
        static extern bool SetVolumeMountPoint(string lpszVolumeMountPoint,
            string lpszVolumeName);

        const int MAX_PATH = 260;
        struct
        {}
        private void RemoveDriveLetter(string letter)
        {
            StringBuilder volume = new StringBuilder(MAX_PATH);
            if (!GetVolumeNameForVolumeMountPoint(letter, volume, (uint)MAX_PATH))
                Marshal.ThrowExceptionForHR(Marshal.GetHRForLastWin32Error());

            if (!DeleteVolumeMountPoint(letter))
                Marshal.ThrowExceptionForHR(Marshal.GetHRForLastWin32Error());

            if (!SetVolumeMountPoint(@"Z:\", volume.ToString()))
                Marshal.ThrowExceptionForHR(Marshal.GetHRForLastWin32Error());
        }
        private void Form1_Resize(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == this.WindowState)
            {
                notifyIcon.Visible = true;
                notifyIcon.BalloonTipIcon = ToolTipIcon.Info;
                notifyIcon.BalloonTipText = "Teemo minimized ";
                notifyIcon.ShowBalloonTip(3000);
                this.Hide();
            }

            else if (FormWindowState.Normal == this.WindowState)
            {
                notifyIcon.Visible = false;
            }
        }

        private void notifyIcon_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            this.Show();
            this.WindowState = FormWindowState.Normal;
        }
        private const int CP_NOCLOSE_BUTTON = 0x200;
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams myCp = base.CreateParams;
                myCp.ClassStyle = myCp.ClassStyle | CP_NOCLOSE_BUTTON;
                return myCp;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var driveInfo = System.IO.DriveInfo.GetDrives();
            foreach (var item in listBox1.SelectedItems)
            {
                RemoveDriveLetter(item.ToString());
                /*
                if (item.ToString().Contains("C:"))
                {
                    hideDriveInRegistry("4");
                }
                else if (item.ToString().Contains("D:"))
                {
                    hideDriveInRegistry("8");
                }
                else if (item.ToString().Contains("E:"))
                {
                    hideDriveInRegistry("10");
                }
                else if (item.ToString().Contains("F:"))
                {
                    hideDriveInRegistry("20");
                }
                else if (item.ToString().Contains("G:"))
                {
                    hideDriveInRegistry("40");
                }
                else if (item.ToString().Contains("H:"))
                {
                    hideDriveInRegistry("80");
                }
                else if (item.ToString().Contains("I:"))
                {
                    hideDriveInRegistry("100");
                }
                else if (item.ToString().Contains("J:"))
                {
                    hideDriveInRegistry("200");
                }
                else if (item.ToString().Contains("K:"))
                {
                    hideDriveInRegistry("400");
                }
                else if (item.ToString().Contains("L:"))
                {
                    hideDriveInRegistry("800");
                }
                else if (item.ToString().Contains("M:"))
                {
                    hideDriveInRegistry("1000");
                }
                else if (item.ToString().Contains("N:"))
                {
                    hideDriveInRegistry("2000");
                }
                else if (item.ToString().Contains("O:"))
                {
                    hideDriveInRegistry("4000");
                }
                else if (item.ToString().Contains("P:"))
                {
                    hideDriveInRegistry("8000");
                }
                else if (item.ToString().Contains("Q:"))
                {
                    hideDriveInRegistry("10000");
                }
                else if (item.ToString().Contains("R:"))
                {
                    hideDriveInRegistry("20000");
                }
                else if (item.ToString().Contains("S:"))
                {
                    hideDriveInRegistry("40000");
                }
                else if (item.ToString().Contains("T:"))
                {
                    hideDriveInRegistry("80000");
                }
                else if (item.ToString().Contains("U:"))
                {
                    hideDriveInRegistry("100000");
                }
                else if (item.ToString().Contains("V:"))
                {
                    hideDriveInRegistry("200000");
                }
                else if (item.ToString().Contains("W:"))
                {
                    hideDriveInRegistry("400000");
                }
                else if (item.ToString().Contains("X:"))
                {
                    hideDriveInRegistry("800000");
                }
                else if (item.ToString().Contains("Y:"))
                {
                    hideDriveInRegistry("1000000");
                }
                else if (item.ToString().Contains("Z:"))
                {
                    hideDriveInRegistry("2000000");
                }
                notifyIcon.BalloonTipIcon = ToolTipIcon.Info;
                notifyIcon.BalloonTipText = "Drive " + item.ToString() + " is hidden";
                notifyIcon.ShowBalloonTip(3000);
                */
            }
        }

        private void hideDriveInRegistry(string p)
        {
            //subkey nodrive in HKEY_CURRENT_USER
            string nodrive = "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer";
            RegistryKey rKey1 = Registry.CurrentUser.CreateSubKey(nodrive);

            //create DWORD 'NoDrives' with value 4. for hiding drive C
            rKey1.SetValue("NoDrives",
            Convert.ToInt32(p, 16), RegistryValueKind.DWord);
            rKey1.Close();

            /*
            //give a message
            MessageBox.Show("Drive " + comboBox1.Text +
            ":\\ successful concealed ", "Drive Conceal",
            MessageBoxButtons.OK, MessageBoxIcon.Information);
                    
            //add logs history
            listBox1.Items.Add("Drive " + comboBox1.Text + ":\\ Hidden");
            listBox1.Items.Add("On " + currentDay);
            listBox1.Items.Add("");
            */
            //need restart to take effect
            restart();
        }
        private void restart()
        {
            // menghentikan process explorer
            Process p = new Process();
            foreach (System.Diagnostics.Process exe in System.Diagnostics.Process.GetProcesses())
            {
                if (exe.ProcessName == "explorer")

                    exe.Kill();
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            foreach (var item in listBox1.SelectedItems)
            {
                showDriveInRegistry();
            }
        }

        private void showDriveInRegistry()
        {
            string nodrive = "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\";
            RegistryKey rKey1 = Registry.CurrentUser.CreateSubKey(nodrive);
            rKey1.SetValue("NoDrives",0, RegistryValueKind.DWord);
            rKey1.Close();
            restart();
        }
    }
}
