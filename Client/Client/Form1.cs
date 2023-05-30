﻿using ePOSOne.btnProduct;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace Client
{
    public partial class Form1 : Form
    {
        Queue<RoomData> rooms;
        public Form1()
        {
            InitializeComponent();

            //Making all the controls in the Form to be invisible 
            //except for the opening panel(tab)
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

        /// <summary>
        /// The functon moves from one panel to another.
        /// </summary>
        /// <param name="preTab"> The last tab to move from</param>
        /// <param name="nextTab"> The next tab to move to</param>
        private void MoveTab(Panel preTab, Panel nextTab)
        {
            preTab.Visible = false;
            nextTab.Visible = true;
        }

        private void button_WOC4_Click(object sender, EventArgs e)
        {
            string username = this.textBox4.Text, password = this.textBox5.Text, email = this.textBox7.Text;
            string street = this.textBox16.Text, city = this.textBox17.Text;
            int apartment = int.Parse(this.numericUpDown1.Value.ToString());
            string date = this.monthCalendar1.SelectionRange.Start.ToShortDateString();
            string phoneNum = this.comboBox1.Text + "-" + this.textBox11.Text;
            string address = street + "," + apartment + "," + city;
            SignupRequest signupReq = new SignupRequest(username, password, email, address, phoneNum, date);
            SignUpResponse signupResp = null;

            try
            {
                signupResp = SendRequestToServer<SignupRequest, SignUpResponse>(signupReq, REQUEST_CODES.SIGN_UP_REQS_CODE);
            }
            catch (Exception excp)
            {
                ShowErrorMessage(excp.Message, "Error Details");
            }

            if(signupResp != null) //did not have an exception
            {
                if (signupResp.GetStatus() != Constants.OK_STATUS_CODE)
                {
                    ShowErrorMessage("Sign up failed", "Sign Up Error");
                }
                else
                {
                    MoveTab(signUpPanel, menuPanel);
                }
            }

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
            string username = this.textBox20.Text, password = this.textBox18.Text;
            LoginRequest logReq = new LoginRequest(username, password);
            LoginResponse logResp = null;

            try
            {
                logResp = SendRequestToServer<LoginRequest, LoginResponse>(logReq, REQUEST_CODES.LOGIN_REQS_CODE);
            }
            catch(Exception excp)
            {
                ShowErrorMessage(excp.Message, "Error");
            }

            if(logResp != null)
            {
                if (logResp.GetStatus() != Constants.OK_STATUS_CODE)
                {
                    ShowErrorMessage("Login failed, there is no such a user", "Login Error");
                }
                else
                {
                    MoveTab(loginPanel, menuPanel);
                }
            }

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
            const string TITLE_ERROR = "Error Getting High Scores";
            GetHighScoreResponse highScoreResponse = null;
            Queue<string> highScores = null;

            try
            {
                highScoreResponse = SendRequestToServer<NullableConverter, GetHighScoreResponse>(null, REQUEST_CODES.GET_HIGH_SCORE_REQS_CODE);
            }
            catch(Exception excp)
            {
                ShowErrorMessage(excp.Message, TITLE_ERROR);
            }

            if(highScoreResponse != null)
            {
                highScores = highScoreResponse.GetStatistics();
                textBox39.Text = highScores.Dequeue();
                textBox41.Text = highScores.Dequeue();
                textBox43.Text = highScores.Dequeue();
            }

            MoveTab(statisticsPanel, bestPlayersPanel);
        }

        private void button_WOC12_Click(object sender, EventArgs e)
        {
            const string TITLE_ERROR = "Error Getting Statistics";
            GetPersonalStatsResponse getPersonalStatsResponse = null;
            Queue<string> statistics = null;

            try
            {
                getPersonalStatsResponse = SendRequestToServer<NullableConverter, GetPersonalStatsResponse>(null, REQUEST_CODES.GET_PERS_STATS_REQS_CODE);
            }
            catch(Exception excp)
            {
                ShowErrorMessage(excp.Message, TITLE_ERROR);
            }

            if (getPersonalStatsResponse != null)
            {
                //Getting the statistics and showing them on the screen
                statistics = getPersonalStatsResponse.GetStatistics();
                statistics.Dequeue();
                textBox31.Text = statistics.Dequeue();
                textBox33.Text = statistics.Dequeue();
                textBox32.Text = statistics.Dequeue();
                textBox30.Text = statistics.Dequeue();
                MoveTab(statisticsPanel, personalStatsPanel);
            }
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
            Queue<RoomData> queueRooms = null;
            UpdateListRooms();

            //Inserting all the rooms in the combo box
            queueRooms = new Queue<RoomData>(this.rooms);
            comboBox2.Text = "";
            while(queueRooms.Count > 0)
            {
                comboBox2.Items.Add(queueRooms.Dequeue().GetName() + Constants.NEW_LINE);
            }
            MoveTab(menuPanel, enterRoomPanel);
        }

        private static U SendRequestToServer<T, U>(T request, int codeReq)
        {
            //Making the request enable to pass to the Server
            string reqMsg = JsonRequestPacketSerializer.SerializeRequest<T>(request, codeReq);
            string FILE_PATH = "serialized.bin";
            using (FileStream fs = new FileStream(FILE_PATH, FileMode.Create))
            {
                BinaryFormatter formatter = new BinaryFormatter();
                formatter.Serialize(fs, reqMsg);
            }

            //Sending the msg to server and getting an answer
            U var = JsonResponsePacketDeserializer.DeserializeResponse<U>("Buffer string");
            return var;

        }

        private void ShowErrorMessage(string message, string title)
        {
            MessageBox.Show(message, title, MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void UpdateListRooms()
        {
            GetRoomsResponse rooms = null; 

            try
            {
                rooms = SendRequestToServer<NullableConverter, GetRoomsResponse>(null, REQUEST_CODES.GET_ROOMS_REQS_CODE);
            }
            catch (Exception excp)
            {
                ShowErrorMessage(excp.Message, "Error");
            }

            if(rooms != null)
            {
                this.rooms = new Queue<RoomData>(rooms.GetRoomDatas());
            }

        }

        private int GetIdRoomByName(string name)
        {
            int id = Constants.ROOM_NOT_FOUND_ID;
            Queue<RoomData> roomDatas = new Queue<RoomData>(this.rooms);
            RoomData roomData = null;

            //Going over the rooms
            while (roomDatas.Count > 0)
            {
                roomData = roomDatas.Dequeue();
                if(roomData.GetName().Equals(name))
                {
                    id = roomData.GetId();
                }
            }
            return id;
        }

        private void button_WOC17_Click(object sender, EventArgs e)
        {
            const string TITLE_ERROR = "Error Entering Room";
            JoinRoomRequest joinRoomRequest = null;
            JoinRoomResponse joinRoomResponse = null;
            int roomId = GetIdRoomByName(comboBox2.Text);

            if(Constants.ROOM_NOT_FOUND_ID == roomId)
            {
                ShowErrorMessage("There is no room with this name", TITLE_ERROR);
            }
            else
            {
                joinRoomRequest = new JoinRoomRequest(roomId);

                try
                {
                    joinRoomResponse = SendRequestToServer<JoinRoomRequest, JoinRoomResponse>(joinRoomRequest, REQUEST_CODES.JOIN_ROOM_REQS_CODE);
                }
                catch(Exception excp)
                {
                    ShowErrorMessage(excp.Message, TITLE_ERROR);
                }

                if(joinRoomResponse != null)
                {
                    if(Constants.OK_STATUS_CODE != joinRoomResponse.GetStatus())
                    {
                        ShowErrorMessage("Error Entering Room", TITLE_ERROR);
                    }
                    else
                    {
                        MessageBox.Show("Good enterance"); //going to the room's page
                    }
                }
            }
        }

        private void button_WOC18_Click(object sender, EventArgs e)
        {
            const string TITLE_ERROR = "Error Creating Room";
            string roomName = textBox51.Text;
            int timePerQuestion = int.Parse(textBox52.Text);
            int maxUsersInRoom = int.Parse(numericUpDown2.Value.ToString());
            int amountQuestions = int.Parse(numericUpDown3.Value.ToString());
            CreateRoomRequest createRoomRequest = new CreateRoomRequest(roomName, maxUsersInRoom, amountQuestions, timePerQuestion);
            CreateRoomResponse createRoomResponse = null;

            try
            {
                createRoomResponse = SendRequestToServer<CreateRoomRequest, CreateRoomResponse>(createRoomRequest, REQUEST_CODES.CREATE_ROOM_REQS_CODE);
            }
            catch(Exception excp)
            {
                ShowErrorMessage(excp.Message, TITLE_ERROR);
            }

            if (createRoomResponse != null)
            {
                if (createRoomResponse.GetStatus() != Constants.OK_STATUS_CODE)
                {
                    ShowErrorMessage("Error creating room", TITLE_ERROR);
                }
                else
                {
                    MessageBox.Show("Room was created");
                }

            }
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            textBox52.Text = trackBar1.Value.ToString();
        }
    }
}
