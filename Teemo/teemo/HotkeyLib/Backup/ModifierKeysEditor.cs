using System;
using System.ComponentModel;
using System.Drawing.Design;
using System.Windows.Forms.Design;

namespace FireAnt.Windows.Forms.Util
{
    public class ModifierKeysEditor : UITypeEditor
    {
        private ModifierKeysEditorUI keysEditorUI;

        public override object EditValue ( ITypeDescriptorContext context, IServiceProvider provider, object value )
        {
            if ( provider != null )
            {
                IWindowsFormsEditorService eds = (IWindowsFormsEditorService)provider.GetService ( typeof ( IWindowsFormsEditorService ) );
                if ( eds == null )
                {
                    return value;
                }

                if ( keysEditorUI == null )
                {
                    this.keysEditorUI = new ModifierKeysEditorUI ();
                }
                this.keysEditorUI.Start ( eds, value );
                eds.DropDownControl ( keysEditorUI );
                value = this.keysEditorUI.Value;
                this.keysEditorUI.End ();
            }
            return base.EditValue ( context, provider, value );
        }

        public override UITypeEditorEditStyle GetEditStyle ( ITypeDescriptorContext context )
        {
            return UITypeEditorEditStyle.DropDown;
        }

     
    }
}
