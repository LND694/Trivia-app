using ePOSOne.btnProduct;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace Client
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button_WOC1_Click(object sender, EventArgs e)
        {

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void button_WOC2_Click(object sender, EventArgs e)
        {

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void button_WOC3_Click(object sender, EventArgs e)
        {
            MoveTab(this.signUpPanel, this.openPanel);
        }

        private void MoveTab(Panel preTab, Panel nextTab)
        {
            //Hiding all the controls of the previous tab
            for (int i = 0; i < preTab.Controls.Count; i++)
            {
                preTab.Controls[i].Visible = false;
            }

            //Showing all the controls of next tab
            for (int i = 0; i < nextTab.Controls.Count; i++)
            {
                nextTab.Controls[i].Visible = true;
            }
            nextTab.BringToFront();
        }

        private void openPanel_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
