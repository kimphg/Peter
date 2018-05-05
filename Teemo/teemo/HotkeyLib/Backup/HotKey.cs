using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Drawing;

namespace FireAnt.Windows.Forms.Util
{
    [ToolboxBitmap(typeof(HotKey), "ToolboxImage.bmp")]
    public partial class HotKey : Component
    {

        /// <summary>
        /// Event fired when this instance receives a WM_HOTKEY message.
        /// </summary>
        public event EventHandler HotKeyPressed;

        public HotKey ()
        {
            InitializeComponent ();
            this.Modifiers = Modifiers.None;
            this.Key = Keys.None;
        }

        public HotKey ( IContainer container ) :
            this ()
        {
            container.Add ( this );
        }

        /// <summary>
        /// Get/Set the HotKeyNativeWindow instance used to capture WM_HOTKEY messages
        /// for this instance.
        /// </summary>
        private HotKeyNativeWindow HotKeyWindow { get; set; }

        /// <summary>
        /// Get/Set the modifier or modifers to use to activate this hotkey
        /// </summary>
        public Modifiers Modifiers { get; set; }

        /// <summary>
        /// Get/Set the virtual key code as the actual hotkey.
        /// </summary>
        /// <remarks>
        /// While it is possible to combine multiple keycodes together, it is likely that under
        /// many circumstances you will have an inoperative hotkey.  Also, you should not use any of the
        /// Keys modifier keys.
        /// </remarks>
        public Keys Key { get; set; }

        /// <summary>
        /// Register a hotkey combination of one or more Modifers and a Key.
        /// </summary>
        /// <remarks>
        /// This method uses the values of the Modifiers and Key properties.  If
        /// Modifiers == Modifiers.None and Key = Keys.None then any current hotkey combination
        /// represented by this instance is deactivated.  Calling this method a subsequent time with
        /// a different combination will cause the current combination to be replaced and the new hotkey
        /// combination installed.  If the hotkey registration process fails (for example the hotkey combination
        /// is already registered) then a Win32Exception is thrown.
        /// </remarks>
        public void RegisterHotkey ()
        {
            if ( this.HotKeyWindow == null )
                this.HotKeyWindow = new HotKeyNativeWindow ( this );

            try
            {
                if ( Modifiers != Modifiers.None && Key != Keys.None )
                {
                    this.HotKeyWindow.RegisterHotkey ();
                }
                else
                {
                    if ( HotKeyWindow.Handle != IntPtr.Zero )
                        HotKeyWindow.DestroyHandle ();
                    HotKeyWindow = null;
                }
            }
            catch
            {
                if ( HotKeyWindow.Handle != IntPtr.Zero )
                    HotKeyWindow.DestroyHandle ();
                this.HotKeyWindow = null;
                throw;
            }
        }

        /// <summary>
        /// Register a hotkey combination of one or more Modifers and a Key.
        /// </summary>
        /// <param name="modifiers">The modifier or modifiers to use to activate the hotkey</param>
        /// <param name="key">The actual hotkey value</param>
        /// <remarks>
        /// This method uses the values of the modifers and key parameters to set the Modifiers and Key properties, and
        /// then delegates to the RegisterHotkey() function.  See remarks for that function to understand the behavior of 
        /// this method.
        /// </remarks>
        public void RegisterHotkey ( Modifiers modifiers, Keys key )
        {
            this.Modifiers = modifiers;
            this.Key = key;
            this.RegisterHotkey ();
        }


        /// <summary>
        /// Utility method to force a window to the foreground. This code is a C# translation
        /// of Karl Peterson's VB6 ForceForegroundWindow function in his ForceFore example: http://vb.mvps.org/samples/ForceFore/
        /// </summary>
        /// <param name="hWnd">The handle of the window to force to the front.</param>
        public void ActivateWindow ( IntPtr hWnd )
        {
            IntPtr hForeground = NativeMethods.GetForegroundWindow ();
            if ( hWnd != hForeground )
            {
                IntPtr hForegroundThread = NativeMethods.GetWindowThreadProcessId ( hForeground, IntPtr.Zero );
                IntPtr hCurrentThread = NativeMethods.GetWindowThreadProcessId ( hWnd, IntPtr.Zero );

                if ( hForegroundThread != hCurrentThread )
                {
                    NativeMethods.AttachThreadInput ( hForegroundThread, hCurrentThread, true );
                    NativeMethods.SetForegroundWindow ( hWnd );
                    NativeMethods.AttachThreadInput ( hForegroundThread, hCurrentThread, false );
                }
                else
                {
                    NativeMethods.SetForegroundWindow ( hWnd );
                }

                if ( NativeMethods.IsIconic ( hWnd ) )
                {
                    NativeMethods.ShowWindow ( hWnd, NativeMethods.ShowWindowCommand.SW_RESTORE );
                }
                else
                {
                    NativeMethods.ShowWindow ( hWnd, NativeMethods.ShowWindowCommand.SW_SHOW );
                }
            }
        }

        /// <summary>
        /// Raises the HotkeyPressed event on WM_HOTKEY notification
        /// </summary>
        protected virtual void OnHotKey ()
        {
            if ( this.HotKeyPressed != null )
                this.HotKeyPressed ( this, new EventArgs () );
        }

        /// <summary>
        /// Handles the capturing of the WM_HOTKEY messages as well as registering/unregistering
        /// the hotkey via Win32 API.
        /// </summary>
        private class HotKeyNativeWindow : NativeWindow
        {
            private static IntPtr HWND_MESSAGE = new IntPtr ( -3 );
            private const int WM_HOTKEY = 0x312;

            public HotKeyNativeWindow ( HotKey owner )
            {
                this.Owner = owner;
            }

            ~HotKeyNativeWindow ()
            {
                try
                {
                    this.UnregisterHotkey ();
                }
                catch
                {
                }
            }
            private HotKey Owner { get; set; }
            private short HotKeyID { get; set; }

            public override void DestroyHandle ()
            {
                UnregisterHotkey ();
                base.DestroyHandle ();
            }

            public override void ReleaseHandle ()
            {
                UnregisterHotkey ();
                base.ReleaseHandle ();
            }

            public void RegisterHotkey ()
            {
                if ( this.HandleCreated () && Owner.Modifiers != Modifiers.None && Owner.Key != Keys.None )
                {
                    if ( this.HotKeyID == 0 )
                        this.HotKeyID = NativeMethods.GlobalAddAtom ( Guid.NewGuid ().ToString ( "N" ) );

                    if ( !NativeMethods.RegisterHotKey ( this.Handle, this.HotKeyID, (int)Owner.Modifiers, (int)Owner.Key ) )
                    {
                        throw new Win32Exception ( Marshal.GetLastWin32Error () );
                    }
                }
            }

            public void UnregisterHotkey ()
            {
                if ( base.Handle != IntPtr.Zero && this.HotKeyID != 0 )
                {
                    NativeMethods.UnregisterHotKey ( this.Handle, this.HotKeyID );
                    NativeMethods.GlobalDeleteAtom ( this.HotKeyID );
                    this.HotKeyID = 0;
                }
            }

            private bool HandleCreated ()
            {
                if ( base.Handle == IntPtr.Zero )
                {
                    CreateParams createParams = new CreateParams ();
                    createParams.Caption = Guid.NewGuid ().ToString ( "N" );
                    createParams.Style = 0;
                    createParams.ExStyle = 0;
                    createParams.ClassStyle = 0;
                    if ( Environment.OSVersion.Platform == PlatformID.Win32NT )
                    {
                        createParams.Parent = HWND_MESSAGE;
                    }
                    this.CreateHandle ( createParams );
                }
                return ( base.Handle != IntPtr.Zero );
            }


            protected override void WndProc ( ref Message m )
            {
                // handle a hotkey event..
                if ( m.Msg == WM_HOTKEY )
                {
                    this.Owner.OnHotKey ();
                }

                base.WndProc ( ref m );
            }
        }
    }


}
