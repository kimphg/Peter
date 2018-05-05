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
        ContextMenu contextMenu1 = new System.Windows.Forms.ContextMenu();
        public Form1()
        {

            InitializeComponent();
            //this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Resize += Form1_Resize;
            listBox1.SelectionMode = SelectionMode.MultiSimple;
            
            UpdateDriveLetters();
            notifyIcon.ContextMenu = contextMenu1;
            //.contextMenu1.contextMenu1.
            //var volumes = GetMountPoints("C:");
            
        }

        private void UpdateDriveLetters()
        {
            listBox2.Items.Clear();
            listBox1.Items.Clear();
            contextMenu1.MenuItems.Clear();
            var driveInfo = System.IO.DriveInfo.GetDrives();
            foreach (var drive in driveInfo)
            {
                //string str = drive.ToString();
                //contextMenu1
                // Initialize menuItem1
                var menuItem = new System.Windows.Forms.MenuItem();
                menuItem.Index = 0;
                menuItem.Text = drive.ToString() ;
                if (drive.IsReady) menuItem.Text += " " + drive.VolumeLabel + " " + drive.TotalSize/1000000+"MB";
                menuItem.Click += new System.EventHandler(this.menuItem_Click);
                contextMenu1.MenuItems.Add(menuItem);
                listBox2.Items.Add(drive);
            }
            var volumes = GetVolumes();
            foreach (var volume in volumes)
            {
                //string str = drive.ToString();

                listBox1.Items.Add(volume);
            }
        }

        private void menuItem_Click(object sender, EventArgs e)
        {
            MenuItem item = sender as MenuItem;
            string[] list = item.Text.Split(' ');
            string letter = list[0];
            RemoveDriveLetter(letter);
        }

        [DllImport("kernel32.dll", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        public static extern int GetVolumePathName(
            string lpszFileName,
          StringBuilder lpszVolumePathName,
          int cchBufferLength);
        ////
        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        static extern bool GetVolumeInformation(string Volume, StringBuilder VolumeName,
            uint VolumeNameSize, out uint SerialNumber, out uint SerialNumberLength,
            out uint flags, StringBuilder fs, uint fs_size);

        [DllImport("kernel32.dll", EntryPoint = "FindFirstVolumeMountPoint", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        public static extern int FindFirstVolumeMountPoint(
          StringBuilder lpszRootPathName,
          StringBuilder lpszVolumeMountPoint,
          int cchBufferLength);
        [DllImport("kernel32.dll", EntryPoint = "FindFirstVolume", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        public static extern int FindFirstVolume(
          StringBuilder lpszVolumeName,
          int cchBufferLength);

        [DllImport("kernel32.dll", EntryPoint = "FindNextVolume", SetLastError = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool FindNextVolume(
          int hFindVolume,
          StringBuilder lpszVolumeName,
          int cchBufferLength);

        [DllImport("kernel32.dll", SetLastError = false)]
        static extern bool GetVolumeNameForVolumeMountPoint(string
            lpszVolumeMountPoint, [Out] StringBuilder lpszVolumeName,
            uint cchBufferLength);

        [DllImport("kernel32.dll")]
        static extern bool DeleteVolumeMountPoint(string lpszVolumeMountPoint);

        [DllImport("kernel32.dll")]
        static extern bool SetVolumeMountPoint(string lpszVolumeMountPoint,
            string lpszVolumeName);
        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]

 

        private static extern IntPtr FindFirstVolumeMountPointW(string lpszRootPathName, StringBuilder lpszVolumeMountPoint, Int32 cchBufferLength);


        
        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]

        private static extern IntPtr FindVolumeMountPointClose(IntPtr handle);


        const int MAX_PATH = 260;
        public static List<string> GetVolumes()
        {

            const int N = 1024;
            StringBuilder cVolumeName = new StringBuilder((int)N);
            List<string> ret = new List<string>();
            int volume_handle = FindFirstVolume(cVolumeName, N);
            do
            {
                string volumeName1 = cVolumeName.ToString();
                uint serialNum, serialNumLength, flags;
                StringBuilder volumeLabel = new StringBuilder(256);
                StringBuilder fstype = new StringBuilder(256);
                
                bool ok = GetVolumeInformation(volumeName1, volumeLabel, 
	                (uint)volumeLabel.Capacity - 1, out serialNum, out serialNumLength, 
	                out flags, fstype, (uint)fstype.Capacity - 1);
                
                ret.Add(getVolumeLetter(volumeName1)+" "+ volumeLabel.ToString() + " " + volumeName1);
                //Console.WriteLine(cVolumeName.ToString());
            } while (FindNextVolume(volume_handle, cVolumeName, N));
            return ret;
        }

        private static string getVolumeLetter(string volume)
        {
            char letter = 'C';
            while (letter != 'Z')
            {
                string letterstring = letter.ToString() + ":\\";
                StringBuilder volumeName = new StringBuilder(MAX_PATH);
                GetVolumeNameForVolumeMountPoint(letterstring, volumeName, (uint)MAX_PATH);
                if (volumeName.ToString().Contains(volume))
                {
                    return letterstring;
                }
                letter++;
            }
            return "";
        }
        private void RemoveDriveLetter(string letter)
        {
            StringBuilder volume = new StringBuilder(MAX_PATH);
            if (!GetVolumeNameForVolumeMountPoint(letter, volume, (uint)MAX_PATH))
                Marshal.ThrowExceptionForHR(Marshal.GetHRForLastWin32Error());

            if (!DeleteVolumeMountPoint(letter))
                Marshal.ThrowExceptionForHR(Marshal.GetHRForLastWin32Error());
            /*
            */
        }
        private void RemoveVolume(string volume)
        {
            //StringBuilder volume = new StringBuilder(MAX_PATH);
            char letter = 'D';
            while(letter!='Z')
            {
                string letterstring = letter.ToString() + ":\\";
                StringBuilder volumeName = new StringBuilder(MAX_PATH);
                GetVolumeNameForVolumeMountPoint(letterstring, volumeName, (uint)MAX_PATH);
                if (volumeName.ToString().Contains(volume))
                {
                    if (!DeleteVolumeMountPoint(letterstring))
                        Marshal.ThrowExceptionForHR(Marshal.GetHRForLastWin32Error());
                    return;
                }
                letter++;
            }
            
            /*
            */
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
        /*private const int CP_NOCLOSE_BUTTON = 0x200;
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams myCp = base.CreateParams;
                myCp.ClassStyle = myCp.ClassStyle | CP_NOCLOSE_BUTTON;
                return myCp;
            }
        }*/

        private void button1_Click(object sender, EventArgs e)
        {
            var driveInfo = System.IO.DriveInfo.GetDrives();
            foreach (var item in listBox1.SelectedItems)
            {
                //RemoveDriveLetter(item.ToString());
                var volumeNames = item.ToString().Split(' ');
                var volumeName = volumeNames[volumeNames.Length-1];
                RemoveVolume(volumeName);
                
            }
            UpdateDriveLetters();
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
            var driveInfo = System.IO.DriveInfo.GetDrives();
            foreach (var item in listBox1.SelectedItems)
            {
                //RemoveDriveLetter(item.ToString());
                var volumeNames = item.ToString().Split(' ');
                var volumeName = volumeNames[volumeNames.Length - 1];
                MountVolume(volumeName);

            }
            UpdateDriveLetters();
        }

        private void MountVolume(string volumeName)
        {
            //StringBuilder volume = new StringBuilder(MAX_PATH);
            char letter = 'D';
            while(letter!='Z')
            {
                foreach (var item in listBox2.Items)
                {
                    string existingLetter = item.ToString();
                    if (existingLetter.Contains(letter.ToString())) continue;
                }
                SetVolumeMountPoint(letter.ToString()+":\\", volumeName);
                letter++;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Minimized;
        }

    }
}
