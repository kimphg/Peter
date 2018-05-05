using System;
using System.Runtime.InteropServices;

namespace FireAnt.Windows.Forms.Util
{
    internal static class NativeMethods
    {
        private const string KERNEL32 = "kernel32";
        private const string USER32 = "user32";

        public enum ShowWindowCommand
        {
            SW_HIDE = 0,
            SW_SHOWNORMAL = 1,
            SW_NORMAL = 1,
            SW_SHOWMINIMIZED = 2,
            SW_SHOWMAXIMIZED = 3,
            SW_MAXIMIZE = 3,
            SW_SHOWNOACTIVATE = 4,
            SW_SHOW = 5,
            SW_SHOWNA = 8,
            SW_RESTORE = 9,
            SW_SHOWDEFAULT = 10,
            SW_FORCEMIMIMZE = 11,
            SW_MAX = 11
        }

        [DllImport ( KERNEL32, CharSet = CharSet.Auto, SetLastError = true )]
        public static extern short GlobalAddAtom ( string lpString );

        [DllImport ( KERNEL32 )]
        public static extern short GlobalDeleteAtom ( short nAtom );

        [DllImport ( USER32, SetLastError = true )]
        public static extern bool RegisterHotKey ( IntPtr hWnd, int id, int fsModifiers, int vk );

        [DllImport ( USER32, SetLastError = true )]
        public static extern bool UnregisterHotKey ( IntPtr hWnd, int id );

        [DllImport ( USER32 )]
        public static extern bool AttachThreadInput ( IntPtr idAttach, IntPtr idAttachTo, bool fAttach );

        [DllImport ( USER32 )]
        public static extern bool SetForegroundWindow ( IntPtr hWnd );

        [DllImport ( USER32 )]
        public static extern bool IsIconic ( IntPtr hWnd );

        [DllImport ( USER32 )]
        public static extern IntPtr GetForegroundWindow ();

        [DllImport ( USER32 )]
        public static extern IntPtr GetWindowThreadProcessId ( IntPtr hWnd, IntPtr lpdwProcessId );

        [DllImport ( USER32 )]
        public static extern bool ShowWindow ( IntPtr hWnd, ShowWindowCommand nCmdShow );
    }
}
