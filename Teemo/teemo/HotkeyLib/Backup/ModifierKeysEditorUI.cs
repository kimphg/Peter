using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace FireAnt.Windows.Forms.Util
{
    internal partial class ModifierKeysEditorUI : UserControl
    {
        private Modifiers _value;
        private CheckBox[] selectedModifiers;

        public ModifierKeysEditorUI ()
        {
            InitializeComponent ();
            this.selectedModifiers = new CheckBox[] { uxAlt, uxControl, uxShift, uxWin };
            uxAlt.Tag = Modifiers.Alt;
            uxControl.Tag = Modifiers.Control;
            uxShift.Tag = Modifiers.Shift;
            uxWin.Tag = Modifiers.Win;
        }

        public void Start ( IWindowsFormsEditorService editor, object value )
        {
            this.Editor = editor;
            this.Value = (Modifiers)value;

            Modifiers modifiers = GetSelectedModifiers ();
            foreach ( CheckBox checkBox in selectedModifiers )
            {
                Modifiers modifier = (Modifiers)checkBox.Tag;
                if ( ( modifiers & modifier ) != Modifiers.None )
                    checkBox.Checked = true;
            }

        }

        public void End ()
        {
            this.Editor = null;
            this.Value = Modifiers.None;
        }

        private Modifiers GetSelectedModifiers ()
        {
            if ( Value == Modifiers.None )
                return Modifiers.Win;

            return ( Value & Modifiers.All );
        }

        private IWindowsFormsEditorService Editor { get; set; }
        public Modifiers Value
        {
            get
            {
                Modifiers returnValue = Modifiers.None;

                foreach ( CheckBox cb in selectedModifiers )
                {
                    if ( cb.Checked )
                        returnValue |= (Modifiers)cb.Tag;
                }

                return returnValue;
            }
            private set
            {
                _value = value;
            }
        }
    }
}
