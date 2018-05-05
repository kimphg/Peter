namespace FireAnt.Windows.Forms.Util
{
    partial class ModifierKeysEditorUI
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose ( bool disposing )
        {
            if ( disposing && ( components != null ) )
            {
                components.Dispose ();
            }
            base.Dispose ( disposing );
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent ()
        {
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel ();
            this.label1 = new System.Windows.Forms.Label ();
            this.uxAlt = new System.Windows.Forms.CheckBox ();
            this.uxControl = new System.Windows.Forms.CheckBox ();
            this.uxShift = new System.Windows.Forms.CheckBox ();
            this.uxWin = new System.Windows.Forms.CheckBox ();
            this.tableLayoutPanel1.SuspendLayout ();
            this.SuspendLayout ();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanel1.ColumnCount = 4;
            this.tableLayoutPanel1.ColumnStyles.Add ( new System.Windows.Forms.ColumnStyle () );
            this.tableLayoutPanel1.ColumnStyles.Add ( new System.Windows.Forms.ColumnStyle () );
            this.tableLayoutPanel1.ColumnStyles.Add ( new System.Windows.Forms.ColumnStyle () );
            this.tableLayoutPanel1.ColumnStyles.Add ( new System.Windows.Forms.ColumnStyle () );
            this.tableLayoutPanel1.Controls.Add ( this.label1, 0, 0 );
            this.tableLayoutPanel1.Controls.Add ( this.uxAlt, 0, 1 );
            this.tableLayoutPanel1.Controls.Add ( this.uxControl, 1, 1 );
            this.tableLayoutPanel1.Controls.Add ( this.uxShift, 2, 1 );
            this.tableLayoutPanel1.Controls.Add ( this.uxWin, 3, 1 );
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point ( 0, 0 );
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add ( new System.Windows.Forms.RowStyle () );
            this.tableLayoutPanel1.RowStyles.Add ( new System.Windows.Forms.RowStyle ( System.Windows.Forms.SizeType.Percent, 100F ) );
            this.tableLayoutPanel1.Size = new System.Drawing.Size ( 272, 48 );
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.Anchor = ( (System.Windows.Forms.AnchorStyles)( ( System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right ) ) );
            this.label1.AutoSize = true;
            this.tableLayoutPanel1.SetColumnSpan ( this.label1, 4 );
            this.label1.Location = new System.Drawing.Point ( 3, 0 );
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size ( 266, 13 );
            this.label1.TabIndex = 0;
            this.label1.Text = "Modifiers: ";
            // 
            // uxAlt
            // 
            this.uxAlt.Anchor = ( (System.Windows.Forms.AnchorStyles)( ( ( System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom )
                        | System.Windows.Forms.AnchorStyles.Left ) ) );
            this.uxAlt.AutoSize = true;
            this.uxAlt.Location = new System.Drawing.Point ( 3, 16 );
            this.uxAlt.Name = "uxAlt";
            this.uxAlt.Size = new System.Drawing.Size ( 38, 29 );
            this.uxAlt.TabIndex = 1;
            this.uxAlt.Text = "Alt";
            this.uxAlt.UseVisualStyleBackColor = true;
            // 
            // uxControl
            // 
            this.uxControl.Anchor = ( (System.Windows.Forms.AnchorStyles)( ( ( System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom )
                        | System.Windows.Forms.AnchorStyles.Left ) ) );
            this.uxControl.AutoSize = true;
            this.uxControl.Location = new System.Drawing.Point ( 47, 16 );
            this.uxControl.Name = "uxControl";
            this.uxControl.Size = new System.Drawing.Size ( 41, 29 );
            this.uxControl.TabIndex = 2;
            this.uxControl.Text = "Ctrl";
            this.uxControl.UseVisualStyleBackColor = true;
            // 
            // uxShift
            // 
            this.uxShift.Anchor = ( (System.Windows.Forms.AnchorStyles)( ( ( System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom )
                        | System.Windows.Forms.AnchorStyles.Left ) ) );
            this.uxShift.AutoSize = true;
            this.uxShift.Location = new System.Drawing.Point ( 94, 16 );
            this.uxShift.Name = "uxShift";
            this.uxShift.Size = new System.Drawing.Size ( 47, 29 );
            this.uxShift.TabIndex = 3;
            this.uxShift.Text = "Shift";
            this.uxShift.UseVisualStyleBackColor = true;
            // 
            // uxWin
            // 
            this.uxWin.Anchor = ( (System.Windows.Forms.AnchorStyles)( ( ( System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom )
                        | System.Windows.Forms.AnchorStyles.Left ) ) );
            this.uxWin.AutoSize = true;
            this.uxWin.Location = new System.Drawing.Point ( 147, 16 );
            this.uxWin.Name = "uxWin";
            this.uxWin.Size = new System.Drawing.Size ( 45, 29 );
            this.uxWin.TabIndex = 4;
            this.uxWin.Text = "Win";
            this.uxWin.UseVisualStyleBackColor = true;
            // 
            // ModifierKeysEditorUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF ( 6F, 13F );
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add ( this.tableLayoutPanel1 );
            this.Name = "ModifierKeysEditorUI";
            this.Size = new System.Drawing.Size ( 272, 48 );
            this.tableLayoutPanel1.ResumeLayout ( false );
            this.tableLayoutPanel1.PerformLayout ();
            this.ResumeLayout ( false );

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.CheckBox uxWin;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox uxAlt;
        private System.Windows.Forms.CheckBox uxControl;
        private System.Windows.Forms.CheckBox uxShift;

    }
}
