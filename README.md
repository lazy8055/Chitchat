# ChitChat
Chitchat is a simple chatroom terminal tool created using sockets and select() in c. Multiple user can connect to the server and communicate with each other.
I created this project as a mini project for my college so don't expect advanceded features like sending photo and voice messages. 
It currently only works in linux. It definitely works on ubuntu but for other linux distributions check out the macros in utility/helper.h 
wheather the terminal emulator and the flag used to execute the file in the respective terminal emulators are correct.
<br>
Look at the Demo gif is in the Demo folder to know how to use ChitChat.
<br><br>
![image](https://github.com/lazy8055/Chitchat/assets/74041622/e584e33b-6ebe-46c0-a0a8-c1b5cb5c5d4e)
<br><br><br>


## Installation
First clone this repo in your local machine by executing the below command in your terminal
```bash
git clone https://github.com/lazy8055/Chitchat.git
```
Then to compile all the required c files execute the make command inside the Chitchat folder
```bash
make
```
<br><br><br>
## Usage
### For Server
Open terminal in the Chitchat folder and execute chitchat file with flag -s or --server
```bash
./chitchat -s
```
Or
```bash
./chitchat --server
```
### For Client
Open terminal in the Chitchat folder and execute chitchat file with flag -s or --server
```bash
./chitchat -c
```
Or
```bash
./chitchat --client
```
<br>

After the give a name for your typespace which is going to the name of the fifo which will act as a communication medium for two terminals.
If you don't know about fifo file don't worry about it. Just give it a random name wihout any symbols.
After that a new terminal will be appeared in which you are going to write your messages and the terminal in which the chitchat program is running is just for display
<br>
![image](https://github.com/lazy8055/Chitchat/assets/74041622/bec77aa1-b622-4224-a521-f8a08bbb39d1)
<br><br>


Type help in the type space to know what you can do
![image](https://github.com/lazy8055/Chitchat/assets/74041622/111debc0-73be-44e6-a6e4-d4162d752b8e)

<br><br>

For client, you need to give the ipv4 address of the server and the user name that you want. Don't forget that symbols are not allowed in username.
<br>
![image](https://github.com/lazy8055/Chitchat/assets/74041622/2520ad3d-497b-4a60-9313-2cfad2ce67bb)
<br><br><br>
## External links
<ul>
<li>Basic chatroom in C: https://avabodha.in/networking-project-in-c-chat-room/</li>
<li>Fifo files : https://youtu.be/dniz3nEOEjU?si=qJ2Z-O3_SX967lEo</li>

</ul>
<br><br>





