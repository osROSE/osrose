/*
    Rose Online Server Emulator
    Copyright (C) 2006,2007 OSRose Team http://osroseon.to.md
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

    depeloped with Main erose/hrose source server + some change from the original eich source        
*/
#ifndef __ROSE_SOCKETS__
#define __ROSE_SOCKETS__

#ifdef _WIN32
    #define FD_SETSIZE 1024
    #include <windows.h>
    #include <winsock2.h>
    #define close closesocket
#else
    #include <unistd.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <arpa/inet.h>
    #include <sys/ioctl.h>
    #include <cstdarg>
    #include <errno.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define PVOID void *
    #define ioctlsocket ioctl
    #define SOCKADDR struct sockaddr
    #define closesocket close    
#endif
#include <mysql/mysql.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <exception>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <vector>
#include <csignal>
#include "log.h"
#include "rosecrypt.hpp"
#include "config.h"
#include "database/database.h"
// Just some defs for easier use
typedef char SBYTE;
typedef unsigned char BYTE; 
typedef short SWORD;
typedef unsigned short WORD;
typedef long SDWORD;
typedef unsigned long DWORD;
typedef long long SQWORD;
typedef unsigned long long QWORD;
using std::cout;
using std::endl;
using std::system;
using std::vector;
using std::nothrow;
using std::string;
#define STARTPACKET(p,c,s) CPacket p; p.Command = c; p.Size = s;
#define SETBYTE(p,o,v) p.Buffer[o]=v;
#define SETWORD(p,o,v) *((unsigned short*)&p.Buffer[o])=v;
#define SETDWORD(p,o,v) *((unsigned*)&p.Buffer[o])=v;
#define SETQWORD(p,o,v) *((long long*)&p.Buffer[o])=v;
#define SETFLOAT(p,o,v) *((float*)&p.Buffer[o])=v;

#define BEGINPACKET(p,c) CPacket p; p.Command = c; p.Size = 6;
#define RESETPACKET(p,c) p.Command = c; p.Size = 6;
#define ADDBYTE(p,v) { p.Buffer[p.Size-6]=v; p.Size+=1; }
#define ADDWORD(p,v) { *((WORD*)&p.Buffer[p.Size-6])=v; p.Size+=2; }
#define ADDDWORD(p,v) { *((unsigned*)&p.Buffer[p.Size-6])=v; p.Size+=4; }
#define ADDQWORD(p,v) { *((QWORD*)&p.Buffer[p.Size-6])=v; p.Size+=8; }
#define ADDFLOAT(p,v) { *((float*)&p.Buffer[p.Size-6])=v; p.Size+=4; }
#define ADDSTRING(p,v) { for(int q=0; q<(int)strlen((char*)v); q++) ADDBYTE(p,(v)[q]); }
#define GETBYTE(p,o) (p.Buffer[o])
#define GETWORD(p,o) *((WORD*)&p.Buffer[o])
#define GETDWORD(p,o) *((DWORD*)&p.Buffer[o])
#define GETQWORD(p,o) *((QWORD*)&p.Buffer[o])
#define GETFLOAT(p,o) *((float*)&p.Buffer[o])
#define pi 3.1415926535897932384626433832795
#define r2d pi/180

// Structures
// Packet information
struct CPacket
{
	unsigned short	Size;            // Packet size
	unsigned short	Command;         // Packet command
	unsigned short	Unused;	         // unused
	unsigned char	Buffer[0x1000];	 // Packet data //0x600
};

// SQL Configuration
struct CROSEServerConfigSQL
{
	char*		pcServer;								// IP
	char*		pcDatabase;								// Database
	char*		pcUserName;								// UserName
	char*		pcPassword;								// Password	
	int         pcPort;                    // port
};

// Server configuration
struct CROSEServerConfig
{
    // GLOBAL
    CROSEServerConfigSQL SQLServer;       // SQL Information
    bool        usethreads;               // Use Threads (in sockets)
    bool		Verbose;                  // print server information?
    unsigned	MaxConnections;           // Max connections
    unsigned	UserPerThread;	          // User per thread
    bool		AllowMultipleConnections; // Allow multiple connection from same ip?
    bool		StayInServerLoop;         // Server actived?    
    char*       LanSubnet;                //lansubnet
    char*       LanIP;                    //ip to send with lansubnet
    UINT        ServerID;                 //id from this server
    UINT        ParentID;                 //id from the server to connect
    char*       ServerName;               //Server name
    UINT        ServerType;               //0 login - 1 server(char) - 2 channel(world)
    BYTE        Connection;
    
	// LOGINSERVER	
	unsigned int MinimumAccessLevel; // Minimum access level   
    char*        LoginIP;            // LoginIP [external]
    unsigned int LoginPort;          // Login port      
    unsigned int LoginPass;    
    bool CreateLoginAccount;         // Create account in login if not exist?             
    bool checkGameGuard;             // Should we check if the user has gameguard?
	
	// CHARSEVER
	unsigned long int DeleteTime;    // Delete time
	char*        CharIP;             // CharIP
	unsigned int CharPort;           // Char port
    unsigned int CharPass;	
	
	// WORLDSERVER
	char*        WorldIP;
	unsigned int WorldPort;
    unsigned int WorldPass;     	
	bool AutoSave;                   // Autosave actived?
	UINT SaveTime;                   // Autosave time          									
    UINT EXP_RATE;
    UINT DROP_RATE;
    UINT ZULY_RATE;
    UINT AUTOSAVE;
    UINT SAVETIME;
    UINT MapDelay;  
    UINT VisualDelay;
    UINT WorldDelay;     
    char* WELCOME_MSG;
    UINT DROP_TYPE;    
    int MaxStat;
    int Partygap;
    int FairyMode;
    int FairyStay;
    int FairyWait;
    int FairyMax;
    int FairyTestMode;
    int PlayerDmg;
    int MonsterDmg;
    int Cfmode;
    int EventType; //Event
    int EventNPC;  //Event
    int osRoseVer ; //Revision of server
    
    // COMMAND LEVELS
    int Command_Go;
    int Command_Who;
    int Command_Who2;    
    int Command_Broadcast;
    int Command_Mute;
    int Command_Event;    //event
    int Command_Tele;
    int Command_Level;
    int Command_Reborn;  //reborn by Core
    int Command_LevelUp;
    int Command_TeleToMe;
    int Command_Save;
    int Command_Reload;
    int Command_Ann;
    int Command_SSpawn;
    int Command_Set;
    int Command_ESpawn;
    int Command_DSpawn;
    int Command_DelSpawn;
    int Command_Pak;
    int Command_Pak2;
    int Command_Pakm;
    int Command_Info;
    int Command_Exp;
    int Command_Mon;
    int Command_Mon2;
    int Command_Kick;
    int Command_Job;
    int Command_Cha;
    int Command_Item;
    int Command_Drop;
    int Command_GiveZuly;
    int Command_Npc;
    int Command_GiveFairy;
    int Command_ManageFairy;
    int Command_ChangeFairyWait;
    int Command_ChangeFairyStay;
    int Command_ChangeFairyTestMode;
    int Command_Move;
    int Command_Goto;
    int Command_PlayerInfo;
    int Command_Give2;
    int Command_Ban;
    int Command_Ani;
    int Command_Summon;
    int Command_ReloadQuest;
    int Command_Shutdown;
    int Command_DQuest;
    int Command_IQuest;
    int Command_ShopType;
    int Command_Stat;
    int Command_KillInRange;
    int Command_GoToMap;
    int Command_Heal;
    int Command_ServerInfo;
    int Command_TargetInfo;
    int Command_Hide;
    int Command_Class;
    int Command_Monster;
    int Command_GlobalTime;
    int Command_Here;  
    int Command_Face;  
    int Command_Hair;  
    int Command_Pvp;  
    int Command_go;  
    int Command_Convert;  
    int Command_Rate;  
    int Command_Moveto;  
    int Command_Settime;  
    int Command_Transx;  
    int Command_Partylvl;
    int Command_Pdmg;
    int Command_Mdmg;
    int Command_Rules;
    int Command_Cfmode;
    int Command_MaxStats;
    
    char* Command_GlobalPrefix;        
    
};


// Classes
// Base class
class CBaseSocket 
{
    public:
    	CBaseSocket( );						// Constructor
    	virtual ~CBaseSocket( );			// Destructor
    	void	CloseSocket( void );		// Close Socket
    	SOCKET			sock;				// This is our socket
    	bool			isActive;			// Is this socket connected?
    	char*			CryptTable;			// This is for decrypting incomming packets
    	string          ClientIP;       // Client IP
    	char            ClientSubNet[12];   // Lan Subnet
};
// Client class
class CClientSocket : public CBaseSocket 
{
    public:     
    	CClientSocket( );					// Client Constructor
    	~CClientSocket( );				    // Client Destructor
    	bool ReceiveData( );                // Receive packet
    	void SendPacket( CPacket *P );      // Send Packet
    	class CServerSocket* GS;	        // Pointer to Server class
    	struct sockaddr_in clientinfo;
        void* player;
    private:
    	unsigned short	PacketSize;		    // Size of the current packet
    	unsigned short	PacketOffset;		// Current offset of the data
    	unsigned char	Buffer[ 0x600 ];    // Buffer to receive data into
};

// Server class
class CServerSocket : public CBaseSocket 
{
    public:
    	CServerSocket( );					 // Constructor
    	~CServerSocket( );					 // Destructor

       	bool StartServer( );                 // Start Server
    	void FillFDS( fd_set* fds );         // Create FDS table
    	void HandleClients( fd_set* fds );   // Handle Clients
    	void AddUser( SOCKET sock, sockaddr_in* ClientInfo ); // Add User
    	void DisconnectClient( CClientSocket* thisclient );   // Disconnect Client

        // Virtual Functions
       	virtual void ServerLoop( );                  // Main Loop         
        virtual CClientSocket* CreateClientSocket( ); // Create a client 
    	virtual void DeleteClientSocket( CClientSocket* thisclient ); // Delete client
    	virtual bool OnServerReady( );       // Executed with server is ready
    	virtual void OnServerStep( );        // Executed
    	virtual void OnServerDie( );
    	virtual bool OnReceivePacket( CClientSocket* thisclient, CPacket* P );
    	virtual bool OnClientConnect( CClientSocket* thisclient );
    	virtual void OnClientDisconnect( CClientSocket* thisclient );  
                
    	//LMA BEGIN
    	//20070623, 224500
    	virtual bool Ping();             //MySQL Ping
    	//LMA END
    	
        // Variables
        CROSEServerConfig	Config;	          // Hold server configuration
        int  maxfd;
        vector<CClientSocket*>      ClientList;    
        UINT                        ConnectedClients;	
    	unsigned short				port;
    	pthread_t                   threads[65535];
};
PVOID ClientMainThread( PVOID ); // Handle clients
bool	InitWinSocket ( void ); // Init windows sockets (wsa)
void	CloseWinSocket( void ); // Close Windows sockets (wsa)
// -----------------------------------------------------------------------------------------

#endif
