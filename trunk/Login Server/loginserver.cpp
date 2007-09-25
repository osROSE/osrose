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
#include "loginserver.h"

// Constructor
CLoginServer::CLoginServer( string fn )
{
    filename = fn;
    LoadConfigurations( (char*)filename.c_str() );    
    GServer = this; 
}

// Destructor	
CLoginServer::~CLoginServer( )
{
    
}

// Create client socket (alloc memory)
CLoginClient* CLoginServer::CreateClientSocket( )
{
	CLoginClient* thisclient = new (nothrow) CLoginClient( );
    if(thisclient==NULL)
        return NULL;
	thisclient->GS = this;
	return thisclient;
}

//LMA BEGIN
//20070623, 224500
bool CLoginServer::Ping()
{
         //Bogus request (checking if quest 1 exists).
        if(DB->QStore( "SELECT id FROM quest_data WHERE id='1'")==NULL)
        {
             Log( MSG_INFO, "MySQL Ping Time Error on port %u",DB->Port);
        }
        else
        {
            Log( MSG_INFO, "MySQL Ping Time Ok on port %u",DB->Port);
            DB->QFree( );
        }
        
     return true;
}
//LMA END

// Delete client socket (free memory)
void CLoginServer::DeleteClientSocket( CClientSocket* thisclient )
{
    CLoginClient* client = (CLoginClient*) thisclient;
    if(client->accesslevel==0xffff) //is a channel
    {
        for(UINT i=0;i<ServerList.size();i++)
        {
            CServers* thisserver = ServerList.at( i );
            if(client->userid == thisserver->id)
            {
                Log( MSG_INFO, "Server #%i disconnected", thisserver->id ); 
                DB->QExecute( "DELETE FROM channels WHERE id=%u and type=1", thisserver->id );             
                ServerList.erase( ServerList.begin()+i );
                delete thisserver;
                break;
            }
        }
    }
    else
    {
    	Log( MSG_INFO, "User disconnected" );            
    }
	delete (CLoginClient*)thisclient;
}

// Load information when server is ready
bool CLoginServer::OnServerReady( )
{
    DB->QExecute("DELETE FROM channels WHERE id=%i and type=%i", Config.ServerID, Config.ServerType );
    if(!DB->QExecute("INSERT INTO channels (id,type,name,host,port,lanip,lansubmask,connected,maxconnections,owner) VALUES (%u,%i,'%s','%s',%u,'%s','%s',0,%i,0)", 
            Config.ServerID, Config.ServerType, Config.ServerName, Config.LoginIP, Config.LoginPort, Config.LanIP, Config.LanSubnet, Config.MaxConnections ))
        Log(MSG_WARNING, "Error accessing to database, the other server will not connect to LoginServer" );
    return true;    
}

// Load Server configuration
void CLoginServer::LoadConfigurations( char* file )
{
    //Database
	Config.SQLServer.pcServer   = ConfigGetString ( file, "mysql_host", "localhost" );
	Config.SQLServer.pcDatabase = ConfigGetString ( file, "mysql_database", "roseon_beta" );
    Config.SQLServer.pcUserName = ConfigGetString ( file, "mysql_user", "root" );
	Config.SQLServer.pcPassword = ConfigGetString ( file, "mysql_pass", "" );
	Config.SQLServer.pcPort     = ConfigGetInt    ( file, "mysql_port", 3306 );
	//Server
    Config.ServerID             = ConfigGetInt    ( file, "serverid", 0 );
    Config.ServerType           = ConfigGetInt    ( file, "servertype", 0 );
    Config.LoginIP              = ConfigGetString ( file, "serverip", "127.0.0.1" );
	Config.LoginPort            = ConfigGetInt    ( file, "serverport", 29000 );
    Config.ServerName           = ConfigGetString ( file, "servername", "Loginserver" );
    Config.Connection           = ConfigGetInt    ( file, "connection", 0 );        
    Config.LanIP                = ConfigGetString ( file, "lanip", "192.168.0.1" );
    Config.LanSubnet            = ConfigGetString ( file, "lansubmask", "192.168.0" );
	//Passwords
	Config.LoginPass            = ConfigGetInt    ( file, "loginpass", 123456 );
	Config.CharPass             = ConfigGetInt    ( file, "charpass", 123456 );	
	Config.WorldPass            = ConfigGetInt    ( file, "worldpass", 123456 );	
    //Login
    Config.MinimumAccessLevel   = ConfigGetInt    ( file, "accesslevel", 100 );    	
	Config.usethreads           = ConfigGetInt    ( file, "usethreads", 0 )==0?false:true;
    Config.CreateLoginAccount   = ConfigGetInt    ( file, "CreateLoginAccount", 0 )==0?false:true;
}

// Handle packets
bool CLoginServer::OnReceivePacket( CClientSocket* thisclient, CPacket *P )
{
	switch( P->Command )
	{
	   case 0x500: return pakConnectToChar    ( (CLoginClient*)thisclient, P );        
	   case 0x703: return pakEncryptionRequest( (CLoginClient*)thisclient, P );
	   case 0x708: return pakUserLogin        ( (CLoginClient*)thisclient, P );
	   case 0x704: return pakGetServers       ( (CLoginClient*)thisclient, P ); 
	   case 0x70a: return pakGetIP            ( (CLoginClient*)thisclient, P );
	   case 0x808: return true;
    	default:
    		Log( MSG_WARNING, "[%i]Login Server Received unknown packet. Command:%04x Size:%04x", thisclient->sock, P->Command, P->Size );
		break;
	}
	return true;
}