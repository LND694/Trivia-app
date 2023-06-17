using ePOSOne.btnProduct;
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
using System.Threading;


namespace Client
{
    public partial class Form1 : Form
    {
        //The fields of the Form.
        private readonly Communicator communicator;
        private Queue<RoomData> rooms;
        private Queue<string> highScores;
        private GetRoomStateResponse roomData;
        private readonly Mutex roomDataLock;
        private readonly Mutex roomsLock;
        private readonly Mutex highScoresLock;
        private int seconds;
        private int questionsLeft;
        private bool wasClicked = false;
        private bool isRight = false;
        public Form1()
        {
            Thread autoUpdateThread = new Thread(new ThreadStart(AutoUpdate));
            InitializeComponent();
            this.communicator = new Communicator();
            communicator.Connect();

            //Initializing the Locks(Mutexes)
            this.roomDataLock = new Mutex(false);
            this.roomsLock = new Mutex(false);
            this.highScoresLock = new Mutex(false);


            //Making all the controls in the Form to be invisible 
            //except for the opening panel(tab)
            for (int i = 0; i < this.Controls.Count; i++)
            {
                this.Controls[i].Visible = false;
            }
            this.openPanel.Visible = true;

            //Activating the AutoUpdate thread
            autoUpdateThread.IsBackground = true;
            autoUpdateThread.Start();
        }

        /// <summary>
        /// The function updates automatically
        /// the data of the current panel.
        /// </summary>
        private void AutoUpdate()
        {
            while(!this.IsDisposed)
            {
                if (this.bestPlayersPanel.Visible)
                {
                    if (UpdateHighScores())
                    {
                        PresentHighScores();
                    }
                }
                else if (this.enterRoomPanel.Visible)
                {
                    if (UpdateListRooms())
                    {
                        PresentListRooms();
                    }
                }
                else if (this.roomAdminPanel.Visible)
                {
                    if (UpdateRoomState())
                    {
                        PresentRoomStateAdmin(this.textBox72.Text);
                    }
                }
                else if (this.roomMemberPanel.Visible)
                {
                    if (UpdateRoomState())
                    {
                        PresentRoomStateMember(this.textBox57.Text);
                    }
                    else
                    {
                        button_WOC21_Click(null, null); // leave room
                        ShowErrorMessage("The host left the room", "Leaving room");
                    }
                }
                
                System.Threading.Thread.Sleep(3000);
            }

        }

        /// <summary>
        /// Back to the Open panel from the 
        /// Sign up panel
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
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
            if (preTab.InvokeRequired)
            {
                preTab.Invoke((MethodInvoker)(() => preTab.Visible = false));
            }
            else
            {
                preTab.Visible = false;
            }

            if (nextTab.InvokeRequired)
            {
                nextTab.Invoke((MethodInvoker)(() => nextTab.Visible = true));
            }
            else
            {
                nextTab.Visible = true;
            }
        }

        /// <summary>
        /// Signing the new user and if the new user
        /// is legal- moving to the Menu panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC4_Click(object sender, EventArgs e)
        {
            string username = this.textBox4.Text, password = this.textBox5.Text, email = this.textBox7.Text;
            string street = this.textBox16.Text, city = this.textBox17.Text;
            int apartment = int.Parse(this.numericUpDown1.Value.ToString());
            string date = this.monthCalendar1.SelectionRange.Start.ToShortDateString();
            string phoneNum = this.comboBox1.Text + "-" + this.textBox11.Text;
            string address = street + "," + apartment + "," + city;
            SignUpResponse signupResp = null;
            SignupRequest signupReq = null;

            //The day of the date does not have 2 digits
            if (date.Length < Constants.DATE_LENGTH)
            {
                date = Constants.ZERO_CHAR + date;
            }

            signupReq = new SignupRequest(username, password, email, address, phoneNum, date);

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

        /// <summary>
        /// Log out button. If the log out was successful-it will move to
        /// the Open panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC11_Click_1(object sender, EventArgs e)
        {
            const string TITLE_ERROR = "Log Out Error";
            LogoutResponse logoutResponse = null;

            try
            {
                logoutResponse = SendRequestToServer<NullableConverter, LogoutResponse>(null, REQUEST_CODES.LOGOUT_REQS_CODE);
            }
            catch(Exception excp)
            {
                ShowErrorMessage(excp.Message, TITLE_ERROR);
            }

            if(logoutResponse!= null) //there was no exception
            {
                if(logoutResponse.GetStatus() != Constants.OK_STATUS_CODE)
                {
                    ShowErrorMessage("Error login out", TITLE_ERROR);
                }
                else
                {
                    MoveTab(menuPanel, openPanel);
                }
            }

        }

        /// <summary>
        /// Moving to the Sign up panel
        /// from the Open panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC1_Click_1(object sender, EventArgs e)
        {
            MoveTab(openPanel, signUpPanel);
        }

        /// <summary>
        /// Exiting the GUI.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC5_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// Moving from the Open panel 
        /// to the Login panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC2_Click_1(object sender, EventArgs e)
        {
            MoveTab(openPanel, loginPanel);
        }

        /// <summary>
        /// Moving from the Statistics panel to the Menu panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC13_Click(object sender, EventArgs e)
        {
            MoveTab(statisticsPanel, menuPanel);
        }

        /// <summary>
        /// Moving from the Menu panel to the Statistics panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC10_Click_1(object sender, EventArgs e)
        {
            MoveTab(menuPanel, statisticsPanel);
        }

        /// <summary>
        /// Moving from the Login panel back to the Open panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC7_Click_1(object sender, EventArgs e)
        {
            MoveTab(loginPanel, openPanel);
        }

        /// <summary>
        /// Login in the user. If successful- moving to the 
        /// Menu panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
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

            if(logResp != null) // there was no exception
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

        /// <summary>
        /// Moving from the Personal stats panel back to the
        /// Statistics panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC16_Click(object sender, EventArgs e)
        {
            MoveTab(personalStatsPanel, statisticsPanel);
        }

        /// <summary>
        /// Moving from the High scores panel back to the
        /// Statistics panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC15_Click(object sender, EventArgs e)
        {
            MoveTab(bestPlayersPanel, statisticsPanel);
        }

        /// <summary>
        /// Updating the high scores and if successful- moving 
        /// from the Statistics panel to the
        /// High scores panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC14_Click(object sender, EventArgs e)
        {
            if(UpdateHighScores())
            {
                PresentHighScores();
                MoveTab(statisticsPanel, bestPlayersPanel);
            }
        }

        /// <summary>
        /// Getting the personal statistics of the user and
        /// if successful moving to the Personal statistics
        /// panel from the Statistics panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
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

            if (getPersonalStatsResponse != null) // there was no exception
            {
                //Getting the statistics and showing them on the screen
                statistics = getPersonalStatsResponse.GetStatistics();
                textBox47.Text = statistics.Dequeue();
                textBox31.Text = statistics.Dequeue();
                textBox33.Text = statistics.Dequeue();
                textBox32.Text = statistics.Dequeue();
                textBox30.Text = statistics.Dequeue();
                MoveTab(statisticsPanel, personalStatsPanel);
            }
        }

        /// <summary>
        /// Moving from the Creating room panel back
        /// to the Menu panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC20_Click(object sender, EventArgs e)
        {
            MoveTab(createRoomPanel, menuPanel);
        }

        /// <summary>
        /// Moving from the Entering room panel back
        /// to the Menu panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC19_Click(object sender, EventArgs e)
        {
            MoveTab(enterRoomPanel, menuPanel);
        }

        /// <summary>
        /// Moving from the Menu panel 
        /// to the Creating room panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC9_Click(object sender, EventArgs e)
        {
            MoveTab(menuPanel, createRoomPanel);
        }

        /// <summary>
        /// Updating the list of the rooms and if successful-
        /// moving from the Menu panel to the Entering room panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC8_Click(object sender, EventArgs e)
        {
            if(UpdateListRooms())
            {
                PresentListRooms();
                MoveTab(menuPanel, enterRoomPanel); 
            }

        }

        /// <summary>
        /// The function sends a request to theserver and getts its response.
        /// </summary>
        /// <typeparam name="T"> The type of the request to send.</typeparam>
        /// <typeparam name="U"> The type of the response to get.</typeparam>
        /// <param name="request"> The request to send.</param>
        /// <param name="codeReq"> The code of the request.</param>
        /// <returns> The response of the server.</returns>
        private U SendRequestToServer<T, U>(T request, int codeReq)
        {
            //Making the request enable to pass to the Server
            string reqMsg = JsonRequestPacketSerializer.SerializeRequest<T>(request, codeReq);

            //Sending the msg to server and getting an answer
            this.communicator.SendRequestToServer(reqMsg);
            U var = JsonResponsePacketDeserializer.DeserializeResponse<U>(this.communicator.GetResponseFromServer());
            return var;

        }

        /// <summary>
        /// The function shows an Error message to the user.
        /// </summary>
        /// <param name="message"> The data of the ErrorMessageBox.</param>
        /// <param name="title"> The title of the ErrorMessageBox.</param>
        private void ShowErrorMessage(string message, string title)
        {
            MessageBox.Show(message, title, MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        /// <summary>
        /// Updating the list of the open rooms.
        /// </summary>
        /// <returns> If the updating was succesful or not.</returns>
        private bool UpdateListRooms()
        {
            GetRoomsResponse rooms = null;
            try
            {
                rooms = SendRequestToServer<NullableConverter, GetRoomsResponse>(null, REQUEST_CODES.GET_ROOMS_REQS_CODE);
            }
            catch (Exception excp)
            {
                ShowErrorMessage(excp.Message, "Error Getting Rooms!");
                return false;
            }
            //Waiting to have permission to update the list of the rooms
            this.roomsLock.WaitOne();
            this.rooms = new Queue<RoomData>(rooms.GetRoomDatas());
            this.roomsLock.ReleaseMutex();

            return true;
        }

        /// <summary>
        /// Presenting the list of the rrom in
        /// the Entering room panel.
        /// </summary>
        private void PresentListRooms()
        {
            Queue<RoomData> copyRooms = null;
            Queue<string> roomsNames = new Queue<string>();

            //Waiting to get permission from the lock to
            //access to the list of rooms.
            this.roomsLock.WaitOne();
            copyRooms = new Queue<RoomData>(this.rooms);
            this.roomsLock.ReleaseMutex();

            //Getting the names of the rooms
            while(copyRooms.Count > 0)
            {
                roomsNames.Enqueue(copyRooms.Dequeue().GetName());
            }

            //Inserting all the rooms in the combo box
            if (comboBox2.InvokeRequired)
            {
                comboBox2.Invoke((MethodInvoker)delegate
                {
                    UpdatingComboBox2(roomsNames);
                });
            }
            else
            {
                UpdatingComboBox2(roomsNames);
            }
        }

        /// <summary>
        /// The function inserts new values to ComboBox2.
        /// </summary>
        /// <param name="newItems"> The new items to insert to the combo box.</param>
        private void UpdatingComboBox2(Queue<string> newItems)
        {
            if (comboBox2.Items.Count > 0)
            {
                comboBox2.Items.Clear();
            }
            while (newItems.Count > 0)
            {
                comboBox2.Items.Add(newItems.Dequeue() + Constants.NEW_LINE);
            }
        }

        /// <summary>
        /// The function updates the high scores.
        /// </summary>
        /// <returns> If the updating was successful or not.</returns>
        private bool UpdateHighScores()
        {
            const string TITLE_ERROR = "Error Getting High Scores";
            GetHighScoreResponse highScoreResponse = null;

            try
            {
                highScoreResponse = SendRequestToServer<NullableConverter, GetHighScoreResponse>(null, REQUEST_CODES.GET_HIGH_SCORE_REQS_CODE);
            }
            catch (Exception excp)
            {
                ShowErrorMessage(excp.Message, TITLE_ERROR);
                return false;
            }

            //Waiting to get persmission to update the high scores.
            this.highScoresLock.WaitOne();
            this.highScores = highScoreResponse.GetStatistics();
            this.highScoresLock.ReleaseMutex();

            return true;
        }

        /// <summary>
        /// The function presents the high scores in the High scores panel.
        /// </summary>
        private void PresentHighScores()
        {
            Queue<string> theHighScores = null;

            ///Waiting to get permission to get the high scores.
            this.highScoresLock.WaitOne();
            theHighScores = new Queue<string>(this.highScores);
            this.highScoresLock.ReleaseMutex();

            if (theHighScores != null && theHighScores.Count > 0)
            {
                textBox44.Text = theHighScores.Dequeue();
            }
            if (theHighScores != null && theHighScores.Count > 0)
            {
                textBox39.Text = theHighScores.Dequeue();
            }
            if (theHighScores != null && theHighScores.Count > 0)
            {
                textBox45.Text = theHighScores.Dequeue();
            }
            if (theHighScores != null && theHighScores.Count > 0)
            {
                textBox41.Text = theHighScores.Dequeue();
            }
            if (theHighScores != null && theHighScores.Count > 0)
            {
                textBox46.Text = theHighScores.Dequeue();
            }
            if (theHighScores != null && theHighScores.Count > 0)
            {
                textBox43.Text = theHighScores.Dequeue();
            }
        }

        /// <summary>
        /// The function updates the state of the current room.
        /// </summary>
        /// <returns>if the updating was successful or not.</returns>
        private bool UpdateRoomState()
        {
            GetRoomStateResponse getRoomState = null;
            
            try
            {
                getRoomState = SendRequestToServer<NullableConverter, GetRoomStateResponse>(null, REQUEST_CODES.GET_ROOM_STATE_REQS_CODE);
            }
            catch
            {
                ShowErrorMessage("Can not get the state of the room", "Error Getting Room State");
                return false;
            }

            //Waiting to get permission to update the data of the room.
            this.roomDataLock.WaitOne();
            this.roomData = getRoomState;
            this.roomDataLock.ReleaseMutex();

            return true;
        }
        
        /// <summary>
        /// The function presents the current room state to the room member.
        /// </summary>
        /// <param name="roomName"> The name of the room the present.</param>
        private void PresentRoomStateMember(string roomName)
        {
            this.textBox57.Text = roomName;
            this.roomDataLock.WaitOne();
            UpdateTextBox(textBox63, "" + this.roomData.GetQuestionCount());
            UpdateTextBox(textBox64, "" + this.roomData.GetAnswerTimeOut());
            UpdateTextBox(textBox75, "" + this.roomData.GetPlayers().Count);
            AddTextsToListBox(this.roomData.GetPlayers(), this.listBox1);
            this.roomDataLock.ReleaseMutex();
        }

        /// <summary>
        /// The function presents the current room state to the room admin.
        /// </summary>
        /// <param name="roomName"> The name of the room the present.</param>
        private void PresentRoomStateAdmin(string roomName)
        {
            this.textBox72.Text = roomName;
            this.roomDataLock.WaitOne();
            UpdateTextBox(textBox69, "" + this.roomData.GetQuestionCount());
            this.questionsLeft = this.roomData.GetQuestionCount();
            textBox80.Text = "questions left: " + questionsLeft;
            UpdateTextBox(textBox67, "" + this.roomData.GetAnswerTimeOut());
            UpdateTextBox(textBox73, "" + this.roomData.GetPlayers().Count);
            AddTextsToListBox(this.roomData.GetPlayers(), this.listBox2);
            AddTextsToListBox(this.roomData.GetPlayers(), this.listBox3);
            // Assuming you have a ListBox named "myListBox"

            // Iterate through the ListBox items
            for (int i = 0; i < this.listBox3.Items.Count; i++)
            {
                // Retrieve the current item
                string currentItem = this.listBox3.Items[i].ToString();

                // Append zero to the current item
                string updatedItem = currentItem + " 0";

                // Update the ListBox with the modified item
                this.listBox3.Items[i] = updatedItem;
            }
            this.roomDataLock.ReleaseMutex();
        }

        /// <summary>
        /// The function adds to a ListBox new texts.
        /// </summary>
        /// <param name="texts"> The texts to insert.</param>
        /// <param name="list"> The ListBox to insert to.</param>
        private void AddTextsToListBox(Queue<string> texts, ListBox list)
        {
            Queue<string> copyTexts = new Queue<string>(texts);


            if (list.InvokeRequired)
            {
                list.Invoke((MethodInvoker)delegate
                {
                    InsertTextsToListBox(list, texts);
                });
            }
            else
            {
                InsertTextsToListBox(list, texts);
            }
        }

        /// <summary>
        /// The function access to a ListBox and inserts to it new texts.
        /// </summary>
        /// <param name="texts"> The texts to insert.</param>
        /// <param name="list"> The ListBox to insert to.</param>
        private void InsertTextsToListBox(ListBox list, Queue<string> texts)
        {
            if (list.Items.Count > 0)
            {
                list.Items.Clear();
            }
            while (texts.Count > 0)
            {
                list.Items.Add(texts.Dequeue() + Constants.NEW_LINE);
            }
        }

        /// <summary>
        /// The function getts the id of a room by its name.
        /// </summary>
        /// <param name="name"> The name of the room.</param>
        /// <returns> The id of the room.</returns>
        private int GetIdRoomByName(string name)
        {
            int id = Constants.ROOM_NOT_FOUND_ID;
            Queue<RoomData> roomDatas = null;
            RoomData roomData = null;

            //Waiting to get permission to copy the data of the rooms.
            this.roomsLock.WaitOne();
            roomDatas = new Queue<RoomData>(this.rooms);
            this.roomsLock.ReleaseMutex();

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

        /// <summary>
        /// Entering into a room and if successful and getting its state-
        /// moving to the Room member panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC17_Click(object sender, EventArgs e)
        {
            const string TITLE_ERROR = "Error Entering Room";
            JoinRoomRequest joinRoomRequest = null;
            JoinRoomResponse joinRoomResponse = null;
            int roomId = Constants.ROOM_NOT_FOUND_ID;

            //Getting the id of the selected room
            if(this.textBox78.Text.Length > 0)
            {
                roomId = GetIdRoomByName(textBox78.Text.Substring(0, textBox78.Text.Length - 1));
            }

            //if there is no selected room
            if(Constants.ROOM_NOT_FOUND_ID == roomId)
            {
                ShowErrorMessage("There is no room with this name", TITLE_ERROR);
                this.textBox78.Text = "";
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

                if(joinRoomResponse != null) //there was no exception
                {
                    if(Constants.OK_STATUS_CODE != joinRoomResponse.GetStatus())
                    {
                        ShowErrorMessage("Error Entering Room", TITLE_ERROR);
                    }
                    else
                    {
                        if(UpdateRoomState())
                        {
                            PresentRoomStateMember(textBox78.Text);
                            MoveTab(this.enterRoomPanel, this.roomMemberPanel);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Creatiing a room and if successful and getting its state-
        /// moving to the Room admin panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC18_Click(object sender, EventArgs e)
        {
            const string TITLE_ERROR = "Error Creating Room";
            string roomName = textBox51.Text;
            textBox79.Text = roomName;
            int timePerQuestion = int.Parse(textBox52.Text);
            this.seconds = timePerQuestion;
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

            if (createRoomResponse != null) //there was no exception
            {
                if (createRoomResponse.GetStatus() != Constants.OK_STATUS_CODE)
                {
                    ShowErrorMessage("Error creating room", TITLE_ERROR);
                }
                else
                {
                    if(UpdateRoomState())
                    {
                        PresentRoomStateAdmin(roomName);
                        MoveTab(this.createRoomPanel, this.roomAdminPanel);
                    }
                }

            }
        }

        /// <summary>
        /// Showing the value of the track bar in the 
        /// Creating room panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            textBox52.Text = trackBar1.Value.ToString();
        }

        /// <summary>
        /// Leaving the room and if successful moving to
        /// the Menu panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC21_Click(object sender, EventArgs e)
        {
            LeaveRoomResponse leaveRoomResponse = null;

            try
            {
                leaveRoomResponse = SendRequestToServer<NullableConverter, LeaveRoomResponse>(null, REQUEST_CODES.LEAVE_ROOM_REQS_CODE);
            }
            catch (Exception excp)
            {
                ShowErrorMessage(excp.Message, "Error Leaving room");
            }

            if(leaveRoomResponse != null && Constants.OK_STATUS_CODE == leaveRoomResponse.GetStatus())
            {
                MoveTab(roomMemberPanel, menuPanel);
            }
        }

        /// <summary>
        /// Closing the room and if successful moving 
        /// to the Menu panel.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC22_Click(object sender, EventArgs e)
        {
            CloseRoomResponse closeRoomResponse = null;

            try
            {
                closeRoomResponse = SendRequestToServer<NullableConverter, CloseRoomResponse>(null, REQUEST_CODES.CLOSE_ROOM_REQS_CODE);
            }
            catch (Exception excp)
            {
                ShowErrorMessage(excp.Message, "Error Closing room");
            }

            if (closeRoomResponse != null && Constants.OK_STATUS_CODE == closeRoomResponse.GetStatus())
            {
                MoveTab(roomAdminPanel, menuPanel);
            }
        }

        /// <summary>
        /// Starting the game and if successful showing a Message Box.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_WOC23_Click(object sender, EventArgs e)
        {
            StartGameResponse startGameResponse = null;

            try
            {
                startGameResponse = SendRequestToServer<NullableConverter, StartGameResponse>(null, REQUEST_CODES.START_GAME_REQS_CODE);
            }
            catch (Exception excp)
            {
                ShowErrorMessage(excp.Message, "Error Starting Game");
            }

            if(startGameResponse != null && Constants.OK_STATUS_CODE == startGameResponse.GetStatus())
            {
                MessageBox.Show("Game begun by you!");
            }
            MoveTab(roomAdminPanel, gamePanel);
            timer1.Start();
        }

        /// <summary>
        /// Updating a textBox with a new text.
        /// </summary>
        /// <param name="textBox"> The text box to update.</param>
        /// <param name="text"> The new text of the Text Box.</param>
        private void UpdateTextBox(TextBox textBox, string text)
        {
            if (textBox.InvokeRequired)
            {
                textBox.Invoke((MethodInvoker)(() => textBox.Text = text));
            }
            else
            {
                textBox.Text = text;
            }
        }

        /// <summary>
        /// Showing the selected room with a Text Box.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.textBox78.Text = this.comboBox2.Text;
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
           
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void timer1_Tick_1(object sender, EventArgs e)
        {
            textBox80.Text = "questions left: " + questionsLeft;
            GetQuestionResponse resp = null;
            GetGameResultsResponse response = null;
            Dictionary<int, string> answers = null;
            if(seconds == this.roomData.GetAnswerTimeOut())
            {

                resp = SendRequestToServer<NullableConverter, GetQuestionResponse>(null, REQUEST_CODES.GET_QUESTION_REQS_CODE);
                textBox81.Text = resp.GetQuestion();
                answers = resp.GetAnswers();
                button1.Text = answers[0];
                button2.Text = answers[1];
                button3.Text = answers[2];
                button4.Text = answers[3];
            }
            label1.Text = seconds--.ToString();
            if (seconds == 0)
            {
                if(this.isRight)
                {
                    updateScore(this.textBox20.Text);
                    this.isRight = false;
                }
                this.questionsLeft--;
                seconds = this.roomData.GetAnswerTimeOut();
                this.wasClicked = false;
            }
            if(questionsLeft == 0)
            {
                this.timer1.Stop();
                Queue<string> results = new Queue<string>();
                response = SendRequestToServer<NullableConverter, GetGameResultsResponse>(null, REQUEST_CODES.GET_GAME_RESULT_REQS_CODE);
                foreach (var i in response.GetPlayerResults())
                {
                    results.Enqueue("Name: " + i.GetUsername() + " Correct Answers: "+i.GetCorrectAnswerCount() + " Average time for question: "+ i.GetAverageAnswerTime());
                }
                AddTextsToListBox(results, this.listBox4);
                MoveTab(this.gamePanel, this.results);//move to the results tab
            }
        }
        private void updateScore(string playerName)
        {
            // Find the player's index
            int playerIndex = -1;
            for (int i = 0; i < this.listBox3.Items.Count; i++)
            {
                string playerEntry = this.listBox3.Items[i].ToString();
                if (playerEntry.StartsWith(playerName))
                {
                    playerIndex = i;
                    break;
                }
            }
            // Update the score
            if (playerIndex != -1)
            {
                string currentEntry = this.listBox3.Items[playerIndex].ToString();
                int currentScore = int.Parse(currentEntry.Substring(playerName.Length + 1));
                int updatedScore = currentScore + 1;  // Append score by 1

                // Update the ListBox with the new score
                this.listBox3.Items[playerIndex] = playerName + " " + updatedScore;
            }
        }
        private void SendAnswer(int id)
        {
            SubmitAnswerRequest req = new SubmitAnswerRequest(id);
            try
            {
                SubmitAnswerResponse response = SendRequestToServer<SubmitAnswerRequest, SubmitAnswerResponse>(req, REQUEST_CODES.SUBMIT_ANSWER_REQS_CODE);
                if(response.GetCorrectAnswerId() == id)
                {
                    this.isRight = true;
                }
            }
            catch (Exception ex)
            {
                ShowErrorMessage(ex.Message,"ERROR");
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if(!this.wasClicked)
            {
                this.wasClicked = true;
                SendAnswer(3);
            }
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            if (!this.wasClicked)
            {
                this.wasClicked = true;
                SendAnswer(1);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (!this.wasClicked)
            {
                this.wasClicked = true;
                SendAnswer(2);
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (!this.wasClicked)
            {
                this.wasClicked = true;
                SendAnswer(4);
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            string playerName = this.textBox20.Text;
            LeaveGameResponse resp = null;
            try
            {
                resp = SendRequestToServer<NullableConverter, LeaveGameResponse>(null, REQUEST_CODES.LEAVE_GAME_REQS_CODE);
                if(resp.GetStatus() != RESPONSE_CODES.ERROR_RESP_CODE)
                {
                    MoveTab(this.gamePanel, this.menuPanel);
                }
                else
                {
                    throw new Exception("Cant leave the game!");
                }
                //update the player list
                for (int i = 0; i < this.listBox3.Items.Count; i++)
                {
                    string playerEntry = this.listBox3.Items[i].ToString();
                    if (playerEntry.StartsWith(playerName))
                    {
                        playerEntry = "";
                        break;
                    }
                }

            }
            catch(Exception ex)
            {
                ShowErrorMessage(ex.Message, "ERROR");
            }

        }

        private void button_WOC24_Click(object sender, EventArgs e)
        {
            MoveTab(this.results, this.menuPanel);
        }
    }
}

