using System;
using System.ComponentModel;
using System.Drawing.Design;

namespace FireAnt.Windows.Forms.Util
{
    [Flags, Editor ( typeof ( ModifierKeysEditor ), typeof ( UITypeEditor ) )]
    public enum Modifiers
    {
        None = 0,
        Alt = 1,
        Control = 2,
        Shift = 4,
        Win = 8,
        All = Alt | Control | Shift | Win
    }
}
