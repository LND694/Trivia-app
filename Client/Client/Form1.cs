﻿using ePOSOne.btnProduct;
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

            for(int i = 0; i < this.Controls.Count; i++)
            {
                this.Controls[i].Visible = false;
            }
            this.openPanel.Visible = true;
        }

        private void button_WOC3_Click(object sender, EventArgs e)
        {
            MoveTab(this.signUpPanel, this.openPanel);
        }

        private void MoveTab(Panel preTab, Panel nextTab)
        {
            preTab.Visible = false;
            nextTab.Visible = true;
        }

        private void button_WOC4_Click(object sender, EventArgs e)
        {
            MoveTab(signUpPanel, menuPanel);
        }

        private void button_WOC11_Click_1(object sender, EventArgs e)
        {
            MoveTab(menuPanel, openPanel);
        }

        private void button_WOC1_Click_1(object sender, EventArgs e)
        {
            MoveTab(openPanel, signUpPanel);
        }

        private void button_WOC5_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button_WOC2_Click_1(object sender, EventArgs e)
        {
            MoveTab(openPanel, loginPanel);
        }

        private void button_WOC13_Click(object sender, EventArgs e)
        {
            MoveTab(statisticsPanel, menuPanel);
        }

        private void button_WOC10_Click_1(object sender, EventArgs e)
        {
            MoveTab(menuPanel, statisticsPanel);
        }

        private void button_WOC7_Click_1(object sender, EventArgs e)
        {
            MoveTab(loginPanel, openPanel);
        }

        private void button_WOC6_Click_1(object sender, EventArgs e)
        {
            MoveTab(loginPanel, menuPanel);
        }

        private void button_WOC16_Click(object sender, EventArgs e)
        {
            MoveTab(personalStatsPanel, statisticsPanel);
        }

        private void button_WOC15_Click(object sender, EventArgs e)
        {
            MoveTab(bestPlayersPanel, statisticsPanel);
        }

        private void button_WOC14_Click(object sender, EventArgs e)
        {
            MoveTab(statisticsPanel, bestPlayersPanel);
        }

        private void button_WOC12_Click(object sender, EventArgs e)
        {
            MoveTab(statisticsPanel, personalStatsPanel);

        }

        private void button_WOC20_Click(object sender, EventArgs e)
        {
            MoveTab(createRoomPanel, menuPanel);
        }

        private void button_WOC19_Click(object sender, EventArgs e)
        {
            MoveTab(enterRoomPanel, menuPanel);
        }

        private void button_WOC9_Click(object sender, EventArgs e)
        {
            MoveTab(menuPanel, createRoomPanel);
        }

        private void button_WOC8_Click(object sender, EventArgs e)
        {
            MoveTab(menuPanel, enterRoomPanel);
        }
    }
}
