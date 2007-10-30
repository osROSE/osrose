/*
    Rose Online Server Emulator
    Copyright (C) 2006,2007 OSRose Team http://www.osrose.net
    
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
#include "worldserver.h"

// Send Characters information
void CWorldServer::pakPlayer( CPlayer *thisclient )
{    
	CRespawnPoint* thisrespawn = GetRespawnByID( thisclient->Position->respawn );   
	if(thisrespawn!=NULL)
	{
        thisclient->Position->destiny = thisrespawn->dest;
    	thisclient->Position->current = thisclient->Position->destiny;   	
    	thisclient->Position->Map = thisrespawn->destMap;
    }
    else
    {
        thisclient->Position->destiny.x = 5300;
        thisclient->Position->destiny.y = 5200;
        thisclient->Position->current = thisclient->Position->destiny;
        thisclient->Position->Map = 2;
    }
    CMap* map = MapList.Index[thisclient->Position->Map];
    map->AddPlayer( thisclient );
    BEGINPACKET( pak, 0x715 );
    ADDBYTE    ( pak, thisclient->CharInfo->Sex );                       // Sex
    ADDWORD    ( pak, thisclient->Position->Map );		                 // Map
    ADDFLOAT   ( pak, thisclient->Position->current.x*100 );	                 // Pos X
    ADDFLOAT   ( pak, thisclient->Position->current.y*100 );		             // Pos Y
    ADDWORD    ( pak, 0x0000 );
    ADDDWORD   ( pak, thisclient->CharInfo->Face );			             // Face
    ADDDWORD   ( pak, thisclient->CharInfo->Hair );	             // Hair
    ADDWORD    ( pak, thisclient->items[2].itemnum );	         // Cap
    ADDWORD    ( pak, BuildItemRefine( thisclient->items[2] ) ); // Cap Refine
    ADDWORD    ( pak, thisclient->items[3].itemnum );	         // Body
    ADDWORD    ( pak, BuildItemRefine( thisclient->items[3] ) ); // Body Refine
    ADDWORD    ( pak, thisclient->items[5].itemnum );	         // Gloves
    ADDWORD    ( pak, BuildItemRefine( thisclient->items[5] ) ); // Gloves Refine
    ADDWORD    ( pak, thisclient->items[6].itemnum );	         // Boots
    ADDWORD    ( pak, BuildItemRefine( thisclient->items[6] ));	 // Boots Refine
    ADDWORD    ( pak, thisclient->items[1].itemnum );	         // Face
    ADDWORD    ( pak, BuildItemRefine( thisclient->items[1] ) ); // Face Refine
    ADDWORD    ( pak, thisclient->items[4].itemnum );	         // Back
    ADDWORD    ( pak, BuildItemRefine( thisclient->items[4] ) ); // Back Refine
    ADDWORD    ( pak, thisclient->items[7].itemnum );	         // Weapon
    ADDWORD    ( pak, BuildItemRefine( thisclient->items[7] ) ); // Weapon Refine
    ADDWORD    ( pak, thisclient->items[8].itemnum );	         // SubWeapon
    ADDWORD    ( pak, BuildItemRefine( thisclient->items[8] ) ); // SubWeapon Refine
    ADDBYTE    ( pak, 0 );
    ADDWORD    ( pak, 0x140f );
	ADDWORD    ( pak, thisclient->CharInfo->Job );			         // Job
    ADDBYTE    ( pak, 0 );
    ADDWORD    ( pak, 0 );
    ADDWORD    ( pak, thisclient->Attr->Str );			             // Str
    ADDWORD    ( pak, thisclient->Attr->Dex );				         // Dex
    ADDWORD    ( pak, thisclient->Attr->Int );				         // Int
    ADDWORD    ( pak, thisclient->Attr->Con );				         // Con
    ADDWORD    ( pak, thisclient->Attr->Cha );				         // Cha
    ADDWORD    ( pak, thisclient->Attr->Sen );				         // Sen
	ADDWORD    ( pak, thisclient->Stats->HP );                 // Current HP
	ADDWORD    ( pak, thisclient->Stats->MP );                 // Current MP
	ADDWORD    ( pak, thisclient->CharInfo->Exp );                       // Exp
	ADDWORD    ( pak, 0 );			
	ADDWORD    ( pak, thisclient->Stats->Level );			         // Level
	ADDWORD    ( pak, thisclient->CharInfo->StatPoints );				 // Stat Points 
	ADDWORD    ( pak, thisclient->CharInfo->SkillPoints );               // Skill Points
	ADDWORD    ( pak, 0x6464 );
	for(int i=0; i<37; i++) ADDBYTE( pak, 0 );					 
	ADDWORD( pak, thisclient->CharInfo->stamina );						 // Stamina
	for(int i=0; i<326; i++) ADDBYTE( pak, 0 );					
	for(int i=0; i<MAX_SKILL; i++) // Class Skills   
        ADDWORD( pak, thisclient->cskills[i].id+thisclient->cskills[i].level-1 );
	for(int i=0; i<260; i++)  ADDWORD( pak, 0 );
	for(int i=0; i<42; i++)       // Basic Skills                               
		ADDWORD( pak, thisclient->bskills[i] );	
	for(int i=0; i<48; i++)       // QuickBar
        ADDWORD( pak, thisclient->quickbar[i] );
	ADDDWORD   ( pak, thisclient->CharInfo->charid );	                     // CharID
	for(int i=0; i<80;i++) ADDBYTE( pak, 0 );
	ADDSTRING  ( pak, thisclient->CharInfo->charname );                      // Char Name
	ADDBYTE    ( pak, 0 );
    thisclient->client->SendPacket( &pak );
}

// Send Inventory Information
void CWorldServer::pakInventory( CPlayer *thisclient )
{
	BEGINPACKET( pak, 0x716 );
	ADDQWORD( pak, thisclient->CharInfo->Zulies );
	//ADDWORD( pak, 0 );
	for(unsigned j=0; j<140; j++)
    {        
       	ADDDWORD( pak, BuildItemHead( thisclient->items[j] ) );
       	ADDDWORD( pak, BuildItemData( thisclient->items[j] ) ); 
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );  

	}
    thisclient->client->SendPacket( &pak );
}


// Send Quest information
void CWorldServer::pakQuestData( CPlayer *thisclient )
{
    BEGINPACKET( pak, 0x71b );   
/*    
    char buffer;                                  
    FILE *packet1 = fopen("packet/quest.dat","r");       //  I USE THIS FOR TEST!
    if(packet1==NULL)
    {
        cout << "file not founded: check pakQuestData function" << endl;
        return;                           
    }
    while((fscanf(packet1,"%c",&buffer))!=EOF)
        ADDBYTE(pak,buffer);      
    fclose(packet1);     
    thisclient->client->SendPacket( &pak );
    return;
*/

    //LMA: Quest Variables (50 bytes)
    for(int i=0;i<25;i++)
    {
         if (thisclient->QuestVariables[i]!=0)
         {
            Log(MSG_INFO,"quest variable %i=%i",i,thisclient->QuestVariables[i]); 
         }
         
        ADDBYTE( pak, thisclient->QuestVariables[i] );          
        ADDBYTE( pak, 0x00 );         
    }
    
    int b = 0;
    for(int i=0;i<thisclient->MyQuest.size( );i++)
    {
        
        QUESTS* myquest = thisclient->MyQuest.at( i );
     if( myquest->active )
        {
            ADDWORD( pak, myquest->thisquest->id );  //# Quest
            
            //LMA BEGIN
            //20060622, 163000
            //patch for healing hands, soil of purification and antidotes
            //or Clan wars or hunting practice.
            switch (myquest->thisquest->id)
            {
                   case 106:
                      {
                           ADDQWORD( pak, 180 ); //Time
                     }
                     break;                       
                   case 107:
                      {
                           ADDQWORD( pak, 180 ); //Time
                     }
                     break;                       
                   case 2813:
                       {
                           ADDQWORD( pak, 120 ); //Time
                     }
                     break;                      
                   case 5053:
                       {
                           ADDQWORD( pak, 60 ); //Time
                     }
                     break; 
                   case 957:
                       {
                           ADDQWORD( pak, 120 ); //Time
                     }
                     break; 
                   case 958:
                       {
                           ADDQWORD( pak, 150 ); //Time
                     }
                     break; 
                   case 959:
                       {
                           ADDQWORD( pak, 240 ); //Time
                     }
                     break;                                                                                     
                   default:
                     {
                           ADDQWORD( pak, 0x00000000 ); //Time
                     }
                     break;
            }
            //LMA END
            
            for(int j=0;j<0x14;j++)
                ADDBYTE( pak, 0x00 );

            for(int j=0;j<5;j++)
            {
                if( myquest->thisquest->CItem[j] != 0 && myquest->items[j]!= 0 )
                {
                    CItem item;
                    item.itemnum = myquest->thisquest->CItem[j];
                    item.itemtype = 13;
                    item.count = 1;
                    ADDDWORD( pak, BuildItemHead( item ) );
                    ADDDWORD( pak, myquest->items[j] );
                    ADDDWORD( pak, 0x00000000 );
                    ADDWORD ( pak, 0x0000 );
                }
                else
                {
                    ADDQWORD( pak, 0x00000000 );
                    ADDDWORD( pak, 0x00000000 );
                    ADDWORD ( pak, 0x0000 );
                }
            }
            ADDQWORD( pak, 0x00000000 ); 
            ADDDWORD( pak, 0x00000000 ); //LMA 139+
            ADDWORD ( pak, 0x0000 );     //LMA 139+

                            
            b++;
            continue;
        }       
        if(b>=10)
            break;
    }
    for(int i=b;i<10;i++)
    {
       /*
        for(int j=0;j<0x4e;j++)
            ADDBYTE( pak, 0x00 );
        */
 
        //LMA 139+        
        for(int j=0;j<0x72;j++)
            ADDBYTE( pak, 0x00 ); 
    } 
    //ADDBYTE( pak, 0x00 );
    
    //quest Flags are here :)
    ADDDWORD( pak, thisclient->speaksLuna ? 8 : 0 ); //Enable luna language (currently enabled for all b4 I make a proper quest for it)
    ADDDWORD( pak, thisclient->canUseFlyingVessel ? 32 : 0 ); //Enable flying vessel    
    
    
    //TESTS
    //ADDDWORD( pak, 0xbfc2) //TEST LMA
    //ADDDWORD( pak, 0)
    //

    for(int i=4;i<41;i++)
        ADDWORD( pak, 0);
             
    ADDBYTE( pak, 0x00 );        
    for(int i=0;i<30;i++) // Wish list [Caali]
    {
        ADDDWORD( pak, 0x00000000 ); //Item Head
        ADDDWORD( pak, 0x00000000 ); //Item Data
    }  

    thisclient->client->SendPacket( &pak );
}

// Get this user set up with the encryption and stuff
bool CWorldServer::pakDoIdentify( CPlayer *thisclient, CPacket *P )
{
	MYSQL_ROW row;
	thisclient->Session->userid = GETDWORD((*P), 0);
	memcpy( thisclient->Session->password, &P->Buffer[4], 32 );
	MYSQL_RES *result = DB->QStore("SELECT username,lastchar,accesslevel,zulystorage FROM accounts WHERE id=%i AND password='%s'", thisclient->Session->userid, thisclient->Session->password);
    if(result==NULL) return false;
	if (mysql_num_rows( result ) != 1) 
    {
		Log( MSG_HACK, "Someone tried to connect to world server with an invalid account" );
		DB->QFree( );
		return false;
	}
	row = mysql_fetch_row(result);
	strncpy( thisclient->Session->username, row[0],16 );
	strncpy( thisclient->CharInfo->charname, row[1],16 );
	thisclient->Session->accesslevel = atoi(row[2]);
	thisclient->CharInfo->Storage_Zulies = atoi( row[3] );
	DB->QFree( );
	if(!thisclient->loaddata( )) return false;
	Log( MSG_INFO, "User '%s'(#%i) logged in with character '%s'", thisclient->Session->username, thisclient->Session->userid, thisclient->CharInfo->charname);
	BEGINPACKET( pak, 0x070c );
	ADDBYTE    ( pak, 0 );
	ADDDWORD   ( pak, 0x87654321 );
	ADDDWORD   ( pak, 0x00000000 );
	thisclient->client->SendPacket( &pak );
    pakPlayer(thisclient);
    pakInventory(thisclient);
    pakQuestData(thisclient);
	RESETPACKET( pak, 0x7de );
	/*ADDDWORD   ( pak, 0x000c1003 );
	ADDDWORD   ( pak, 0xffff0000 );
	ADDDWORD   ( pak, 0x00000000 );
	ADDDWORD   ( pak, 0x9b000038 );
	ADDDWORD   ( pak, 0x7272656a );
	ADDDWORD   ( pak, 0x3c3c3479 );
	ADDDWORD   ( pak, 0x534d5547 );
	ADDWORD    ( pak, 0x3e3e );
	ADDBYTE    ( pak, 0x00 );*/
	// This packet didn't match what I was getting on official. Changing it allowed jRose connections. This is a "Platinum" packet.
	ADDDWORD   ( pak, 0x00011002 );
	ADDDWORD   ( pak, 0x006f0000 );
	ADDDWORD   ( pak, 0x32350000 );
	ADDDWORD   ( pak, 0x1d383239 );
	ADDDWORD   ( pak, 0x31093033 );
	ADDDWORD   ( pak, 0x3c3c6c09 );
	ADDDWORD   ( pak, 0x534d5547 );
	ADDWORD    ( pak, 0x3e3e );
	ADDBYTE    ( pak, 0x00 );
	thisclient->client->SendPacket( &pak );
	RESETPACKET( pak, 0x702 );
	ADDSTRING  ( pak, Config.WELCOME_MSG );
	ADDBYTE    ( pak, 0 );
	thisclient->client->SendPacket( &pak );	
	//SendSysMsg( thisclient, "Open Source Rose Online Private Server" );
	
	thisclient->SetStats( );
	return true;
}

// Give the user an ID
bool CWorldServer::pakDoID( CPlayer* thisclient, CPacket* P )
{
	// Assign a new id to this person
   	thisclient->clientid = GetNewClientID();    
	if (thisclient->clientid <= 0) 
    {
		Log( MSG_WARNING, "User '%s'(#%i) denied access. Server is full.", thisclient->Session->username, thisclient->Session->userid );
		return false;
	}
	Log( MSG_INFO, "User '%s'(#%i) assigned id #%i", thisclient->Session->username, thisclient->Session->userid, thisclient->clientid );
    if( thisclient->Party->party )
    {
        BEGINPACKET( pak, 0x7d5 );
        ADDDWORD   ( pak, thisclient->CharInfo->charid );
        ADDWORD    ( pak, thisclient->clientid );
        ADDWORD    ( pak, thisclient->GetMaxHP( ) );
        ADDWORD    ( pak, thisclient->Stats->HP );
        ADDDWORD   ( pak, 0x01000000 );
        ADDDWORD   ( pak, 0x0000000f );
        ADDWORD    ( pak, 0x1388 );
        thisclient->Party->party->SendToMembers( &pak, thisclient );
    }   
	BEGINPACKET( pak, 0x721 );
    ADDWORD    ( pak, 0x0022 );
    ADDWORD    ( pak, 0x0002 );
    ADDWORD    ( pak, 0x0000 );			
    thisclient->client->SendPacket( &pak );
    
	RESETPACKET( pak, 0x730 );
    ADDWORD    ( pak, 0x0005 );			
    ADDDWORD   ( pak, 0x40b3a24d );		
    thisclient->client->SendPacket( &pak );

    RESETPACKET( pak, 0x753 );
    ADDWORD    ( pak, thisclient->clientid );			// USER ID
    ADDWORD    ( pak, thisclient->Stats->HP );		// CURRENT HP
    ADDWORD    ( pak, thisclient->Stats->MP );		// CURRENT MP
    ADDDWORD   ( pak, thisclient->CharInfo->Exp );				// CURRENT EXP
    ADDDWORD   ( pak, 0x00000000 );						// LVL EXP (UNSUSED)        
    // thanks to StrikeX to post this source  
        //[economy]    
    ADDWORD    ( pak, 0x0063 );  // World Rate
    ADDBYTE    ( pak, 0x70 );
    ADDBYTE    ( pak, 0x69 );
    ADDBYTE    ( pak, 0x68 );
    ADDBYTE    ( pak, 0x67 );                
    ADDWORD    ( pak, 0x0062 );  // Town rate    
    ADDBYTE    ( pak, 0x61 );  // misc rate
    ADDBYTE    ( pak, 0x32 );  //1
    ADDBYTE    ( pak, 0x32 );  //2
    ADDBYTE    ( pak, 0x32 );  //3           
    ADDBYTE    ( pak, 0x32 );  //4
    ADDBYTE    ( pak, 0x32 );  //5
    ADDBYTE    ( pak, 0x32 );  //6
    ADDBYTE    ( pak, 0x32 );  //7    
    ADDBYTE    ( pak, 0x32 );  //8
    ADDBYTE    ( pak, 0x32 );  //9
    ADDBYTE    ( pak, 0x32 );  //10
    ADDBYTE    ( pak, 0x32 );  //11
    CMap* map = MapList.Index[thisclient->Position->Map];
    if(map->allowpvp!=0)
        ADDWORD(pak, 0x0001)//player vs player map
    else
        ADDWORD(pak, 0x0000)//non player vs player map
    ADDWORD    (pak, 0x0000 );//??
    // Map Time
    ADDDWORD( pak, map->MapTime );
    if(map->allowpvp==1){ ADDWORD(pak, 51 );} // pvp all vs all
    else if(map->allowpvp==2) // pvp group vs group
    {
        /*ADDWORD(pak, thisclient->Clan->clanid );*/ 
        ADDWORD(pak, 51);
    }
    else 
    {
        ADDWORD(pak, 2 );
    }
    thisclient->client->SendPacket( &pak );
    // set weight
    RESETPACKET( pak, 0x762 );
    ADDWORD    ( pak, thisclient->clientid );       	// USER ID
    ADDBYTE    ( pak, 1 );								// SOMETHING TO DO WITH WEIGHT
    thisclient->client->SendPacket( &pak );
    // set speed
	RESETPACKET(pak, 0x782 );
	ADDWORD    ( pak, thisclient->clientid );
	ADDBYTE    ( pak, thisclient->Status->Stance );
	ADDWORD    ( pak, thisclient->Stats->Move_Speed );
	SendToVisible( &pak, thisclient );
    thisclient->CleanPlayerVector( );
	thisclient->Session->inGame = true;
	thisclient->firstlogin=clock();    //LMA for fairy
	return true;
}

// Move Characters in map
bool CWorldServer::pakMoveChar( CPlayer* thisclient, CPacket* P )
{
    if( thisclient->Shop->open || (!thisclient->Ride->Drive && thisclient->Ride->Ride) || !thisclient->Status->CanMove )
        return true;
    if( thisclient->Status->Stance==1 )
        thisclient->Status->Stance=3;                
    ClearBattle( thisclient->Battle );
	thisclient->Battle->target = GETWORD((*P), 0x00 );
	thisclient->Position->destiny.x = GETFLOAT((*P), 0x02 )/100;
    thisclient->Position->destiny.y = GETFLOAT((*P), 0x06 )/100;
    thisclient->Position->lastMoveTime = clock();
	BEGINPACKET( pak, 0x79a );
	ADDWORD    ( pak, thisclient->clientid );		// USER ID
	ADDWORD    ( pak, thisclient->Battle->target );		// TARGET
	ADDWORD    ( pak, thisclient->Stats->Move_Speed );	// MSPEED
	ADDFLOAT   ( pak, GETFLOAT((*P), 0x02 ) );	// POSITION X
	ADDFLOAT   ( pak, GETFLOAT((*P), 0x06 ) );	// POSITION Y
	ADDWORD    ( pak, GETWORD((*P), 0x0a ) );		// POSITION Z (NOT USED)
	SendToVisible( &pak, thisclient );
	return true;
}

// Stop Character movement
bool CWorldServer::pakStopChar( CPlayer* thisclient, CPacket* P )
{
    ClearBattle( thisclient->Battle );
	thisclient->Position->destiny.x = GETFLOAT((*P), 0x00 )/100;
    thisclient->Position->destiny.y = GETFLOAT((*P), 0x04 )/100;
	BEGINPACKET( pak, 0x770 );
	ADDWORD    ( pak, thisclient->clientid );		// USER ID
	ADDFLOAT   ( pak, thisclient->Position->destiny.x*100 );	// POSITION X 
	ADDFLOAT   ( pak, thisclient->Position->destiny.y*100 );	// POSITION Y
	ADDWORD    ( pak, GETWORD((*P), 0x08 ) );		// POSITION Z (NOT USED)
	SendToVisible( &pak, thisclient );
	return true;
}

// Spawn NPC
bool CWorldServer::pakSpawnNPC( CPlayer* thisclient, CNPC* thisnpc )
{
	BEGINPACKET( pak, 0x791 );
    ADDWORD( pak, thisnpc->clientid );
	ADDFLOAT( pak, thisnpc->pos.x*100 );
    ADDFLOAT( pak, thisnpc->pos.y*100 );
    ADDFLOAT( pak, thisnpc->pos.x*100 );
    ADDFLOAT( pak, thisnpc->pos.y*100 );
	ADDBYTE( pak, 0x00 );
    ADDWORD( pak, 0x0000 );
	ADDWORD( pak, 0x0000 );
	ADDWORD( pak, 0x03e8 );
	ADDWORD( pak, 0x0000 );
	ADDWORD( pak, 0x0001 );
	ADDWORD( pak, 0x0000 );//Buffs
	ADDWORD( pak, 0x0000 );//Buffs
	ADDWORD( pak, 0x0000 );//buffs
	ADDWORD( pak, thisnpc->npctype );	
    if(thisnpc->thisnpc->dialogid!=0)
    {
        ADDWORD( pak, thisnpc->thisnpc->dialogid );
    }
    else
    {// fixed by tomiz [npc dialogs fixed [still not all]]
         unsigned int factor;
        if (thisnpc->npctype >= 1000 && thisnpc->npctype <=1041 || thisnpc->npctype >= 1043 && thisnpc->npctype <=1084 || thisnpc->npctype >= 1086 && thisnpc->npctype <=1119 || thisnpc->npctype >= 1131 && thisnpc->npctype <=1199 || thisnpc->npctype >= 1207 && thisnpc->npctype <=1299) factor=900;
       	//else if (thisnpc->npctype >= 1200 && thisnpc->npctype <=1203 || thisnpc->npctype >= 1205 && thisnpc->npctype <=1206) factor=896; // event npc
       	else if (thisnpc->npctype >= 1200 && thisnpc->npctype <=1203 || thisnpc->npctype >= 1206 && thisnpc->npctype <=1206) factor=896; // event npc
        else if (thisnpc->npctype == 1042 || thisnpc->npctype == 1085 ) factor=899;  // Storage Adventure Plain -> Fabrizio - Junon Clan Field -> Nell
        else if (thisnpc->npctype == 1120 ) factor=896;  // dunno if that dialog are corect 1120 [Clan Clerk] Regina
        else if (thisnpc->npctype == 1204 ) factor=897;  // event npc another warp to zant dialog npc 1203 [Event Info] Lucielle Fete
        else if (thisnpc->npctype == 1473 ) factor=1457; // should be ok 1473 Melendino adventure plain
        else if (thisnpc->npctype == 1752 ) factor=1528; // Clan Merchant Aliche Patt ver 141
        else if (thisnpc->npctype == 1121 ) factor=900; // Ikaness Staff Shroon - Anima Lake
        else if (thisnpc->npctype == 1205 ) factor=901; // Event Santa Claus - Non event factor 896, event 901
        else if (thisnpc->npctype >= 1122 && thisnpc->npctype <=1130) factor=905;                
        else if (thisnpc->npctype >= 1500 && thisnpc->npctype <=1599) factor=1210;
        else if (thisnpc->npctype >= 1750 && thisnpc->npctype <=1755 || !thisnpc->npctype == 1752 ) factor=1000; //should be ok
        
        if (thisnpc->dialog!=0)
        {
            //Log(MSG_INFO,"Special dialog %i for NPC %i",thisnpc->dialog, thisnpc->npctype);       
            ADDWORD( pak, thisnpc->dialog );    
        }
        else
        {
            //Log(MSG_INFO,"Dialog %i for NPC %i",thisnpc->npctype - factor, thisnpc->npctype);            
            ADDWORD( pak, thisnpc->npctype - factor );    
        }
        
    }	
	ADDFLOAT( pak, thisnpc->dir );
	
    if (thisnpc->npctype == 1115)
    { 
       ADDBYTE( pak, GServer->Config.Cfmode ) // Burland Clan Field open/close
    }
    
    //Event:
    if (thisnpc->event!=0&&thisnpc->npctype!=1115)
    {
        Log(MSG_INFO,"Event number %i for NPC %i",thisnpc->event, thisnpc->npctype);
        ADDBYTE ( pak, thisnpc->event);
    }
    else
    {
        ADDBYTE ( pak, 0 );
     }
    
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );
	return true;
}

// Changes stance
bool CWorldServer::pakChangeStance( CPlayer* thisclient, CPacket* P )
{
    if(thisclient->Shop->open)
        return true;    
	BYTE stancenum = GETBYTE((*P),0x00);
	if (stancenum == 0)
	{
        if(thisclient->Status->Stance == RUNNING) //Walking
        {
            stancenum = WALKING;
            thisclient->Status->Stance = WALKING;
        }
        else
        if(thisclient->Status->Stance != DRIVING) // Running
        {
            stancenum = RUNNING;
            thisclient->Status->Stance = RUNNING;
        }
    }	
    else   
	if (stancenum == 1) 
    {
        if(thisclient->Status->Stance!=DRIVING && thisclient->Status->Stance>0x01) //
        {   // Walking
			thisclient->Status->Stance = 0x01;            			
        }
        else
        if(thisclient->Status->Stance<0x04)
        {   //Runing         
			thisclient->Status->Stance = 0x03;                      
        }
    }    
    else
	if (stancenum == 2) 
    {
		if (thisclient->Status->Stance==RUNNING || thisclient->Status->Stance==0x02)
		{// // Driving
            // Clean Buffs 		  
            stancenum = DRIVING;
			thisclient->Status->Stance = DRIVING;			
            thisclient->Ride->Ride = false;
            thisclient->Ride->Drive = true;
            thisclient->Ride->charid = 0;
        }
		else
		{ // Running
            stancenum = RUNNING;
			thisclient->Status->Stance = RUNNING;
        }
	}
    else
		thisclient->Status->Stance = stancenum;
	if(!thisclient->Status->CanMove)
	   thisclient->Status->Stance = RUNNING;
	thisclient->Stats->Move_Speed = thisclient->GetMoveSpeed( );
	BEGINPACKET( pak, 0x782 );
	ADDWORD( pak, thisclient->clientid );
	ADDBYTE( pak, thisclient->Status->Stance );
	ADDWORD( pak, thisclient->Stats->Move_Speed );
	SendToVisible( &pak, thisclient );       
            
	return true;
}

// Spawn Drop
bool CWorldServer::pakSpawnDrop( CPlayer* thisclient, CDrop* thisdrop )
{
	BEGINPACKET( pak, 0x7a6 );
	ADDFLOAT( pak, thisdrop->pos.x*100 );
	ADDFLOAT( pak, thisdrop->pos.y*100 );
	if (thisdrop->type==1) 
    {
		// -- ZULY --
		ADDDWORD( pak, 0xccccccdf );
		ADDDWORD( pak, thisdrop->amount );
        ADDDWORD( pak, 0xcccccccc );
            ADDWORD ( pak, 0xcccc );
		ADDWORD( pak, thisdrop->clientid );
		ADDWORD( pak, 0x0000 );
		ADDWORD( pak, 0x5f90 );
	}
    else 
    {
		// -- ITEM --
		ADDDWORD( pak, BuildItemHead( thisdrop->item ) );
		ADDDWORD( pak, thisdrop->amount );
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );
		ADDWORD( pak, thisdrop->clientid );
		ADDDWORD( pak, BuildItemData( thisdrop->item ) );
 
		
	}
	thisclient->client->SendPacket( &pak );
	return true;
}

// Player Left
void CWorldServer::pakClearUser( CPlayer* thisclient )
{
	BEGINPACKET( pak, 0x794 );
	ADDWORD( pak, thisclient->clientid );
	SendToVisible( &pak, thisclient, false );			
}

// Drop items on map
bool CWorldServer::pakDoDrop( CPlayer* thisclient, CPacket* P )
{
	BYTE itemid = GETBYTE((*P), 0x0);
	if(!CheckInventorySlot(thisclient, itemid ))
	   return false;
	DWORD amount = GETDWORD((*P), 0x1);
	if (itemid == 0) 
    {
		if ( amount<1 ) return true;
		if ( thisclient->CharInfo->Zulies < amount ) return true;
		CDrop* thisdrop = new CDrop;
		assert(thisdrop);
		thisdrop->clientid = GetNewClientID();
		thisdrop->type = 1; // ZULY
		thisdrop->pos = RandInCircle( thisclient->Position->current, 3 );
		thisdrop->posMap = thisclient->Position->Map;
		thisdrop->droptime = time(NULL);
		thisdrop->amount = amount;
		thisdrop->owner = 0;
		CMap* map = MapList.Index[thisdrop->posMap];
		map->AddDrop( thisdrop );
		thisclient->CharInfo->Zulies -= amount;
		BEGINPACKET( pak, 0x71d );
		ADDQWORD( pak, thisclient->CharInfo->Zulies );
		thisclient->client->SendPacket( &pak );
	}
    else
    {
        // fixed by tomciaaa [item count drop correctly and dissaper from inventory]		
        bool flag = false;
        if(thisclient->items[itemid].itemtype >=10 && thisclient->items[itemid].itemtype <= 13)
        {
            if(thisclient->items[itemid].count<amount) return true;
           thisclient->items[itemid].count -= amount;
           flag= (thisclient->items[itemid].count <= 0);
        }
        else
        {
           flag= true;
        }		
		CDrop* thisdrop = new CDrop;
		assert(thisdrop);
		thisdrop->clientid = GetNewClientID();
		thisdrop->type = 2; // ITEM
		thisdrop->pos = RandInCircle( thisclient->Position->current, 3 );
		thisdrop->posMap = thisclient->Position->Map;
		thisdrop->droptime = time(NULL);
		thisdrop->amount = amount;
		thisdrop->item = thisclient->items[itemid];
		thisdrop->item.count = amount;
		thisdrop->owner = 0;
		CMap* map = MapList.Index[thisdrop->posMap];
		map->AddDrop( thisdrop );
		if (flag)
		   ClearItem(thisclient->items[itemid]);
        thisclient->UpdateInventory( itemid );
	}
	return true;
}

// Picks up item
bool CWorldServer::pakPickDrop( CPlayer* thisclient, CPacket* P )
{
    if(thisclient->Shop->open)
        return true;   
    thisclient->RestartPlayerVal( ); 
	WORD dropid = GETWORD((*P), 0x00);
	CDrop* thisdrop = GetDropByID( dropid, thisclient->Position->Map );
	if (thisdrop==NULL) return true;	
	bool flag = false;
    BEGINPACKET( pak, 0x7a7 );
	ADDWORD    ( pak, thisdrop->clientid );	
    CPlayer* dropowner = 0;	
    if( (thisdrop->owner==0 || thisdrop->owner==thisclient->CharInfo->charid || time(NULL)-thisdrop->droptime>=30 ) || ( thisclient->Party->party!=NULL && thisclient->Party->party == thisdrop->thisparty ))	
    {
        if( thisclient->Party->party!=NULL )
        {
            unsigned int dropparty = thisclient->Party->party->Option/0x10;
            if( dropparty == 8 ) // Orderly
            {
                unsigned int num = 0;
                bool dpflag = false;
                if( thisdrop->type==1 )
                {
                    num = thisclient->Party->party->LastZulies;
                    thisclient->Party->party->LastZulies++;
                    if( thisclient->Party->party->LastZulies>=thisclient->Party->party->Members.size() )
                        thisclient->Party->party->LastZulies = 0;                    
                }
                else
                if( thisdrop->type==2 )
                {
                    num = thisclient->Party->party->LastItem;
                    thisclient->Party->party->LastItem++;
                    if( thisclient->Party->party->LastItem>=thisclient->Party->party->Members.size() )                    
                        thisclient->Party->party->LastItem = 0;  
                }
                unsigned int n = 0;
                while( !dpflag )
                {
                    n++;
                    if( num>=thisclient->Party->party->Members.size() )
                        num = 0;
                    dropowner = thisclient->Party->party->Members.at( num );
                    if( dropowner == NULL )
                        num++;
                    else
                    {
                        dpflag = true;
                        num++;
                    }
                    if(n>20) // not founded yet? >.>
                    {
                        dropowner = thisclient;
                        dpflag = true;
                    }
                }
            }
            else // Equal Loot
            {
                dropowner = thisclient;
            }
        }
        else
        {
            dropowner = thisclient;
        }
        if( thisdrop->type == 1 ) //Zulie
        {
		    dropowner->CharInfo->Zulies += thisdrop->amount;
		    ADDWORD( pak, 0 );
		    ADDBYTE( pak, 0 );
		    ADDDWORD( pak, 0xccccccdf );
		    ADDDWORD( pak, thisdrop->amount );
                ADDDWORD( pak, 0xcccccccc );
                ADDWORD ( pak, 0xcccc );
		    dropowner->client->SendPacket( &pak );
		    flag = true;            
        }
        else
        if( thisdrop->type == 2 ) // Item
        {
            unsigned int type = UseList.Index[thisdrop->item.itemnum]->type;
            if (type == 320 && thisdrop->item.itemtype == 10) {
                RESETPACKET( pak,0x7a3 );
                ADDWORD    ( pak, dropowner->clientid );
                ADDWORD    ( pak, thisdrop->item.itemnum );
                SendToVisible( &pak, dropowner );
                flag = true;
            } else {

            unsigned int tempslot = dropowner->AddItem( thisdrop->item );
            if(tempslot!=0xffff)// we have slot
            {
                unsigned int slot1 = tempslot;
                unsigned int slot2 = 0xffff;
                if(tempslot>MAX_INVENTORY)
                {
                    slot1 = tempslot/1000;
                    slot2 = tempslot%1000;
                }
                ADDBYTE    ( pak, 0x00 );
                ADDBYTE    ( pak, slot1 );
                ADDBYTE    ( pak, 0x00 );
                ADDDWORD   ( pak, BuildItemHead( dropowner->items[slot1] ) );
                ADDDWORD   ( pak, BuildItemData( dropowner->items[slot1] ) );
                ADDDWORD( pak, 0x00000000 );
                ADDWORD ( pak, 0x0000 );   
              
                dropowner->client->SendPacket( &pak );  
                dropowner->UpdateInventory( slot1, slot2 ); 
                flag = true;                                                                
            }
            else
            {
                ADDBYTE    (pak, 0x03);
                dropowner->client->SendPacket(&pak);                     
            }
        }       
        }          
    }
	else
	{
		ADDBYTE    (pak, 0x02);
		thisclient->client->SendPacket(&pak);     
    }
	if( flag )
	{
        if( thisclient->Party->party!=NULL )
        {
            unsigned int dropparty = thisclient->Party->party->Option/0x10;
            if( dropparty == 8 )
            {
                BEGINPACKET( pak, 0x7d3 );
                ADDWORD    ( pak, dropowner->clientid );   
                if( thisdrop->type == 1 )
                {
                    ADDDWORD( pak, 0xccccccdf );
        		    ADDDWORD( pak, thisdrop->amount );
                    ADDDWORD( pak, 0xcccccccc );
                    ADDWORD ( pak, 0xcccc );                    
                }                
                else             
                if( thisdrop->type == 2 )
                {
                    ADDDWORD   ( pak, BuildItemHead( thisdrop->item ) );
                    ADDDWORD   ( pak, BuildItemData( thisdrop->item ) );
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );   
                    
                }
                thisclient->Party->party->SendToMembers( &pak, dropowner );                
            }
        }        
		CMap* map = MapList.Index[thisdrop->posMap];
		pthread_mutex_lock( &map->DropMutex );
		map->DeleteDrop( thisdrop );        
		pthread_mutex_unlock( &map->DropMutex );
    }    
	return true;
}

// Changes equipment
bool CWorldServer::pakChangeEquip( CPlayer* thisclient, CPacket* P )
{
    if(thisclient->Shop->open)
        return true;    
	WORD srcslot = GETWORD((*P), 0);
	WORD destslot = GETWORD((*P), 2);
	if(!CheckInventorySlot(thisclient, srcslot))
	   return false;
	if(!CheckInventorySlot(thisclient, destslot))
	   return false;	
    if (destslot < 13 && GServer->EquipList[thisclient->items[srcslot].itemtype].Index[thisclient->items[srcslot].itemnum]->level > thisclient->Stats->Level)
       return true;
	if( destslot==0 ) destslot = thisclient->GetNewItemSlot( thisclient->items[srcslot] );
	if( destslot==0xffff ) return true;
    if( srcslot==7 || destslot==7 )
    {
        // clear the buffs if we change the weapon
        for(UINT i=0;i<15;i++)
        {
            switch(thisclient->MagicStatus[i].Buff)
            {
                case A_ATTACK:
                case A_ACCUR:
                case A_HASTE:
                case A_CRITICAL:
                    thisclient->MagicStatus[i].Duration = 0;
                break;
            }
        } 
    }
	CItem tmpitm = thisclient->items[srcslot];
	thisclient->items[srcslot] = thisclient->items[destslot];
	thisclient->items[destslot] = tmpitm;
	thisclient->UpdateInventory( srcslot, destslot );
	BEGINPACKET( pak, 0x7a5 );     
	ADDWORD    ( pak, thisclient->clientid );
	ADDWORD    ( pak, srcslot );	
	ADDWORD    ( pak, thisclient->items[srcslot].itemnum );	
	ADDWORD    ( pak, BuildItemRefine( thisclient->items[srcslot] ) );
	ADDWORD    ( pak, thisclient->Stats->Move_Speed );
	SendToVisible( &pak, thisclient );	
    if( srcslot==7 || destslot==7 )
    {
        // if is two hand weapon, we have to check if have shield and unequip it            
        UINT weapontype = EquipList[WEAPON].Index[thisclient->items[7].itemnum]->type;   
        switch(weapontype)
        {
            case TWO_HAND_SWORD:
            case SPEAR:
            case TWO_HAND_AXE:
            case BOW:
            case GUN:
            case LAUNCHER:
            case STAFF:
            case KATAR:
            case DOUBLE_SWORD:
            case DUAL_GUN:
            {
                if(thisclient->items[8].itemnum!=0)
                { // we should unequip the shield
                    UINT newslot = thisclient->GetNewItemSlot( thisclient->items[8] );
                    if(newslot==0xffff)
                    {
                        // we have no slot for the shield, we have to change back the items
                        tmpitm = thisclient->items[destslot];
                        thisclient->items[destslot] = thisclient->items[srcslot];            
                        thisclient->items[srcslot] = tmpitm;
                        SendSysMsg( thisclient, "You don't have free slot" );           
                        return true;
                    }       
                    thisclient->items[newslot] = thisclient->items[8];
                    ClearItem( thisclient->items[8] );  
                    thisclient->UpdateInventory( newslot, 8 );                      
                    RESETPACKET( pak, 0x7a5 );
                    ADDWORD    ( pak, thisclient->clientid );
                    ADDWORD    ( pak, 8 );
                    ADDWORD    ( pak, thisclient->items[8].itemnum );
                    ADDWORD    ( pak, BuildItemRefine( thisclient->items[8] ) );
                    ADDWORD    ( pak, thisclient->Stats->Move_Speed );
                    SendToVisible( &pak, thisclient );                                                           
                }
            }
            break;
        }            
    }
    thisclient->SetStats( );
	return true;
}

// Construct a cart/change parts
bool CWorldServer::pakChangeCart( CPlayer* thisclient, CPacket* P )
{
    if(thisclient->Shop->open)
        return true;    
	WORD cartslot = GETWORD((*P), 0);
	WORD srcslot = GETWORD((*P), 0) + 135;
	WORD destslot = GETWORD((*P), 2);
	if(!CheckInventorySlot( thisclient, srcslot ))
	   return false;
	if(!CheckInventorySlot( thisclient, destslot ))
	   return false;	   
	if( destslot == 0 ) destslot=thisclient->GetNewItemSlot( thisclient->items[srcslot] );
	if( destslot == 0xffff ) return true;

	CItem tmpitm = thisclient->items[srcslot];
	thisclient->items[srcslot] = thisclient->items[destslot];
	thisclient->items[destslot] = tmpitm;

	BEGINPACKET( pak, 0x7ca );     
	ADDWORD    ( pak, thisclient->clientid );
	ADDWORD    ( pak, cartslot);							
	ADDWORD    ( pak, thisclient->items[srcslot].itemnum);
	ADDWORD    ( pak, BuildItemRefine( thisclient->items[srcslot] ) );
	ADDWORD    ( pak, thisclient->Stats->Move_Speed );
	SendToVisible( &pak, thisclient );

    thisclient->UpdateInventory( srcslot, destslot );	
	thisclient->Stats->Move_Speed = thisclient->GetMoveSpeed( );
	return true;
}

// Start attacking a monster or player
bool CWorldServer::pakStartAttack( CPlayer* thisclient, CPacket* P )
{
    if(!thisclient->CanAttack( )) return true;                              
    WORD clientid = GETWORD((*P),0x00);   
    if ( thisclient->Battle->target == clientid ) return true; 

    int weapontype = EquipList[WEAPON].Index[thisclient->items[7].itemnum]->type;
    if( weapontype == 231 && thisclient->items[132].count<1 ) 
        return true;
    else
    if( weapontype == 232 && thisclient->items[133].count<1 )
        return true;
    else
    if( weapontype == 233 && thisclient->items[134].count<1 )
        return true;
    else
    if( weapontype == 271 && thisclient->items[132].count<1 )
        return true;    
        
    CMap* map = MapList.Index[thisclient->Position->Map];
    CCharacter* character = map->GetCharInMap( clientid );
    if(character==NULL) return true;
    if(character->IsMonster( ))
    {
        if(map->allowpvp==0 && character->IsSummon( )) return true;
        if(!character->IsSummon())
            character->AddDamage( (CCharacter*)thisclient, 0 );
    }    	
	if(character->IsMonster())
	{
	   // SET MONSTER HEALTH
	   BEGINPACKET( pak, 0x79f );
	   ADDWORD    ( pak, character->clientid );
	   ADDDWORD   ( pak, character->Stats->HP );
	   thisclient->client->SendPacket( &pak );
    }
    thisclient->StartAction( character, NORMAL_ATTACK, 0 ); 
    thisclient->Battle->contatk = true; 	
	return true;
}

// Goto next map through gate
bool CWorldServer::pakGate( CPlayer* thisclient, CPacket* P )
{
    thisclient->Session->inGame = false;	    
	CTeleGate* thisgate = GetTeleGateByID( GETWORD((*P), 0x00) );
    fPoint position;
    UINT map = 0;	
	if( thisgate==NULL )
    {
		map = 61;
		position.x = 5800.00; 
        position.y = 5200.00;
	}
    else
    {
		map = thisgate->destMap;
		position = thisgate->dest;
	}
    MapList.Index[map]->TeleportPlayer( thisclient, position );
	return true;
}

// Emotions
bool CWorldServer::pakDoEmote( CPlayer* thisclient, CPacket* P )
{
    if(thisclient->Shop->open)
        return true;    
    thisclient->RestartPlayerVal( );
	BEGINPACKET( pak, 0x781 );
	ADDWORD( pak, GETWORD((*P), 0) );
	ADDWORD( pak, 0x8000 );
	ADDWORD( pak, thisclient->clientid );
	SendToVisible( &pak, thisclient );
	return true;
}

// This function is for increasing your stats
bool CWorldServer::pakAddStats( CPlayer* thisclient, CPacket* P )
{
	BYTE statid = GETBYTE((*P), 0);
	int statval = -1;
	int nstatval = 0;
	switch(statid) 
    {
		case 0: nstatval=(int)floor((float)thisclient->Attr->Str/5); break;
		case 1: nstatval=(int)floor((float)thisclient->Attr->Dex/5); break;
		case 2: nstatval=(int)floor((float)thisclient->Attr->Int/5); break;
		case 3: nstatval=(int)floor((float)thisclient->Attr->Con/5); break;
		case 4: nstatval=(int)floor((float)thisclient->Attr->Cha/5); break;
		case 5: nstatval=(int)floor((float)thisclient->Attr->Sen/5); break;
	}
	if (thisclient->CharInfo->StatPoints>=nstatval && nstatval < ((Config.MaxStat/5)+1)) 
    {
		switch(statid) 
        {
			case 0: statval=++thisclient->Attr->Str; break;
			case 1: statval=++thisclient->Attr->Dex; break;
			case 2: statval=++thisclient->Attr->Int; break;
			case 3: statval=++thisclient->Attr->Con; break;
			case 4: statval=++thisclient->Attr->Cha; break;
			case 5: statval=++thisclient->Attr->Sen; break;
		}
	}
	if (statval>0) 
    {
		thisclient->CharInfo->StatPoints-=nstatval;
		BEGINPACKET( pak, 0x7a9 );
		ADDBYTE( pak, statid );
		ADDWORD( pak, (unsigned short)statval );
		thisclient->client->SendPacket( &pak );
	}
	thisclient->SetStats( );
	return true;
}

// Normal Chat
bool CWorldServer::pakNormalChat( CPlayer* thisclient, CPacket* P )
{
	if (P->Buffer[0]=='/') 
    {
		return pakGMCommand( thisclient, P );
        //std::cout << thisclient->CharInfo->charname << ": " << P->Buffer << "\n";            
                   
    }
    else
    {
		BEGINPACKET( pak, 0x783 );
		ADDWORD( pak, thisclient->clientid );
		ADDSTRING( pak, P->Buffer );
		ADDBYTE( pak, 0 );
		SendToVisible( &pak, thisclient );
	}
	return true;
};

// Exit rose
bool CWorldServer::pakExit( CPlayer* thisclient, CPacket* P )
{
	BEGINPACKET( pak, 0x707 );
	ADDWORD( pak, 0 );
	thisclient->client->SendPacket( &pak );
	thisclient->client->isActive = false;
	return true;
}

// move skill to function keys
bool CWorldServer::pakMoveSkill ( CPlayer* thisclient, CPacket* P )
{
	BYTE slotid = GETBYTE((*P), 0x0 );
	WORD itemid = GETWORD((*P), 0x1 );
	if(slotid>47) return true;
	thisclient->quickbar[slotid] = itemid; 
	BEGINPACKET( pak, 0x7aa );
	ADDBYTE( pak, slotid ); 
	ADDWORD( pak, itemid );
	thisclient->client->SendPacket( &pak );
	return true;
}

// User is checking up on the server
bool CWorldServer::pakPing ( CPlayer* thisclient, CPacket* P )
{
	BEGINPACKET( pak, 0x0700 );
	ADDWORD( pak, 0 );
	thisclient->client->SendPacket( &pak );
	return true;
}

// GameGuard
bool CWorldServer::pakGameGuard ( CPlayer* thisclient, CPacket* P )
{
	thisclient->lastGG = clock();
	return true;
}

 
// Changing Respawn for a client
bool CWorldServer::pakChangeRespawn( CPlayer* thisclient, CPacket* P )
{
        //player position (map)
       unsigned int spot = 0;
        switch(thisclient->Position->Map)
        {
            case 22:  //AP
            {
                spot = 1;
                break;
            }           
            case 1:  //Zant
            {
                spot = 2;
                break;
            }           
            case 2:  //Junon
            {
                spot = 4;
                break;
            }           
            case 51:  //Eucar
            {
                spot = 5;
                break;
            }           
            case 61:  //Xita
            {
                spot = 68;
                break;
            }           
        }   
 
                thisclient->Position->saved = spot;
    return true;
}
 
// User Died
bool CWorldServer::pakUserDied ( CPlayer* thisclient, CPacket* P )
{               
    thisclient->Session->inGame = false;
    BYTE respawn = GETBYTE((*P),0);
    //1 - Current / 2 - save point    
    CMap* map = MapList.Index[thisclient->Position->Map];
    CRespawnPoint* thisrespawn = NULL;
    if(respawn==1)
    {
        thisrespawn = map->GetNearRespawn( thisclient );    
    }
    else
    {
        thisrespawn = GetRespawnByID( thisclient->Position->saved );
    }
    thisclient->Stats->HP = thisclient->Stats->MaxHP * 10 / 100;
    if(thisrespawn!=NULL)
    {        
             /// geo edit for saved town warp // 29 sep 07
        map = MapList.Index[thisrespawn->destMap];
        map->TeleportPlayer( thisclient, thisrespawn->dest, false );
    }    else
    {
        fPoint coord;
        coord.x = 5200;
        coord.y = 5200;
        MapList.Index[2]->TeleportPlayer( thisclient, coord, false );        
    }  
	for(unsigned int i=0;i<30;i++)
	{	// Clean Buffs	   
        thisclient->MagicStatus[i].Duration = 0;
    }	    	
	return true;
}

// Shouting
bool CWorldServer::pakShout ( CPlayer* thisclient, CPacket* P )
{
	BEGINPACKET(pak, 0x0785);
	ADDSTRING  ( pak, thisclient->CharInfo->charname );
	ADDBYTE    ( pak, 0 );
	ADDSTRING  ( pak, &P->Buffer[0] );
	ADDBYTE    ( pak, 0 );
	SendToMap  ( &pak, thisclient->Position->Map );

	return true;
}

// Whispering
bool CWorldServer::pakWhisper ( CPlayer* thisclient, CPacket* P )
{
	char msgto[17];
	memset( &msgto, '\0', 17 );
	strncpy( msgto, (char*)&(*P).Buffer[0],16 );
	CPlayer* otherclient = GetClientByCharName( msgto );
	if(otherclient!=NULL)
	{
	   BEGINPACKET( pak, 0x0784 );
	   ADDSTRING( pak, thisclient->CharInfo->charname );
	   ADDBYTE( pak, 0 );
	   ADDSTRING( pak, &P->Buffer[strlen(msgto)+1] );
	   ADDBYTE( pak, 0 );
	   otherclient->client->SendPacket( &pak );
	   return true;
	}
	BEGINPACKET( pak, 0x0784 );
	ADDSTRING( pak, msgto );
	ADDBYTE( pak, 0 );
	ADDBYTE( pak, 0 );
	thisclient->client->SendPacket( &pak );
	return true;
}

// Return to Char Select
bool CWorldServer::pakCharSelect ( CPlayer* thisclient, CPacket* P )
{
   	thisclient->savedata();
  	if(!thisclient->Session->inGame) return true;
    thisclient->Session->isLoggedIn = false;
    if(thisclient->client!=NULL) thisclient->client->isActive = false;
    //send packet to change messenger status (offline)
   	BEGINPACKET( pak, 0x7e1 );
   	ADDBYTE    ( pak, 0xfa ); 
   	ADDWORD    ( pak, thisclient->CharInfo->charid );
   	ADDBYTE    ( pak, 0x00 );
   	cryptPacket( (char*)&pak, cct );
   	send( csock, (char*)&pak, pak.Size, 0 );  
   	pakClearUser( thisclient );
   	ClearClientID( thisclient->clientid );	    
    thisclient->Saved = true;
	RESETPACKET( pak, 0x505 );
	ADDDWORD( pak, thisclient->Session->userid );
	cryptPacket( (char*)&pak, cct );
	send( csock, (char*)&pak, pak.Size, 0 );
	return true;
}

// Buy from NPC (usual and Clan Shop).
//2do: Count and check Z and reward points BEFORE giving the items...
bool CWorldServer::pakNPCBuy ( CPlayer* thisclient, CPacket* P )
{        
    if(thisclient->Shop->open)
        return true;
    WORD NPC = GETWORD((*P), 0);
	CNPC* thisnpc = GetNPCByID( NPC, thisclient->Position->Map );
	if( thisnpc==NULL )  
	   return true;
	   
	BYTE buycount = GETBYTE((*P), 2);
	BYTE sellcount = GETBYTE((*P), 3);
	
     //Clan Shop case...
     Log(MSG_INFO,"Buying /selling from NPC %i",thisnpc->npctype);
     bool is_clanshop=false;
     if (thisnpc->npctype==1752)
     {
        is_clanshop=true;
        
        //impossible to buy from clan shop if you don't have a clan
        if (thisclient->Clan->clanid==0)
        {
            return true;
        }
        
     }
     	
	BYTE ncount = 0;
	BEGINPACKET( pak, 0x717 );
	ADDQWORD   ( pak, thisclient->CharInfo->Zulies );
	ADDBYTE    ( pak, 0x00 );
	for (int i=0; i<buycount; i++) 
    {   
		BYTE tabid = GETBYTE((*P), 8+(i*4));
		BYTE itemid = GETBYTE((*P), 9+(i*4));
		WORD count = GETWORD((*P), 10+(i*4));
		unsigned sellid = 0;
		switch(tabid)
		{
            case 0:
                sellid = thisnpc->thisnpc->tab1;
            break;
            case 1:
                sellid = thisnpc->thisnpc->tab2;
            break;
            case 2:
                sellid = thisnpc->thisnpc->tab3;
            break;
            case 3:
                sellid = thisnpc->thisnpc->specialtab;
            break;            
            default:
                sellid = 0;
        }	
        if(SellList.Index[sellid]==NULL) // invalid tab
            continue;	
		int tmpcount = count;
		while (tmpcount>0) 
        {
			CItem thisitem;
			thisitem.itemnum = SellList.Index[sellid]->item[itemid] % 1000;
			thisitem.itemtype = SellList.Index[sellid]->item[itemid] / 1000;
			unsigned newslot = thisclient->GetNewItemSlot( thisitem );
			if ( newslot == 0xffff ) { ncount-=1; break; }
			int thisslotcount = 999 - thisclient->items[newslot].count;
			if ( thisslotcount > tmpcount ) thisslotcount=tmpcount;
			tmpcount -= thisslotcount;
			thisitem.count = thisslotcount + thisclient->items[newslot].count;
			thisitem.refine = 0;
			thisitem.durability = 35;
			thisitem.lifespan = 100;
			thisitem.appraised = true;
			thisitem.socketed = false;
			thisitem.stats = 0;
			thisitem.gem = 0;
			
			//checking money / reward points now...
			/*
			thisclient->items[newslot] = thisitem;
			
			ADDBYTE  ( pak, newslot );
			ADDDWORD ( pak, BuildItemHead( thisclient->items[newslot] ) );
			ADDDWORD ( pak, BuildItemData( thisclient->items[newslot] ) );
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );   
            */
            
    		switch(thisitem.itemtype)
    		{			
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 8:
                case 9:
                case 14:
                {			
                    float price = 0;
                    UINT quality = 0;
                    UINT bprice = 0;                    
                    if(thisitem.itemtype<10 )
                    {
                        quality = EquipList[thisitem.itemtype].Index[thisitem.itemnum]->quality;
                        bprice = EquipList[thisitem.itemtype].Index[thisitem.itemnum]->price;
                    }
                    else
                    {
                        quality = PatList.Index[thisitem.itemnum]->quality;
                        bprice = PatList.Index[thisitem.itemnum]->price;
                    }     
                    price = quality;
                    price += 0x32; // town rate
                    price *= bprice;
                    price += 0.5;
                    price /= 100;
                    price = (float)round(price);            
                    Log( MSG_WARNING, "Item bought: itemnum %i, itemtype %i, itemcount %i, price %0.0f", thisitem.itemnum, thisitem.itemtype, thisitem.count, price);                                                            
                    
                    if (is_clanshop)
                    {
                        if (thisclient->CharInfo->rewardpoints<(long int) price)
                        {
                          Log(MSG_HACK, "Not enough reward points player %s, have %u, need %u",thisclient->CharInfo->charname,thisclient->CharInfo->rewardpoints,(long int) price);                                    
                          return true;
                        }
                        thisclient->CharInfo->rewardpoints -= (long int) price;
                    }
                    else
                    {
                        if (thisclient->CharInfo->Zulies<(long int)price)
                        {
                          Log(MSG_HACK, "Not enough Zuly player %s, have %li, need %li",thisclient->CharInfo->charname,thisclient->CharInfo->Zulies,(long int) price);                                    
                          return true;
                        }
                        thisclient->CharInfo->Zulies -= (long int)price;
                    }
                      
                }     
                break;
                case 10:                
                case 12:
                {
                    // this values should be the same than packet 753
                    BYTE values[11] = {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32};
                    UINT type = 0;
                    UINT bprice = 0;
                    UINT pricerate = 0;
                    UINT pvalue = 0;
                    if(thisitem.itemtype==10)
                    {
                        type = UseList.Index[thisitem.itemnum]->type;
                        bprice = UseList.Index[thisitem.itemnum]->price;
                        pricerate = UseList.Index[thisitem.itemnum]->pricerate;
                        pvalue = UseList.Index[thisitem.itemnum]->pricevalue;
                    }
                    else
                    {
                        type = NaturalList.Index[thisitem.itemnum]->type;
                        bprice = NaturalList.Index[thisitem.itemnum]->price;
                        pricerate = NaturalList.Index[thisitem.itemnum]->pricerate;
                        pvalue = NaturalList.Index[thisitem.itemnum]->pricevalue;
                    }                    
                    unsigned int value = 0;
                    float price = 0;
                    bool flag;
                    if(type<421)
                        if(type<311)
                            flag = false;
                        else
                            if(type>312)
                                flag = false;
                            else
                                flag = true;
                    else
                        if(type<=428)
                            flag = true;
                        else
                            if(type<311)
                                flag = false;
                            else
                                flag = true;                   
                    if(flag)
                    {
                        value = pvalue;        
                        if(value>=11)
                            value ^= 0xffffffff;
                        else
                            value = values[value];     
                        value -= 0x32;      
                        value *= pricerate;   
                        price = value * 0.001;
                        price += 1;     
                        price *= bprice;                                                                                                                 
                        price += 0.5;
                        price = (float)floor(price);
                        Log( MSG_WARNING, "Item bought: itemnum %i, itemtype %i, itemcount %i, price %0.0f", thisitem.itemnum, thisitem.itemtype, thisitem.count, price);                                                            
                        
                        if (is_clanshop)
                        {
                            if (thisclient->CharInfo->rewardpoints<(long int) price*count)
                            {
                              Log(MSG_HACK, "Not enough reward points player %s, have %u, need %u",thisclient->CharInfo->charname,thisclient->CharInfo->rewardpoints,(long int) price*count);                                    
                              return true;
                            }
                            thisclient->CharInfo->rewardpoints -= (long int) price*count;
                        }
                        else
                        {
                            if (thisclient->CharInfo->Zulies<(long int)price*count)
                            {
                              Log(MSG_HACK, "Not enough Zuly player %s, have %li, need %li",thisclient->CharInfo->charname,thisclient->CharInfo->Zulies,(long int)price*count);                                    
                              return true;
                            }
                            thisclient->CharInfo->Zulies -= (long int)price*count;
                        }
                        
                    }
                    else
                    {
                        float price = pricerate;
                        unsigned int value = 0x61 - 0x32; // misc rate - 0x32
                        price *= value;                
                        price *= 0.001;
                        price += 1; 
                        price *= bprice;                                                                                   
                        price += 0.5;
                        price = (float)floor(price);          
                		Log( MSG_WARNING, "Item bought: itemnum %i, itemtype %i, itemcount %i, price %0.0f", thisitem.itemnum, thisitem.itemtype, thisitem.count, price);                                                                    			
                    	
                        if (is_clanshop)
                        {
                            if (thisclient->CharInfo->rewardpoints<(long int) price*count)
                            {
                              Log(MSG_HACK, "Not enough reward points player %s, have %u, need %u",thisclient->CharInfo->charname,thisclient->CharInfo->rewardpoints,(long int) price*count);                                    
                              return true;
                            }
                            thisclient->CharInfo->rewardpoints -= (long int) price*count;
                        }
                        else
                        {
                            if (thisclient->CharInfo->Zulies<(long int)price*count)
                            {
                              Log(MSG_HACK, "Not enough Zuly player %s, have %li, need %li",thisclient->CharInfo->charname,thisclient->CharInfo->Zulies,(long int)price*count);                                    
                              return true;
                            }
                            thisclient->CharInfo->Zulies -= (long int)price*count;
                        }
                                                                                             
                    }
                }   
                break;    
                case 7:
                case 11:
                case 13:
                {
                    float price = 0;
                    UINT bprice = 0;
                    UINT pricerate;
                    if(thisitem.itemtype==7)
                    {
                        pricerate = EquipList[7].Index[thisitem.itemnum]->pricerate;
                        bprice = EquipList[7].Index[thisitem.itemnum]->price;
                    }
                    else
                    {
                        switch(thisitem.itemtype)
                        {
                            case 11:
                                pricerate = JemList.Index[thisitem.itemnum]->pricerate;
                                bprice = JemList.Index[thisitem.itemnum]->price;                                
                            break;
                            case 13:continue;
                        }
                    }     
                    price = pricerate;                               
                    unsigned int value = 0x61 - 0x32; // misc rate - 0x32                    
                    price *= value;                
                    price *= 0.001;
                    price += 1;   
                    price *= bprice;                                              
                    price += 0.5;
                    price = (float)round(price);          
            		Log( MSG_WARNING, "Item bought: itemnum %i, itemtype %i, itemcount %i, price %0.0f", thisitem.itemnum, thisitem.itemtype, thisitem.count, price);                                                                    			
                	                	
                    if (is_clanshop)
                    {
                        if (thisclient->CharInfo->rewardpoints<(long int) price*count)
                        {
                          Log(MSG_HACK, "Not enough reward points player %s, have %u, need %u",thisclient->CharInfo->charname,thisclient->CharInfo->rewardpoints,(long int) price*count);                                    
                          return true;
                        }
                        thisclient->CharInfo->rewardpoints -= (long int) price*count;
                    }
                    else
                    {
                        if (thisclient->CharInfo->Zulies<(long int)price*count)
                        {
                          Log(MSG_HACK, "Not enough Zuly player %s, have %li, need %li",thisclient->CharInfo->charname,thisclient->CharInfo->Zulies,(long int)price*count);                                    
                          return true;
                        }
                          
                        thisclient->CharInfo->Zulies -= (long int)price*count;
                    }
                                                                 
                }
                break;
                default:
                    Log( MSG_WARNING, "Invalid Item Type: %i", thisitem.itemtype );                            
            }

            //We add item at the end when checks done.
			thisclient->items[newslot] = thisitem;
			
			ADDBYTE  ( pak, newslot );
			ADDDWORD ( pak, BuildItemHead( thisclient->items[newslot] ) );
			ADDDWORD ( pak, BuildItemData( thisclient->items[newslot] ) );
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );   
                      
			ncount++;
		}
	}
	
	//refresh Reward points from player if needed
	if (is_clanshop&&buycount>0)
	{
       GServer->pakGMClanRewardPoints(thisclient,thisclient->CharInfo->charname,0);
    }
	
	for (int i=0; i<sellcount; i++) 
    {
		BYTE slotid = GETBYTE((*P), 8+(buycount*4)+(i*3)); 
		WORD count = GETWORD((*P), 9+(buycount*4)+(i*3));
        if (thisclient->items[slotid].count < count)
           return true;
		CItem thisitem = thisclient->items[slotid];
		thisitem.count = count;
		
		if (thisclient->items[slotid].count<thisitem.count)
		{
          Log(MSG_HACK, "[NPC-SELL] Player %s tryes to sell %i [%i:%i], but has only %i",thisclient->CharInfo->charname,thisitem.count,thisitem.itemtype,thisitem.itemnum,thisclient->items[slotid].count);
          return true;
        }
		
		switch(thisitem.itemtype)
		{
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 8:
            case 9:
            case 14:
            {
                float price = 0;
                price = 7.142857E-05F * 5000;
                if(thisitem.itemtype<10)
                {
                    price *= EquipList[thisitem.itemtype].Index[thisitem.itemnum]->price;
                }
                else
                {
                    price *= PatList.Index[thisitem.itemnum]->price;
                }                
                price *= thisitem.durability + 0xc8; 
                price *= 40;                   
                price *= 0xc8 - 0x62; //town rate
                price *= 1.000000E-06;
                price = (float)floor(price);
    			Log( MSG_WARNING, "Item Sold: itemnum %i, itemtype %i, itemcount %i, price %0.0f", thisitem.itemnum, thisitem.itemtype, thisitem.count, price);                                                            
        		thisclient->CharInfo->Zulies += (long int)price*count;
            }
            break;	
            case 10:
            case 12:
            {
                // this values are the same from packet 753
                BYTE values[11] = {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32};
                UINT type = 0;
                UINT bprice = 0;
                UINT pricerate = 0;
                UINT pvalue = 0;
                if(thisitem.itemtype==10)
                {
                   type = UseList.Index[thisitem.itemnum]->type;
                   bprice = UseList.Index[thisitem.itemnum]->price;
                   pricerate = UseList.Index[thisitem.itemnum]->pricerate;
                   pvalue = UseList.Index[thisitem.itemnum]->pricevalue;
                }
                else
                {
                   type = NaturalList.Index[thisitem.itemnum]->type;
                   bprice = NaturalList.Index[thisitem.itemnum]->price;
                   pricerate = NaturalList.Index[thisitem.itemnum]->pricerate;
                   pvalue = NaturalList.Index[thisitem.itemnum]->pricevalue; 
                }
                unsigned int value = 0;
                float price = 0;
                bool flag;
                if(type<421)
                {
                    if(type<311)
                        flag = false;
                    else
                        if(type>312)
                            flag = false;
                        else
                            flag = true;
                }
                else
                {
                    if(type<=428)
                        flag = true;
                    else
                        if(type<311)
                            flag = false;
                        else
                            flag = true;                                                              
                }
                if(flag)
                {           
                    value = pvalue;
                    if(value>=11)
                        value ^= 0xffffffff;
                    else
                        value = values[value];
                    value -= 0x32;
                    value *= pricerate;
                    value += 1000;
                    value *= bprice; 
                    value *= (200 - 0x62); //town rate ( 100)
                    price = value * 5.555555555555556E-06;           
                    price = (float)floor(price);                 
                	Log( MSG_WARNING, "Item Sold: itemnum %i, itemtype %i, itemcount %i, price %0.0f", thisitem.itemnum, thisitem.itemtype, thisitem.count, price);                                                                    			
                    thisclient->CharInfo->Zulies += (long int)price*count;                                                                                                                                                        
                }
                else
                {                   
                    float price = pricerate;
                    unsigned int value = 0x61 - 0x32; // misc rate -0x32
                    price *= value;
                    price += 1000;
                    price *= bprice;                                                                   
                    price *= (200 - 0x62); //town rate ( 100)
                    price *= 5.555555555555556E-06;           
                    price = (float)floor(price);                 
                	Log( MSG_WARNING, "Item Sold: itemnum %i, itemtype %i, itemcount %i, price %0.0f", thisitem.itemnum, thisitem.itemtype, thisitem.count, price);                                                                    			
                    thisclient->CharInfo->Zulies += (long int)price*count;                     
                }                                         
            }
            break;
            case 7:
            case 11:
            case 13:
            {
                float price = 0;
                UINT bprice = 0;
                UINT pricerate = 0;
                switch(thisitem.itemtype)
                {
                    case 7:              
                        pricerate = EquipList[thisitem.itemtype].Index[thisitem.itemnum]->pricerate;
                        bprice = EquipList[thisitem.itemtype].Index[thisitem.itemnum]->price;
                    break;
                    case 11:
                        pricerate = JemList.Index[thisitem.itemnum]->pricerate;
                        bprice = JemList.Index[thisitem.itemnum]->price;
                    break;
                    case 13:continue;//can we sell quest items? :S
                        
                }      
                price = pricerate;     
                price *= 0x61 - 0x32;   // misc rate -0x32   
                price += 1000; 
                price *= bprice;                                  
                price *= (200 - 0x62); //town rate ( 100)
                price *= 5.555555555555556E-06;                  
                price = (float)floor(price);          
        		Log( MSG_WARNING, "Item Sold: itemnum %i, itemtype %i, itemcount %i, price %0.0f", thisitem.itemnum, thisitem.itemtype, thisitem.count, price);                                                                    			
            	thisclient->CharInfo->Zulies += (long int)price*count;                     
            }
            break;
            default:
                Log( MSG_WARNING, "Invalid Item Type: %i", thisitem.itemtype );
        }
        thisclient->items[slotid].count	-= count;
        if( thisclient->items[slotid].count <=0 )	
    		ClearItem( thisclient->items[slotid] );
		ADDBYTE( pak, slotid );
		ADDDWORD( pak, BuildItemHead( thisclient->items[slotid] ) );
		ADDDWORD( pak, BuildItemData( thisclient->items[slotid] ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   

		ncount++;
	}
	SETQWORD( pak, 0, thisclient->CharInfo->Zulies );
	SETBYTE( pak, 8, ncount );
	thisclient->client->SendPacket( &pak );
	return true;
}

// Attack skill
bool CWorldServer::pakStartSkill ( CPlayer* thisclient, CPacket* P )
{
    if( thisclient->Shop->open || thisclient->Status->Stance==DRIVING || thisclient->Status->Mute !=0xff ) 
        return true;    
    fPoint thispoint;
    UINT targetid = GETWORD( (*P), 0 );
    BYTE skillnum = GETBYTE( (*P), 2 );
    if(skillnum>=MAX_SKILL)
    {
        Log( MSG_HACK, "Invalid Skill id %i for %s ", skillnum, thisclient->CharInfo->charname );
        return false;
    }    
    unsigned int skillid = thisclient->cskills[skillnum].id+thisclient->cskills[skillnum].level-1;
    CMap* map = MapList.Index[thisclient->Position->Map];
    CCharacter* character = map->GetCharInMap( targetid );
    if(character==NULL) return true;
    
	if(character->IsMonster())
	{
	   BEGINPACKET( pak, 0x79f );
	   ADDWORD( pak, character->clientid );
	   ADDWORD( pak, character->Stats->HP );
	   ADDWORD( pak, 0 );
	   thisclient->client->SendPacket( &pak );
    }    
	CSkills* thisskill = GetSkillByID( skillid );
	if(thisskill==NULL)
	   return true;
	if(thisskill->target==9 && !character->IsDead())
	{
        ClearBattle( thisclient->Battle );
        return true;
    }    
    if( isSkillTargetFriendly( thisskill ) )
    {
        thisclient->StartAction( character, SKILL_BUFF, skillid );
    }
    else
    {
        thisclient->StartAction( character, SKILL_ATTACK, skillid );        
    }
	return true;
}

// Trade action
bool CWorldServer::pakTradeAction ( CPlayer* thisclient, CPacket* P )
{
    if(thisclient->Shop->open)
        return true;    
	CPacket pak;
	BYTE action = GETBYTE( (*P), 0 );
	thisclient->Trade->trade_target = GETWORD( (*P), 1 );
	CPlayer* otherclient = GetClientByID( thisclient->Trade->trade_target, thisclient->Position->Map );
	if (otherclient==NULL) return true;
	switch(action) 
    {
		case 0:
			// REQUEST TRADE
			RESETPACKET( pak, 0x7c0 );
			ADDBYTE( pak, 0 );
			ADDWORD( pak, thisclient->clientid );
			ADDBYTE( pak, 0 );
			otherclient->client->SendPacket( &pak );
			thisclient->Trade->trade_status=2;
			otherclient->Trade->trade_status=1;
			break;
		case 1:
			// ACCEPT TRADE
			RESETPACKET( pak, 0x7c0 );
			ADDBYTE( pak, 1 );
			ADDWORD( pak, thisclient->clientid );
			ADDBYTE( pak, 0 );
			otherclient->client->SendPacket( &pak );
			thisclient->Trade->trade_status=3;
			otherclient->Trade->trade_status=3;
			for(int i=0; i<11; i++) thisclient->Trade->trade_count[i]=0;
			for(int i=0; i<10; i++) thisclient->Trade->trade_itemid[i]=0;
			for(int i=0; i<11; i++) otherclient->Trade->trade_count[i]=0;
			for(int i=0; i<10; i++) otherclient->Trade->trade_itemid[i]=0;
			break;
		case 3:
			RESETPACKET( pak, 0x7c0 );
			ADDBYTE( pak, 3 );
			ADDWORD( pak, thisclient->clientid );
			ADDBYTE( pak, 0 );
			otherclient->client->SendPacket( &pak );
			thisclient->Trade->trade_target=0;
			otherclient->Trade->trade_target=0;
			thisclient->Trade->trade_status=0;
			thisclient->Trade->trade_status=0;
			break;
		case 4:
			RESETPACKET( pak, 0x7c0 );
			ADDBYTE( pak, 4 );
			ADDWORD( pak, thisclient->clientid );
			ADDBYTE( pak, 0 );
			otherclient->client->SendPacket( &pak );
			thisclient->Trade->trade_status=4;
			break;
		case 6:
			if (thisclient->Trade->trade_status==6)
				thisclient->Trade->trade_status=4;
			else
				thisclient->Trade->trade_status=6;
			if (otherclient->Trade->trade_status==6) 
            {
				RESETPACKET( pak, 0x7c0 );
				// Complete the trade
				ADDBYTE( pak, 6 );
				ADDWORD( pak, otherclient->clientid );
				ADDBYTE( pak, 0 );
				thisclient->client->SendPacket( &pak );
				RESETPACKET( pak, 0x7c0 );
				ADDBYTE( pak, 6 );
				ADDWORD( pak, thisclient->clientid );
				ADDBYTE( pak, 0 );
				otherclient->client->SendPacket( &pak );
				
				// Check if user has enough zuly
                if ( thisclient->CharInfo->Zulies < thisclient->Trade->trade_count[0x0a])
                {
                  Log(MSG_HACK, "[TRADE] Player %s has to trade %li, but has only %li",thisclient->CharInfo->charname,thisclient->Trade->trade_count[0x0a],thisclient->CharInfo->Zulies);
                  return true;
                }
                //LMA: bug... surely...
                //if ( thisclient->CharInfo->Zulies < otherclient->Trade->trade_count[0x0a]) return true;
                if ( otherclient->CharInfo->Zulies < otherclient->Trade->trade_count[0x0a])
                {
                  Log(MSG_HACK, "[TRADE] Player %s has to trade %li, but has only %li",otherclient->CharInfo->charname,otherclient->Trade->trade_count[0x0a],otherclient->CharInfo->Zulies);
                  return true;
                }

				// Update the zuly
				//LMA: anti hack...
				long int zulythis=0;
				long int zulyother=0;
				zulythis=thisclient->CharInfo->Zulies;
				zulyother=otherclient->CharInfo->Zulies;
				
				thisclient->CharInfo->Zulies -= thisclient->Trade->trade_count[0x0a];
				otherclient->CharInfo->Zulies -= otherclient->Trade->trade_count[0x0a];
				thisclient->CharInfo->Zulies += otherclient->Trade->trade_count[0x0a];
				otherclient->CharInfo->Zulies += thisclient->Trade->trade_count[0x0a];
				
				unsigned tucount = 0;
				unsigned oucount = 0;

				// Begin this clients inventory update
				BEGINPACKET( pakt, 0x717 );
				ADDQWORD( pakt, thisclient->CharInfo->Zulies );
				ADDBYTE( pakt, 0 );

				// Begin the other clients inventory update
				BEGINPACKET( pako, 0x717 );
				ADDQWORD( pako, otherclient->CharInfo->Zulies );
				ADDBYTE( pako, 0 );
                
                //LMA: check before actually giving the items to players...
  				for (unsigned i=0; i<10; i++) 
                {
					if(thisclient->Trade->trade_count[i] > 0) 
                    {
						//LMA: anti hack check.
						if(thisclient->items[thisclient->Trade->trade_itemid[i]].count < thisclient->Trade->trade_count[i])
						{
                          Log(MSG_HACK, "[TRADE] Player %s has to trade %i [%i:%i], but has only %i",thisclient->CharInfo->charname,thisclient->Trade->trade_count[i],thisclient->items[thisclient->Trade->trade_itemid[i]].itemtype,thisclient->items[thisclient->Trade->trade_itemid[i]].itemnum,thisclient->items[thisclient->Trade->trade_itemid[i]].count);
          				  thisclient->CharInfo->Zulies=zulythis;
        				  otherclient->CharInfo->Zulies=zulyother;
                          return true;
                        }

					}
					if(otherclient->Trade->trade_count[i] > 0) 
                    {
						//LMA: anti hack check.
						if(otherclient->items[otherclient->Trade->trade_itemid[i]].count<otherclient->Trade->trade_count[i])
						{
                          Log(MSG_HACK, "[TRADE] Player %s has to trade %i [%i:%i], but has only %i",otherclient->CharInfo->charname,otherclient->Trade->trade_count[i],otherclient->items[otherclient->Trade->trade_itemid[i]].itemtype,otherclient->items[otherclient->Trade->trade_itemid[i]].itemnum,otherclient->items[otherclient->Trade->trade_itemid[i]].count);                                                                                                                            
          				  thisclient->CharInfo->Zulies=zulythis;
        				  otherclient->CharInfo->Zulies=zulyother;
                          return true;
                        }
                        						
					}
				}              
            
                //Ok, go, it should be ok now...
				for (unsigned i=0; i<10; i++) 
                {
					if(thisclient->Trade->trade_count[i] > 0) 
                    {
						CItem thisitem = thisclient->items[thisclient->Trade->trade_itemid[i]];
						unsigned newslot = otherclient->GetNewItemSlot( thisitem );
						if(newslot==0xffff) continue;
						
						//LMA: anti hack check.
						if(thisclient->items[thisclient->Trade->trade_itemid[i]].count < thisclient->Trade->trade_count[i])
						{
                          Log(MSG_HACK, "[TRADE] Player %s has to trade %i [%i:%i], but has only %i",thisclient->CharInfo->charname,thisclient->Trade->trade_count[i],thisclient->items[thisclient->Trade->trade_itemid[i]].itemtype,thisclient->items[thisclient->Trade->trade_itemid[i]].itemnum,thisclient->items[thisclient->Trade->trade_itemid[i]].count);
          				  thisclient->CharInfo->Zulies=zulythis;
        				  otherclient->CharInfo->Zulies=zulyother;
                          return true;
                        }
                        
						thisclient->items[thisclient->Trade->trade_itemid[i]].count -= thisclient->Trade->trade_count[i];
						if( thisclient->items[thisclient->Trade->trade_itemid[i]].count<=0)
      						ClearItem(thisclient->items[thisclient->Trade->trade_itemid[i]]);
                        if (otherclient->items[newslot].count > 0)
                            thisitem.count = otherclient->items[newslot].count + thisclient->Trade->trade_count[i];
                        else
                            thisitem.count = thisclient->Trade->trade_count[i];
                        otherclient->items[newslot]=thisitem;
						ADDBYTE( pakt, (unsigned char)thisclient->Trade->trade_itemid[i] );
						ADDDWORD( pakt, BuildItemHead( thisclient->items[thisclient->Trade->trade_itemid[i]] ) );
						ADDDWORD( pakt, BuildItemData( thisclient->items[thisclient->Trade->trade_itemid[i]] ) );
                        ADDDWORD( pakt, 0x00000000 );
                        ADDWORD ( pakt, 0x0000 );  
						ADDBYTE( pako, newslot );
						ADDDWORD( pako, BuildItemHead( otherclient->items[newslot] ) );
						ADDDWORD( pako, BuildItemData( otherclient->items[newslot] ) );
                        ADDDWORD( pako, 0x00000000 );
                        ADDWORD ( pako, 0x0000 );
						tucount++;
						oucount++;
					}
					if(otherclient->Trade->trade_count[i] > 0) 
                    {
						CItem thisitem = otherclient->items[otherclient->Trade->trade_itemid[i]];
						unsigned newslot = thisclient->GetNewItemSlot( thisitem );
						if(newslot==0xffff) continue;
						
						//LMA: anti hack check.
						if(otherclient->items[otherclient->Trade->trade_itemid[i]].count<otherclient->Trade->trade_count[i])
						{
                          Log(MSG_HACK, "[TRADE] Player %s has to trade %i [%i:%i], but has only %i",otherclient->CharInfo->charname,otherclient->Trade->trade_count[i],otherclient->items[otherclient->Trade->trade_itemid[i]].itemtype,otherclient->items[otherclient->Trade->trade_itemid[i]].itemnum,otherclient->items[otherclient->Trade->trade_itemid[i]].count);                                                                                                                            
          				  thisclient->CharInfo->Zulies=zulythis;
        				  otherclient->CharInfo->Zulies=zulyother;
                          return true;
                        }
                        						
						otherclient->items[otherclient->Trade->trade_itemid[i]].count -= otherclient->Trade->trade_count[i];
						if( otherclient->items[otherclient->Trade->trade_itemid[i]].count<=0 )
      						ClearItem( otherclient->items[otherclient->Trade->trade_itemid[i]] );
                        if (thisclient->items[newslot].count > 0)
                            thisitem.count = thisclient->items[newslot].count + otherclient->Trade->trade_count[i];
                        else
                            thisitem.count = otherclient->Trade->trade_count[i];
                        thisclient->items[newslot]=thisitem;					
						ADDBYTE( pako, (unsigned char)otherclient->Trade->trade_itemid[i] );
						ADDDWORD( pako, BuildItemHead( otherclient->items[otherclient->Trade->trade_itemid[i]] ) );
						ADDDWORD( pako, BuildItemData( otherclient->items[otherclient->Trade->trade_itemid[i]] ) );
                        ADDDWORD( pako, 0x00000000 );
                        ADDWORD ( pako, 0x0000 );
						ADDBYTE( pakt, newslot );
						ADDDWORD( pakt, BuildItemHead( thisclient->items[newslot] ) );
						ADDDWORD( pakt, BuildItemData( thisclient->items[newslot] ) );
                        ADDDWORD( pakt, 0x00000000 );
                        ADDWORD ( pakt, 0x0000 );
						tucount++;
						oucount++;
					}
				}

				// Update itemcount and send
				SETBYTE( pakt, 8, tucount );
				SETBYTE( pako, 8, oucount );
				thisclient->client->SendPacket( &pakt );
				otherclient->client->SendPacket( &pako );
				thisclient->Trade->trade_status=0;
				otherclient->Trade->trade_status=0;
			}
			break;
	}

	return true;
}

// Trade action
bool CWorldServer::pakTradeAdd ( CPlayer* thisclient, CPacket* P )
{
    if(thisclient->Shop->open)
        return true;    
	BYTE islot = GETBYTE((*P),0);
	WORD slotid = GETWORD((*P),1);
	DWORD count = GETDWORD((*P),3);
	CPlayer* otherclient = GetClientByID( thisclient->Trade->trade_target, thisclient->Position->Map );
	if (otherclient==NULL) return true;
	BEGINPACKET( pak, 0x7c1 );
	ADDBYTE( pak, islot );	
	if( islot == 0x0a ) 
    {
        if (count > thisclient->CharInfo->Zulies)
           return true;
		ADDDWORD( pak, 0xccccccdf );
		ADDDWORD( pak, count );
        ADDDWORD( pak, 0xcccccccc );
        ADDWORD ( pak, 0xcccc );
		thisclient->Trade->trade_count[islot] = count;
	}
    else
    {
        if(islot>0x0a)
            return false;
		if( count != 0 )
        {
            if(count>thisclient->items[slotid].count)
                return false;
			thisclient->Trade->trade_count[islot] = count;
			thisclient->Trade->trade_itemid[islot] = slotid;
			CItem tmpitem = thisclient->items[slotid]; tmpitem.count = count;
			ADDDWORD( pak, BuildItemHead( tmpitem ) );
			ADDDWORD( pak, BuildItemData( tmpitem ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );  
		}
        else
        {
			thisclient->Trade->trade_count[islot] = 0;
			thisclient->Trade->trade_itemid[islot] = 0;
			ADDDWORD( pak, 0 );
			ADDDWORD( pak, 0 );
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );
		}
	}
	otherclient->client->SendPacket( &pak );
	return true;
}

// Give Quest
bool CWorldServer::pakGiveQuest ( CPlayer* thisclient, CPacket* P )
{    
    BYTE action = GETBYTE((*P),0);
    BYTE questpart = GETBYTE((*P),1);
    DWORD questid = GETDWORD((*P),2);
    /*
    //LMA: not useful anymore.
    if( thisclient->questdebug )
        std::cout << "QuestID: " << questid << std::endl;
    */
    bool qflag = false;
    switch (action)
    {
        case 0x02:
            qflag = thisclient->DelQuest( questid );                
            action = 0x03;            
        break;        
        case 0x03:
            qflag = thisclient->AddQuest( questid );
            action = 0x05;             
        break;
        default:
            Log( MSG_WARNING, "Quest unknown action: %i", action );
    }
    if( qflag )
    {
        BEGINPACKET( pak, 0x730);
        ADDBYTE    (pak, action);
        ADDBYTE    (pak, questpart);  
        ADDDWORD   (pak, questid );  
    	thisclient->client->SendPacket( &pak );    
    }
    return true;
}

// Self skills
bool CWorldServer::pakSkillSelf( CPlayer* thisclient, CPacket* P )
{
    if( thisclient->Shop->open || thisclient->Status->Stance==DRIVING ||
        thisclient->Status->Mute!=0xff || !thisclient->Status->CanCastSkill)
        return true;          
    WORD num = GETWORD((*P),0);
    if(num>=MAX_SKILL)
    {
        Log( MSG_HACK, "Invalid Skill id %i for %s ", num, thisclient->CharInfo->charname );
        return false;
    }
	unsigned int skillid = thisclient->cskills[num].id+thisclient->cskills[num].level-1;
	CSkills* thisskill = GetSkillByID( skillid );
	if(thisskill == NULL)
	   return true;
	unsigned int skilltarget = thisskill->target;
    unsigned int skillrange = thisskill->aoeradius;  
         
	if( thisskill->aoe == 0 )
	{
        thisclient->StartAction( NULL, BUFF_SELF, skillid );                              	        
    }
    else
    {
        if(isSkillTargetFriendly( thisskill ))
        {          
            thisclient->StartAction( NULL, BUFF_AOE, skillid );         
        }
        else
        {
            thisclient->StartAction( NULL, SKILL_AOE, skillid );                    
        }                                 
    }          	
    return true;
}

// Consumible Items
bool CWorldServer::pakUseItem ( CPlayer* thisclient, CPacket* P )
{
    if(thisclient->Shop->open)
        return true;    
    BYTE slot = GETBYTE((*P),0);
    if(!CheckInventorySlot( thisclient, slot))
        return false;
    if( thisclient->items[slot].count<=0 )
        return true;
    CUseInfo* thisuse = GetUseItemInfo( thisclient, slot);    
    if(thisuse == NULL)
    {
        Log(MSG_WARNING,"[%i]Invalid Item, Item: %i. - Type: %i",
        thisclient->clientid, thisclient->items[slot].itemnum, 
        thisclient->items[slot].itemtype);    
        return true;
    }
    bool flag = false;    
    switch(thisuse->usescript)
    {
        case 1: // Food
        {
            thisclient->UsedItem->usevalue = thisuse->usevalue;
            thisclient->UsedItem->usetype = thisuse->usetype;                
            thisclient->UsedItem->userate = 15;
            thisclient->UsedItem->used = 0;                    
            BEGINPACKET( pak,0x7a3 );
            ADDWORD    ( pak, thisclient->clientid );
            ADDWORD    ( pak, thisuse->itemnum );   
            SendToVisible( &pak, thisclient );    
            flag = true;            
        }
        break;
        case 2: // Return Scroll        
        {      
            BEGINPACKET( pak,0x7a3 );
            ADDWORD    ( pak,thisclient->clientid );
            ADDWORD    ( pak, thisuse->itemnum );
            ADDBYTE    ( pak,slot );
            thisclient->client->SendPacket( &pak );
            thisclient->items[slot].count -= 1;        
            if( thisclient->items[slot].count == 0 )        
                ClearItem( thisclient->items[slot] );                           
            fPoint thispoint;
            thispoint.x = floor( thisuse->usevalue/10000 );
            thispoint.y = floor( thisuse->usevalue%10000 );
            TeleportTo ( thisclient, thisuse->usetype, thispoint );                 
            flag = true;            
        }
        break;
        case 3: // Charm Scroll
        {    
	        CSkills* thisskill = GetSkillByID( thisuse->usetype );
            if(thisskill == NULL)
            {
                Log(MSG_WARNING, "Char %s tried to use invalid scroll type: %i", thisclient->CharInfo->charname, thisuse->usetype );
                delete thisuse;
          	    return true;
            }
        	int skilltarget = thisskill->target;
            int skillrange = thisskill->aoeradius;        
        	if( thisskill->aoe == 0 )
        	{
                thisclient->StartAction( NULL,BUFF_SELF,thisuse->usetype );   	        
            }
            else
            {
                if(isSkillTargetFriendly( thisskill ))
                {          
                    thisclient->StartAction( NULL,BUFF_AOE,thisuse->usetype );                       
                }
                else
                {
                    thisclient->StartAction( NULL,SKILL_AOE,thisuse->usetype );                                                                   
                }            
            }   
            flag = true;            
        }
        break;        
        case 4: // Dance Scroll | FireCrackers | Emotion 
        {
            //Start Animation
            BEGINPACKET( pak, 0x7b2 );
            ADDWORD    ( pak, thisclient->clientid );
            ADDWORD    ( pak, thisuse->usevalue );                        
            SendToVisible( &pak, thisclient );
            //Finish Animation
            RESETPACKET( pak, 0x7bb );
            ADDWORD    ( pak, thisclient->clientid );
            SendToVisible( &pak, thisclient );    
            //????
            RESETPACKET( pak, 0x7b9);
            ADDWORD    ( pak, thisclient->clientid);
            ADDWORD    ( pak, thisuse->usevalue );
	        SendToVisible( &pak, thisclient );               
            flag = true;	        
        }
        break;        
        case 5: // Summons
        {
            CNPCData* thisnpc = GetNPCDataByID( thisuse->usevalue );
            if(thisnpc==NULL)
            {                
                Log( MSG_WARNING,"[%i]NPCDATA NOT Founded: %i.", 
                                    thisclient->clientid,thisuse->usevalue );
                delete thisuse;                
                return true;        
            }            
            //Start Animation
            BEGINPACKET( pak, 0x7b2 );
            ADDWORD    ( pak, thisclient->clientid );
            ADDWORD    ( pak, thisuse->usetype );  
            ADDBYTE    ( pak, 6 );                      
            SendToVisible( &pak, thisclient );
            //Finish Animation
            RESETPACKET( pak, 0x7bb );
            ADDWORD    ( pak, thisclient->clientid );
            SendToVisible( &pak, thisclient );    
            //????
            RESETPACKET( pak, 0x7b9);
            ADDWORD    ( pak, thisclient->clientid);
            ADDWORD    ( pak, thisuse->usetype );
	        SendToVisible( &pak, thisclient );   	        
	        // Add our Mob to the mobs list
           	fPoint position = RandInCircle( thisclient->Position->current, 5 );
           	CMap* map = MapList.Index[thisclient->Position->Map];
           	map->AddMonster( thisuse->usevalue, position, thisclient->clientid );              
            flag = true;                        
        }
        break;        
        case 6: // Snowball
        {
            WORD clientid = GETWORD((*P),2);
            fPoint thispoint;
            CMap* map = MapList.Index[thisclient->Position->Map];
            CCharacter* character = map->GetCharInMap( clientid );
            if(character==NULL) return true;
            thisclient->StartAction( character, SKILL_BUFF, thisuse->usevalue ); 
            flag = true;            
        }
        break;    
        case 7: // scrolls [damage,maxhp,...]
        {
	        CSkills* thisskill = GetSkillByID( thisuse->usetype );
            if(thisskill == NULL)
            {
                Log(MSG_WARNING, "Char %s tried to use invalid scroll type: %i", thisclient->CharInfo->charname, thisuse->usetype );
                delete thisuse;
          	    return true;
            }
        	int skilltarget = thisskill->target;
            int skillrange = thisskill->aoeradius;       
        	if( thisskill->aoe == 0 )
        	{
                thisclient->StartAction( NULL, BUFF_SELF, thisuse->usetype );   
            }
            else
            {
                if(isSkillTargetFriendly( thisskill ))
                {          
                    thisclient->StartAction( NULL, BUFF_AOE, thisuse->usetype );                     
                }
                else
                {
                    thisclient->StartAction( NULL, SKILL_AOE, thisuse->usetype );                                                                 
                }            
            }            
            flag = true;
        }
        break;    
        case 10: // Skill Book
        {
            flag = LearnSkill( thisclient, thisuse->usevalue );
        }
        break;        
    }
    if(flag == true)
    {    
        BEGINPACKET( pak,0x7a3 );
        ADDWORD    ( pak, thisclient->clientid );
        ADDWORD    ( pak, thisuse->itemnum );
        ADDBYTE    ( pak, slot );
        thisclient->client->SendPacket( &pak );
        thisclient->items[slot].count -= 1;        
        if( thisclient->items[slot].count <= 0 )        
            ClearItem( thisclient->items[slot] );        
    }    
    delete thisuse;
    return true; 
}

// Level UP Skill
bool  CWorldServer::pakLevelUpSkill( CPlayer *thisclient, CPacket* P )
{       
    WORD pos = GETWORD ((*P),0);
    WORD skill = GETWORD ((*P),2);
    if(pos>=MAX_SKILL)
    {
        Log( MSG_HACK, "Invalid Skill id %i for %s ", pos, thisclient->CharInfo->charname );
        return false;
    }        
    CSkills* thisskill = GetSkillByID( skill );
    if(thisskill==NULL)
        return true;
    if(thisclient->cskills[pos].id!=skill-thisclient->cskills[pos].level)                
        return true;
    if(thisclient->CharInfo->SkillPoints>=thisskill->sp)
    {
       thisclient->CharInfo->SkillPoints -= 1;
	   BEGINPACKET( pak, 0x7b1 );
	   ADDBYTE    ( pak, 0x00);
	   ADDWORD    ( pak, pos);	
	   ADDWORD    ( pak, skill);
	   ADDWORD    ( pak, thisclient->CharInfo->SkillPoints);	 
	   thisclient->client->SendPacket( &pak );       
       thisclient->cskills[pos].level+=1; 
       thisclient->cskills[pos].thisskill = thisskill;
       thisclient->SetStats( );
    }
	return true;
}

// Equip bullets arrows and cannons
bool CWorldServer::pakEquipABC ( CPlayer* thisclient, CPacket* P )
{   
    if(thisclient->Shop->open)
        return true;        
    BYTE itemslot = GETBYTE((*P),0);
    int slot = ((itemslot-32)/4)+72;
    int dest = 0;
    int type = (itemslot-32)%4;
    if(!CheckInventorySlot( thisclient, slot))
        return false;    
    if(itemslot>3)
    {
        dest = type+132;
    }
    else
    {
        slot = itemslot + 132;                
        dest = thisclient->GetNewItemSlot( thisclient->items[slot] );
        if(dest==0xffff)
            return true;
        type=300;
    }
	CItem tmpitm = thisclient->items[slot];
	thisclient->items[slot] =thisclient->items[dest];
	thisclient->items[dest] = tmpitm;
	
	thisclient->UpdateInventory( dest, slot );
    BEGINPACKET( pak, 0x7ab );
    ADDWORD    ( pak, thisclient->clientid );
    unsigned int effect = ( thisclient->items[dest].itemnum * 32 ) + type;
    ADDWORD    ( pak, effect );
    SendToVisible( &pak, thisclient );
    thisclient->Stats->Attack_Power = thisclient->GetAttackPower( );
    return true;
}

///////////////////////////////////////////////////////////////////////////////////
// Craft item  (From RageZone)  (Crafting modifications by: Killerfly)  	//                        
/////////////////////////////////////////////////////////////////////////////////

bool CWorldServer::pakCraft( CPlayer* thisclient, CPacket* P )
{    
	if(thisclient->Shop->open==true)
        return true;
	CItem item;        
	item.count = 1;
	// item durability randomizer
	int lowest = thisclient->Attr->Con/ 10 + 17;
	int highest = thisclient->Attr->Con / 10 + 70;
	int range=(highest-lowest)+1;
	item.durability = lowest+int(range*rand()/(RAND_MAX + 1.0));

	// durability set
	item.itemnum = (GETWORD((*P), 3));
	item.itemtype = (GETBYTE((*P), 0x2));
	item.lifespan = 100; //Its new so 100%
	item.refine = 0;
	
	// stats randomizer
	int changeofstatslow = thisclient->Attr->Sen / 13 + 10;
	int changeofstatshigh = thisclient->Attr->Sen / 13 + 50;
	int changeofstatsrange = (changeofstatshigh-changeofstatslow)+1;
	if (changeofstatslow+int(changeofstatsrange*rand()/(RAND_MAX + 1.0)) > 50){
	int statslowget = 1;
	int statshighget = 256;
	int setstatrange=(statshighget-statslowget)+1;
  	item.stats = statslowget+int(setstatrange*rand()/(RAND_MAX + 1.0));
	item.appraised = 1;              
	}else {
          item.stats = 0;
          item.appraised = 0;
	}
	// stats set
	item.socketed = 0;
	item.gem = 0;
	
	unsigned newslot= thisclient->GetNewItemSlot( item );
	if (newslot !=0xffff)
    {
		//get material used lookup number
		int	materialnumber = 0;
		if(item.itemtype<10)
		{
            materialnumber = EquipList[item.itemtype].Index[item.itemnum]->material;
        }
        else
        {
            switch(item.itemtype)
            {
                case 11:materialnumber = JemList.Index[item.itemnum]->material;break;
                case 14:materialnumber = PatList.Index[item.itemnum]->material;break;
            }
        }		
		int	m = 0;
		for(char used=5; used != 11; used +=2)
        {
            WORD material= GETWORD((*P), used);//gets inventory location
            if (material != 0) {
                if(!CheckInventorySlot( thisclient, material))
                    return false;
                if (thisclient->items[material].count < ProductList.Index[materialnumber]->amount[m])
                    return false;
                UINT clientMat = (thisclient->items[material].itemtype * 1000) + thisclient->items[material].itemnum;
                if (clientMat != ProductList.Index[materialnumber]->item[m]) {
                    Log(MSG_HACK, "Client craft mats don't equal server. Player: %s", thisclient->CharInfo->charname);
                    return false;
                }
                thisclient->items[material].count -= ProductList.Index[materialnumber]->amount[m];
                m++;
                if (thisclient->items[material].count <=0) {
                    ClearItem(thisclient->items[material]);
                }
            }
        }
		thisclient->items[newslot] = item;
        BEGINPACKET( pak, 0x07d8);
        ADDWORD( pak, thisclient->clientid );
        ADDWORD( pak, 0x0100);
        ADDWORD( pak, item.itemnum);// item id not shifted
        thisclient->client->SendPacket(&pak);
     
        RESETPACKET( pak, 0x07af);
        ADDBYTE( pak, 0x00);//00
        ADDWORD( pak, newslot); 
        
        // Make craft bars
        int bar1 = item.durability * 9;                                  
        int bar2 = changeofstatsrange * 9;
        int bar3 = item.durability + changeofstatsrange * 6;
        int bar4 = item.durability + changeofstatsrange + bar3 / 3;                   
        
        ADDWORD( pak, bar1);//progress bar1 0 is empty 0x0400 is full bar
        ADDWORD( pak, bar2);//progress bar2 0 is empty 0x0400 is full bar
        if ((GETWORD((*P),  9))==0){ ADDWORD( pak, 0x99a0);}else{ ADDWORD( pak, bar3);}//progress bar3 0 is empty 0x0400 is full bar
        if ((GETWORD((*P), 11))==0){ ADDWORD( pak, 0x99a0);}else{ ADDWORD( pak, bar4);}//progress bar4 0 is empty 0x0400 is full bar
        ADDDWORD(pak, BuildItemHead(item)); 

     if (item.itemtype == 11){
	     ADDWORD( pak, 0x0001);// amount
	     ADDWORD( pak, 0x0000);
     }
     else{
	     ADDDWORD(pak, BuildItemData(item));  
     }
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );
        int crafting_exp = item.durability + changeofstatsrange * (thisclient->Stats->Level/ 15);
		thisclient->CharInfo->Exp += crafting_exp;//  add exp		
		thisclient->client->SendPacket(&pak);
        RESETPACKET( pak, 0x79b );
        ADDDWORD   ( pak, thisclient->CharInfo->Exp );
        ADDWORD    ( pak, thisclient->CharInfo->stamina );
        ADDWORD    ( pak, 0 );
        thisclient->client->SendPacket( &pak );	
     }
     else
     {
         BEGINPACKET (pak, 0x702); 
         ADDSTRING(pak, "No free slot !"); 
         ADDBYTE(pak, 0); 
         thisclient->client->SendPacket(&pak);
     }
     return true;
}


///////////////////////////////////////////////////////////////////////////////////
// After Craft  (From RageZone)  (if you coded this tell me to add your name)    //                        
///////////////////////////////////////////////////////////////////////////////////
bool CWorldServer::pakModifiedItemDone( CPlayer* thisclient, CPacket* P )
{
     DWORD result = GETDWORD((*P), 0x00 );    
     BEGINPACKET( pak, 0x07d8);
     ADDWORD( pak, thisclient->clientid );
     ADDDWORD( pak, result);
     SendToVisible( &pak , thisclient);
     return true;
     
}

// Aoe Skill
bool CWorldServer::pakSkillAOE( CPlayer* thisclient, CPacket* P)
{
    if( thisclient->Shop->open || thisclient->Status->Stance==DRIVING ||
        thisclient->Status->Mute!=0xff || !thisclient->Status->CanCastSkill) return true;         
    BYTE num = GETBYTE( (*P), 0 );	   
    if(num>=MAX_SKILL)
    {
        Log( MSG_HACK, "Invalid Skill id %i for %s ", num, thisclient->CharInfo->charname );
        return false;
    }    
    unsigned int skillid = thisclient->cskills[num].id+thisclient->cskills[num].level-1;    
    CSkills* thisskill = GetSkillByID( skillid );
    if(thisskill==NULL) return true;             
    if(thisskill->aoe==1)
    {
        if(isSkillTargetFriendly( thisskill ))
        {   
            cout << "Friendly skill: " << thisclient->Battle->skillid << endl;
        }
        else
        {
            CMap* map = MapList.Index[thisclient->Position->Map];
            CCharacter* character = map->GetCharInMap( thisclient->Battle->target );
            if(character==NULL) return true;
            thisclient->StartAction( character , AOE_TARGET, skillid );
        }      
    }
    else
    {
        Log( MSG_INFO, "no aoe, skillid = %i ", thisclient->Battle->skillid);
    }
    return true;
}

// Identify Item
bool CWorldServer::pakidentify( CPlayer* thisclient, CPacket* P)
{
    WORD itemslot = GETWORD ((*P),0);
    if(!CheckInventorySlot( thisclient, itemslot))
        return false;           
    thisclient->items[itemslot].appraised = true;
    BEGINPACKET( pak, 0x7ba );
    ADDWORD    ( pak, itemslot );
    ADDBYTE    ( pak, 0x00 );
    thisclient->client->SendPacket( &pak );
    return true;
}

// Show Storge Items
bool CWorldServer::pakStorage( CPlayer* thisclient, CPacket* P)
{
    BYTE action = GETBYTE((*P),0);
    switch(action)
    {
        case 0x00:
        {
             //LMA: get storage from database to be sure.
             //GetAllStorage(thisclient);
             
            BEGINPACKET( pak, 0x7ad );
            ADDBYTE    ( pak, 0x00 );            
            ADDBYTE    ( pak, thisclient->nstorageitems ); //numero de items
            for(int i=0;i<160;i++)
            {
           		if( thisclient->storageitems[i].itemtype !=0 )
           		{
                    ADDBYTE    ( pak, i );
                  	ADDDWORD   ( pak, BuildItemHead( thisclient->storageitems[i] ) );
               		ADDDWORD   ( pak, BuildItemData( thisclient->storageitems[i] ) );
                    ADDDWORD( pak, 0x00000000 );
                    ADDWORD ( pak, 0x0000 );   
                }            
            }
            ADDQWORD( pak, thisclient->CharInfo->Storage_Zulies );
            thisclient->client->SendPacket( &pak );            
        }
        break;
        default:
            Log( MSG_INFO, "Storage unknown action: %i ", action);            
    }
    return true;
}

// Change Storage (Deposit/Withdraw items)
//2do: take Zulyes from player's money when getting / putting items from / into storage.
bool CWorldServer::pakChangeStorage( CPlayer* thisclient, CPacket* P)
{
    BYTE action = GETBYTE((*P),0);
    switch(action)
    {
        case 0x00: //Deposit
        {
            BYTE itemslot = GETBYTE((*P),1);
            if(!CheckInventorySlot( thisclient, itemslot ))
                return false;
            
            /*
            //LMA: checking if item is the same we get from packet, testing itemtype and itemnum...
            //2do: test other things too (later)
            CItem testitem = GetItemByHeadAndData(GETDWORD((*P),2),GETDWORD((*P),6));
            if ((testitem.itemnum!=thisclient->items[itemslot].itemnum)||(testitem.itemtype!=thisclient->items[itemslot].itemtype))
            {
               Log(MSG_HACK,"%s, Different object in Packet [%i:%i] and in inventory slot %i [%i:%i]",thisclient->CharInfo->charname,testitem.itemtype,testitem.itemnum,itemslot,thisclient->items[itemslot].itemtype,thisclient->items[itemslot].itemnum);
               return false;
            }
            */
            
            CItem newitem = thisclient->items[itemslot];   
            if(newitem.itemtype>9 && newitem.itemtype<14)
            {
                WORD count = GETWORD((*P),6);                
                if(count>thisclient->items[itemslot].count)
                    count = thisclient->items[itemslot].count;
                newitem.count = count;
                thisclient->items[itemslot].count -= count;
                if(thisclient->items[itemslot].count<=0)
                    ClearItem(thisclient->items[itemslot]);
            }    
            else
            {
                ClearItem(thisclient->items[itemslot]);                
            }        
            int newslot = thisclient->GetNewStorageItemSlot ( newitem );
            //Log(MSG_INFO,"New (?) slot for deposit: %i",newslot);
            
            //LMA: New code (stackables?)
            if (thisclient->storageitems[newslot].itemnum!=0)
            {
               //Log(MSG_INFO,"it should be a stackable in slot: %i, from %i (+%i)",newslot,thisclient->storageitems[newslot].count,newitem.count);
               newitem.count+=thisclient->storageitems[newslot].count;
               //Log(MSG_INFO,"so new=%i",newitem.count);               
            }
            else
            {
                //Log(MSG_INFO,"it should be a new slot: %i",newslot);
                thisclient->nstorageitems++;
            }
                        
            if(newslot==0xffff)
                return true;
            BEGINPACKET( pak, 0x7ae );
            ADDWORD    ( pak, itemslot );
            ADDWORD    ( pak, newslot ); 
	       	ADDDWORD   ( pak, BuildItemHead( thisclient->items[itemslot] ) );
    		ADDDWORD   ( pak, BuildItemData( thisclient->items[itemslot] ) );
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );   
            ADDDWORD   ( pak, BuildItemHead( newitem ) ); 
            ADDDWORD   ( pak, BuildItemData( newitem ) ); 
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );   
    		ADDQWORD   ( pak, thisclient->CharInfo->Zulies );
            thisclient->client->SendPacket( &pak );
            
            //LMA: previous code:
            /* 
            thisclient->storageitems[newslot] = newitem;    
            thisclient->nstorageitems++;
            */
                       
            thisclient->storageitems[newslot] = newitem;
            
            //LMA: need to save the storage item...
            SaveSlotStorage(thisclient,newslot);
        }
        break;//thanks to anon for post that this break was missing 
        case 0x01: //Withdraw
        {   
            BYTE storageslot = GETBYTE((*P),1);            
            if(storageslot>=160)
            {
                Log( MSG_HACK, "%s, Invalid storage slot %i from %s",thisclient->CharInfo->charname, storageslot, thisclient->Session->username );
                return false;
            }
            
            //LMA: get the slot concerned to refresh it from MySQL storage
            if(!GetSlotStorage(thisclient,storageslot))
            {
                Log( MSG_HACK, "%s, Invalid storage slot %i from %s (from Mysql)",thisclient->CharInfo->charname, storageslot, thisclient->Session->username );
                return false;             
            }
            
            /*
            //LMA: checking if item is the same we get from packet, testing itemtype and itemnum...
            //2do: test other things too (later)
            CItem testitem = GetItemByHeadAndData(GETDWORD((*P),2),GETDWORD((*P),6));
            if ((testitem.itemnum!=thisclient->storageitems[storageslot].itemnum)||(testitem.itemtype!=thisclient->storageitems[storageslot].itemtype))
            {
               Log(MSG_HACK,"%s, Different object in Packet [%i:%i] and in storage slot %i [%i:%i]",thisclient->CharInfo->charname,testitem.itemtype,testitem.itemnum,storageslot,thisclient->storageitems[storageslot].itemtype,thisclient->storageitems[storageslot].itemnum);
               return false;
            }            
            */
            
            //CItem newitem =  newitem = thisclient->storageitems[storageslot];                  
            CItem newitem = thisclient->storageitems[storageslot];
            if(newitem.itemtype>9 && newitem.itemtype<14)
            {
                WORD count = GETWORD((*P),6);                
                if( count>thisclient->storageitems[storageslot].count )
                    count = thisclient->storageitems[storageslot].count;
                newitem.count = count;
                thisclient->storageitems[storageslot].count -= count;
                if(thisclient->storageitems[storageslot].count<=0)
                    ClearItem(thisclient->storageitems[storageslot]);
            }    
            else
            {
                ClearItem(thisclient->storageitems[storageslot]);                
            }                             
            
            int newslot= thisclient->GetNewItemSlot ( newitem );
            //no place in player's inventory, so back to storage. 
            if(newslot==0xffff)
            {
                thisclient->storageitems[storageslot] = newitem;
                return true;
            }
                            
            int amount = 0;
            if(thisclient->items[newslot].count>0)                
            {
              int amount = thisclient->items[newslot].count;                     
			  newitem.count+=amount;
            }
            if( newitem.count > 999 )
            {
                amount = 999 - newitem.count;
                newitem.count = 999;                         
            }        
            thisclient->items[newslot] = newitem;   
            if( amount > 0 )
            {
                newitem.count = amount;
                unsigned int newslot2 = thisclient->GetNewItemSlot( newitem );
                if( newslot2 == 0xffff )
                {
                    thisclient->storageitems[storageslot] = thisclient->items[newslot];
                    thisclient->items[newslot].count = amount;
                    return true;
                }
                thisclient->items[newslot2] = newitem;
                thisclient->UpdateInventory( newslot2 );    
            }                                                                                     
            BEGINPACKET( pak, 0x7ae );
            ADDWORD    ( pak, newslot );
            ADDWORD    ( pak, storageslot );
	       	ADDDWORD   ( pak, BuildItemHead( thisclient->items[newslot] ) );
    		ADDDWORD   ( pak, BuildItemData( thisclient->items[newslot] ) );
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );   
                  
            ADDDWORD   ( pak, BuildItemHead( thisclient->storageitems[storageslot] ) );
            ADDDWORD   ( pak, BuildItemData( thisclient->storageitems[storageslot] ) );
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );   
    		ADDQWORD   ( pak, thisclient->CharInfo->Zulies );
            ADDBYTE    ( pak, 0x00 );    		
            thisclient->client->SendPacket( &pak );            
            thisclient->nstorageitems--;
            
            //LMA: need to save the storage item...
            SaveSlotStorage(thisclient,storageslot);                                  
        }        
        break;
        default:
            Log( MSG_INFO, "Storage unknown action: %i ", action);             
    }
    return true;
}

// Deposit/Widthdraw zulies
bool CWorldServer::pakStoreZuly( CPlayer* thisclient, CPacket* P)
{
    BYTE action = GETBYTE ((*P), 0 );
    QWORD zuly = GETQWORD ((*P), 1 );    
    switch(action)
    {
        case 0x10://deposit'
        {            
            if(zuly > thisclient->CharInfo->Zulies)
                return true;
            thisclient->CharInfo->Zulies -= zuly;
            thisclient->CharInfo->Storage_Zulies += zuly;
            BEGINPACKET( pak, 0x7da );
            ADDQWORD   ( pak, thisclient->CharInfo->Zulies );
            ADDQWORD   ( pak, thisclient->CharInfo->Storage_Zulies );
            ADDBYTE    ( pak, 0x00 );
            thisclient->client->SendPacket( &pak ); 
            
            //LMA: Saving Zuly Storage
            SaveZulyStorage(thisclient);                   
        }
        break;
        case 0x11://withdraw
        {
             //LMA: refreshing Zuly from MySQL...
             if(!GetZulyStorage(thisclient))
                   return true;
            if(zuly > thisclient->CharInfo->Storage_Zulies )
                return true;
            thisclient->CharInfo->Zulies += zuly;
            thisclient->CharInfo->Storage_Zulies -= zuly;
            BEGINPACKET( pak, 0x7da );
            ADDQWORD   ( pak, thisclient->CharInfo->Zulies );
            ADDQWORD   ( pak, thisclient->CharInfo->Storage_Zulies );            
            ADDBYTE    ( pak, 0x00 );
            thisclient->client->SendPacket( &pak );
            
            //LMA: Saving Zuly Storage
            SaveZulyStorage(thisclient);    
        }
        break;
        default:
            Log( MSG_INFO, "Storage unknown action: %i ", action);     
    }
    return true;
}

 
// Open Shop
bool CWorldServer::pakOpenShop( CPlayer* thisclient, CPacket* P )
{
     //LMA 139+:
     //They added six extra 0x00 between items and prices + shop name...
     UINT lma_offset=6;
     
     
    if( thisclient->Shop->open ) 
        return true;    
    BYTE nselling = GETBYTE((*P),0);
    BYTE nbuying = GETBYTE((*P),1);
    if(nselling>30 || nbuying>30) 
        return true;
    int nchar = ((nselling + nbuying) * (12+lma_offset) ) + nselling + nbuying + 2;
    strncpy(thisclient->Shop->name ,(char*)&(*P).Buffer[nchar] , P->Size-nchar );  
    
    Log(MSG_INFO,"[Create] Begin Shop %s:",thisclient->Shop->name);
    thisclient->Shop->Selling = nselling;
    thisclient->Shop->Buying = nbuying;
    for(int i=0;i<nselling;i++)
    {
        int n=(i*(13+lma_offset))+2;
        BYTE slot =  GETBYTE((*P),n);
        if(!CheckInventorySlot( thisclient, slot))
            return false; 
        thisclient->Shop->SellingList[i].slot = slot;
        if( thisclient->items[slot].itemtype>9 && thisclient->items[slot].itemtype<14 )
        {
            int itemCount = GETWORD((*P),n+5);
            if (thisclient->items[slot].count < itemCount)
               return true;
            thisclient->Shop->SellingList[i].count = itemCount; // geo edit for invisible stackables // 30 sep 07
        }
        else
            thisclient->Shop->SellingList[i].count = 1;
        thisclient->Shop->SellingList[i].price = GETDWORD((*P),n+9+lma_offset);
        
        //LMA Log:
        Log(MSG_INFO,"[S-%i/%i], Item (%i:%i), slot %i, Nb %i, Price %i",i+1,nselling,thisclient->items[slot].itemtype,thisclient->items[slot].itemnum,slot,thisclient->Shop->SellingList[i].count,thisclient->Shop->SellingList[i].price);        
    }         
    for(int i=0;i<nbuying;i++)
    {
        unsigned int n=(nselling*(13+lma_offset))+2+(i*(12+lma_offset))+i;
        BYTE slot = GETBYTE((*P),n);
        thisclient->Shop->BuyingList[slot].slot = slot;     
        DWORD head = GETDWORD((*P),n+1);
        DWORD data = GETDWORD((*P),n+5);
        CItem thisitem = GetItemByHeadAndData( head, data );
        thisclient->Shop->BuyingList[slot].item = thisitem;
        if( thisitem.itemtype>9 && thisitem.itemtype<14 )
            thisclient->Shop->BuyingList[slot].count = GETWORD((*P),n+5); 
        else
            thisclient->Shop->BuyingList[slot].count = 1;         
        thisclient->Shop->BuyingList[slot].price = GETDWORD((*P),n+9+lma_offset);
        
        //LMA Log:
        Log(MSG_INFO,"[B-%i/%i], Item (%i:%i), slot %i, Nb %i, Price %i",i+1,nbuying,thisitem.itemtype,thisitem.itemnum,slot,thisclient->Shop->BuyingList[slot].count,thisclient->Shop->BuyingList[slot].price);                
    }
    
    Log(MSG_INFO,"[Create] End Shop %s.",thisclient->Shop->name);
    
    BEGINPACKET( pak, 0x796 );
    ADDWORD    ( pak, thisclient->clientid );
    ADDFLOAT   ( pak, thisclient->Position->current.x );
    ADDFLOAT   ( pak, thisclient->Position->current.y );
    ADDWORD    ( pak, 0x9057 );//>???
    SendToVisible( &pak, thisclient );
    
    RESETPACKET( pak, 0x7c2 );
    ADDWORD    ( pak, thisclient->clientid );
    ADDWORD    ( pak, thisclient->Shop->ShopType );
    ADDSTRING  ( pak, thisclient->Shop->name );
    ADDBYTE    ( pak, 0x00 );
    SendToVisible( &pak, thisclient );
    thisclient->Shop->open = true;
    return true;
}
 

// Show Shop to other players
bool CWorldServer::pakShowShop( CPlayer* thisclient, CPacket* P )
{
    WORD otherclientid = GETWORD((*P),0);
    CPlayer* otherclient = GetClientByID ( otherclientid, thisclient->Position->Map );
    if(otherclient==NULL)
        return true;
    BEGINPACKET( pak, 0x7c4 );
    ADDBYTE    ( pak, otherclient->Shop->Selling );
    ADDBYTE    ( pak, otherclient->Shop->Buying );
    for(unsigned int i = 0; i<otherclient->Shop->Selling;i++)
    {
        CItem thisitem =  otherclient->items[otherclient->Shop->SellingList[i].slot];
        thisitem.count = otherclient->Shop->SellingList[i].count;
        ADDBYTE     ( pak, i );
        ADDDWORD    ( pak, BuildItemHead( thisitem ) );
        ADDDWORD    ( pak, BuildItemData( thisitem ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
       ADDDWORD    ( pak, otherclient->Shop->SellingList[i].price );
    }
    for(unsigned int i = 0; i<30;i++)
    {
        if(otherclient->Shop->BuyingList[i].count>0)
        {
            CItem thisitem = otherclient->Shop->BuyingList[i].item;
            ADDBYTE     ( pak, i );
            ADDDWORD    ( pak, BuildItemHead( thisitem ) );
            ADDDWORD    ( pak, BuildItemData( thisitem ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
            ADDDWORD    ( pak, otherclient->Shop->BuyingList[i].price );
        }
    }
    ADDBYTE    ( pak, 0x00 );
    thisclient->client->SendPacket( &pak );    
    return true;
}

// Buy From Shop
//LMA: checking for Zuly hacks
bool CWorldServer::pakBuyShop( CPlayer* thisclient, CPacket* P )
{
    WORD otherclientid = GETWORD((*P),0);
    BYTE action = GETBYTE((*P),2);
    switch(action)
    {
        case 0x01://check this
        {
            CPlayer* otherclient = GetClientByID( otherclientid, thisclient->Position->Map );
            if( otherclient==NULL )
                return true;
            BYTE slot = GETBYTE((*P),3);
            unsigned int count = 0;
            unsigned int invslot = otherclient->Shop->SellingList[slot].slot;
            CItem newitem =  otherclient->items[invslot];                                      
            if(otherclient->items[invslot].itemtype>9 && 
                    otherclient->items[invslot].itemtype<14)
                count = GETWORD((*P),8);
            else
                count = 1;                
            if( count > otherclient->Shop->SellingList[slot].count )
                return true;
            newitem.count = count;                                                              
            if( thisclient->CharInfo->Zulies<(otherclient->Shop->SellingList[slot].price*count) )
                return true;            
            unsigned int newslot = thisclient->GetNewItemSlot ( newitem );
            if(newslot==0xffff)
                return true;
                
            //LMA: check for hacks...
            if (thisclient->CharInfo->Zulies < (otherclient->Shop->SellingList[slot].price*count))
            {
              Log(MSG_HACK, "[Buy in Shop] Not enough Zuly player %s, have %li, need %li",thisclient->CharInfo->charname,thisclient->CharInfo->Zulies,otherclient->Shop->SellingList[slot].price*count);
              return true;
            }
            
            thisclient->CharInfo->Zulies -= (otherclient->Shop->SellingList[slot].price*count);
            otherclient->CharInfo->Zulies += (otherclient->Shop->SellingList[slot].price*count);                
            if(otherclient->items[invslot].itemtype>9 && 
                    otherclient->items[invslot].itemtype<14)
            {
                if(otherclient->items[invslot].count<=count)
                {
                     ClearItem(otherclient->items[invslot])
                     otherclient->Shop->SellingList[slot].slot = 0;
                     otherclient->Shop->SellingList[slot].count = 0;
                     otherclient->Shop->SellingList[slot].price = 0;                                          
                }
                else
                {
                    if(otherclient->Shop->SellingList[slot].count<=count)
                    {                            
                        otherclient->Shop->SellingList[slot].slot = 0;
                        otherclient->Shop->SellingList[slot].count = 0;
                        otherclient->Shop->SellingList[slot].price = 0;                           
                    }
                    else
                    {
                        otherclient->Shop->SellingList[slot].count -= count;
                    }
                    otherclient->items[invslot].count-=count;                      
                }
            }                    
            else
            {             
                ClearItem( otherclient->items[invslot] )  
                otherclient->Shop->SellingList[slot].slot = 0;
                otherclient->Shop->SellingList[slot].count = 0;
                otherclient->Shop->SellingList[slot].price = 0;                   
            }
            if(thisclient->items[newslot].count!=0)            
                thisclient->items[newslot].count += newitem.count;
            else
                thisclient->items[newslot] = newitem;
            //update inventory (buyer)
            BEGINPACKET( pak, 0x7c7 );
            ADDQWORD   ( pak, thisclient->CharInfo->Zulies );
            ADDBYTE    ( pak, 0x01 ); //buy action
            ADDBYTE    ( pak, newslot );
            ADDDWORD   ( pak, BuildItemHead( thisclient->items[newslot] ) );
            ADDDWORD   ( pak, BuildItemData( thisclient->items[newslot] ) );
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );
            thisclient->client->SendPacket( &pak );    
            
            //update inventory (seller)        
            RESETPACKET( pak, 0x7c7 );
            ADDQWORD   ( pak, otherclient->CharInfo->Zulies );
            ADDBYTE    ( pak, 0x01 ); //buy action
            ADDBYTE    ( pak, invslot );
            ADDDWORD   ( pak, BuildItemHead( otherclient->items[invslot] ) );
            ADDDWORD   ( pak, BuildItemData( otherclient->items[invslot] ) );
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );   
            otherclient->client->SendPacket( &pak );             
            
            //update shop
            RESETPACKET( pak, 0x7c6 );
            ADDWORD    ( pak, otherclient->clientid );
            ADDBYTE    ( pak, 0x05 );
            ADDBYTE    ( pak, 0x01 );
            ADDBYTE    ( pak, slot );
            if(otherclient->Shop->SellingList[slot].count<=0)
            {
               ADDDWORD ( pak, 0x00000000 );
               ADDDWORD ( pak, 0x00000000 );
               ADDDWORD( pak, 0x00000000 );
               ADDWORD ( pak, 0x0000 );                
            }
            else
            {
               ADDDWORD ( pak, BuildItemHead( otherclient->items[invslot] ));
               ADDDWORD ( pak, BuildItemData( otherclient->items[invslot] ));                               
               ADDDWORD( pak, 0x00000000 );
               ADDWORD ( pak, 0x0000 );   
            }
            SendToVisible( &pak, otherclient );                        
        }
        break;
        default:
            Log( MSG_WARNING, "Buy unknown action: %i", action );
    }
    return true;
}

// Sell in Shop
bool CWorldServer::pakSellShop( CPlayer* thisclient, CPacket* P )
{
    WORD otherclientid = GETWORD((*P),0);
    BYTE action = GETBYTE((*P),2);
    switch(action)
    {
        case 0x01:
        {
            CPlayer* otherclient = GetClientByID( otherclientid, thisclient->Position->Map );
            if( otherclient==NULL )
                return true;
            unsigned int count = 0;                
            BYTE invslot = GETBYTE((*P),3);
            BYTE slot = GETBYTE((*P),4);   
            if(!CheckInventorySlot( thisclient, slot))
                return false;             
            CItem newitem =  thisclient->items[invslot];  
            if(thisclient->items[invslot].itemtype>9 && thisclient->items[invslot].itemtype<14)
                count = GETWORD((*P),9);
            else
                count = 1;      
            newitem.count = count;                                                              
            if( otherclient->CharInfo->Zulies<(otherclient->Shop->BuyingList[slot].price*count) )
                return true;                            
            unsigned int newslot = otherclient->GetNewItemSlot ( newitem );
            if( newslot==0xffff )
                return true;                  
            if( thisclient->items[invslot].count<count )                        
                return true;                           
            thisclient->CharInfo->Zulies += (otherclient->Shop->BuyingList[slot].price*count);
            otherclient->CharInfo->Zulies -= (otherclient->Shop->BuyingList[slot].price*count);              
            if(thisclient->items[invslot].itemtype>9 && 
                    thisclient->items[invslot].itemtype<14)
            {
                if(thisclient->items[invslot].count<=count)
                {
                    ClearItem( thisclient->items[invslot] );
                    if(count>=otherclient->Shop->BuyingList[slot].count)
                    {
                         ClearItem( otherclient->Shop->BuyingList[slot].item ); 
                         otherclient->Shop->BuyingList[slot].slot = 0;
                         otherclient->Shop->BuyingList[slot].count = 0;
                         otherclient->Shop->BuyingList[slot].price = 0;
                    }
                    else
                    {                 
                         otherclient->Shop->BuyingList[slot].count -= count; 
                    }
                }
                else
                {
                    if(otherclient->Shop->BuyingList[slot].count<=count)
                    {                            
                         ClearItem( otherclient->Shop->BuyingList[slot].item ); 
                         otherclient->Shop->BuyingList[slot].slot = 0;
                         otherclient->Shop->BuyingList[slot].count = 0;
                         otherclient->Shop->BuyingList[slot].price = 0;                        
                    }
                    else
                    {  
                        otherclient->Shop->BuyingList[slot].count -= count;                      
                    }
                    thisclient->items[invslot].count-=count;                      
                }
            }                    
            else
            {             
                ClearItem( thisclient->items[invslot] )  
                ClearItem( otherclient->Shop->BuyingList[slot].item ); 
                otherclient->Shop->BuyingList[slot].slot = 0;
                otherclient->Shop->BuyingList[slot].count = 0;
                otherclient->Shop->BuyingList[slot].price = 0;                       
            }
            if(otherclient->items[newslot].count!=0)            
                otherclient->items[newslot].count += newitem.count;
            else
                otherclient->items[newslot] = newitem;   
                
            //update inventory (seller)
            BEGINPACKET( pak, 0x7c7 );
            ADDQWORD   ( pak, thisclient->CharInfo->Zulies );
            ADDBYTE    ( pak, 0x01 ); 
            ADDBYTE    ( pak, invslot );
            ADDDWORD   ( pak, BuildItemHead( thisclient->items[invslot] ) );
            ADDDWORD   ( pak, BuildItemData( thisclient->items[invslot] ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
            thisclient->client->SendPacket( &pak );    
            
            //update inventory (buyer)        
            RESETPACKET( pak, 0x7c7 );
            ADDQWORD   ( pak, otherclient->CharInfo->Zulies );
            ADDBYTE    ( pak, 0x01 ); 
            ADDBYTE    ( pak, newslot );
            ADDDWORD   ( pak, BuildItemHead( otherclient->items[newslot] ) );
            ADDDWORD   ( pak, BuildItemData( otherclient->items[newslot] ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
            otherclient->client->SendPacket( &pak );     
            
            //update shop
            CItem thisitem = otherclient->Shop->BuyingList[slot].item;
            RESETPACKET( pak, 0x7c6 );
            ADDWORD    ( pak, otherclient->clientid );
            ADDBYTE    ( pak, 0x07 );
            ADDBYTE    ( pak, 0x01 );
            ADDBYTE    ( pak, slot );
            ADDDWORD   ( pak, BuildItemHead( thisitem ) );
            ADDDWORD   ( pak, BuildItemData( thisitem ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
            SendToVisible( &pak, otherclient );                                                  
        }
        break;
        default:
            Log( MSG_WARNING, "Sell unknown action: %i", action );        
    }    
    return true;
}

// Close Shop
bool CWorldServer::pakCloseShop( CPlayer* thisclient, CPacket* P )
{
    BEGINPACKET( pak, 0x7c3 );
    ADDWORD    ( pak, thisclient->clientid );
    ADDBYTE    ( pak, 0x00 );
    SendToVisible( &pak, thisclient );
    thisclient->Shop->open = false;  
    for(unsigned int j=0;j<30;j++)
    {
        thisclient->Shop->SellingList[j].slot = 0;
        thisclient->Shop->SellingList[j].count = 0;
        thisclient->Shop->SellingList[j].price = 0;
        thisclient->Shop->BuyingList[j].slot = 0;
        thisclient->Shop->BuyingList[j].count = 0;
        thisclient->Shop->BuyingList[j].price = 0;                     
        ClearItem( thisclient->Shop->BuyingList[j].item );
       
    }      
    return true;
}

// Modified item (put gem, refine, Drill )
bool CWorldServer::pakModifiedItem( CPlayer* thisclient, CPacket* P )
{
    if(thisclient->Shop->open==true)
        return true;    
    BYTE action = GETBYTE((*P),0);
    switch(action)
    {
        case 0x01://Gemming
        {            
            BYTE destslot = GETBYTE((*P),1);
            BYTE srcslot = GETBYTE((*P),2);
            if(!CheckInventorySlot( thisclient, destslot))
                return false;  
            if(!CheckInventorySlot( thisclient, srcslot))
                return false;                                
            if( thisclient->items[srcslot].count<=0 ) 
            {
                BEGINPACKET( pak, 0x7bc );
                ADDBYTE    ( pak, 0x12 ); // dont have requerid material
                ADDBYTE    ( pak, 0x00 );
                thisclient->client->SendPacket( &pak );
                return true;            
            }
            if( thisclient->items[destslot].gem!=0 )
            {
                BEGINPACKET( pak, 0x7bc );
                ADDBYTE    ( pak, 0x02 ); //no socket available
                ADDBYTE    ( pak, 0x00 );                             
                thisclient->client->SendPacket( &pak );
                return true;
            }
            thisclient->items[destslot].gem = thisclient->items[srcslot].itemnum;
            thisclient->items[srcslot].count--;                                                                          
            if(thisclient->items[srcslot].count<1)
                ClearItem( thisclient->items[srcslot] );
            BEGINPACKET( pak, 0x7bc );
            ADDBYTE    ( pak, 0x01 );//gemming success               
            ADDBYTE    ( pak, 0x02 );      
            ADDBYTE    ( pak, destslot );
            ADDDWORD   ( pak, BuildItemHead( thisclient->items[destslot] ) );
            ADDDWORD   ( pak, BuildItemData( thisclient->items[destslot] ) );      
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
            ADDBYTE    ( pak, srcslot );
            ADDDWORD   ( pak, BuildItemHead( thisclient->items[srcslot] ) );
            ADDDWORD   ( pak, BuildItemData( thisclient->items[srcslot] ) );               
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
            thisclient->client->SendPacket( &pak );        
            thisclient->SetStats( );
        }   
        break; //case 0x02 ( Surprise gift box )       

       case 0x02: // Treasure Chests, Gift Box - by Drakia
                  // Disassemble - by Geobot
        {
            CItem item;
            CItem itemextra;
            unsigned int chestSlot = GETBYTE((*P), 3);
            unsigned int rewardCount = (RandNumber(0, 100) < 20)?2:1;
            CChest* thischest = GetChestByID(thisclient->items[chestSlot].itemnum);
            if (thischest == NULL)
            {
/////////////////////////////////////   start disassemble           
            BYTE src = GETBYTE((*P),3);
            if(!CheckInventorySlot( thisclient, src))
                return false;
            if(thisclient->items[src].count < 1)
                return false;
           
           int k = 9999;
           for(int i=0;i<1000;i++)
           {
               if(thisclient->items[src].itemnum == BreakList[i].itemnum && thisclient->items[src].itemtype == BreakList[i].itemtype)
                   k = i;
           }
           if(k==9999)
               return false;
           
           UINT totalprob = 0;
           for(int i=0;i<BreakList[k].total;i++)
           {
               totalprob += BreakList[k].prob[i];
           }
           if(totalprob==0)
               return false;
               
           UINT rand = RandNumber(0,99999);
           UINT m = 99;
           for(int i=0;i<BreakList[k].total;i++)
           {
               if(rand < BreakList[k].prob[i])
                   m = i;
               else
                   rand -= BreakList[k].prob[i];
           }
           if(m>14)
               return false;
               
           UINT num = BreakList[k].product[m] % 1000;
           UINT type = int(BreakList[k].product[m] / 1000);
               
               CItem newitem;
               newitem.itemnum = num;
               newitem.itemtype = type;
               newitem.count = BreakList[k].amount[m];
               newitem.refine = 0;
               newitem.lifespan = 100;
               newitem.durability = RandNumber(40,50);
               newitem.socketed=0;
               newitem.appraised=0;
               newitem.stats=0;
               newitem.gem=0;
         
               unsigned newslot = thisclient->GetNewItemSlot(newitem);
               if(newslot == 0xffff) return false;
 
               if(thisclient->items[newslot].count > 0)
               {
                   thisclient->items[newslot].count += newitem.count;
                   if(thisclient->items[newslot].count > 999)
                       thisclient->items[newslot].count = 999;
               }
               else
                   thisclient->items[newslot] = newitem;  
             
              thisclient->items[src].count -= 1;
              if( thisclient->items[src].count < 1)
                  ClearItem( thisclient->items[src] );      
              thisclient->UpdateInventory(src);        
 
              BEGINPACKET( pak, 0x7bc );
              ADDBYTE    ( pak, 0x07 );//disassemble success
              ADDBYTE    ( pak, 0x02 );//number of items to follow  
              ADDBYTE    ( pak, newslot );
              ADDDWORD   ( pak, BuildItemHead( thisclient->items[newslot] ) );
              ADDDWORD   ( pak, BuildItemData( thisclient->items[newslot] ) );      
              ADDWORD    ( pak, 0x0000);
              ADDWORD    ( pak, 0x0000);
              ADDWORD    ( pak, 0x0000);
              ADDBYTE    ( pak, src );
              ADDDWORD   ( pak, BuildItemHead( thisclient->items[src] ) );
              ADDDWORD   ( pak, BuildItemData( thisclient->items[src] ) );              
              ADDWORD    ( pak, 0x0000);
              ADDWORD    ( pak, 0x0000);
              ADDWORD    ( pak, 0x0000);
              thisclient->client->SendPacket( &pak );        
              return true;
/////////////////////////////////////    end disassemble            
            }
            unsigned int randv = RandNumber( 1, thischest->probmax );
 
            DWORD prob = 1;
            for(UINT i=0;i<thischest->Rewards.size();i++)
            {
                CReward* reward = thischest->Rewards.at( i );
                prob += reward->prob;
                if(randv<=prob)
                {
                    item.itemtype = reward->type;
                    item.itemnum = reward->id;
                    item.count = 1;
                    item.socketed = false;
                    item.appraised = true;
                    item.lifespan = 100;
                    item.durability = 100;
                    item.refine = 0;
                    item.stats = 0;
                    item.gem = 0;
                    prob = reward->prob;
                    break;
                }
            }
            if (rewardCount > 1)
            {
                DWORD probextra = 1;
                randv = RandNumber( 1, thischest->probmax - prob );
                itemextra.itemnum = 0;
                for(UINT i=0;i<thischest->Rewards.size() - 1;i++)
                {
                    CReward* reward = thischest->Rewards.at( i );
                    if (reward->id != item.itemnum) {
                        prob += reward->prob;
                        if(randv<=prob)
                        {
                            itemextra.itemtype = reward->type;
                            itemextra.itemnum = reward->id;
                            itemextra.count = 1;
                            itemextra.socketed = false;
                            itemextra.appraised = true;
                            itemextra.lifespan = 100;
                            itemextra.durability = 100;
                            itemextra.refine = 0;
                            itemextra.stats = 0;
                            itemextra.gem = 0;
                            break;
                        }
                    }
                }
                if (itemextra.itemnum == 0)
                {
                    Log(MSG_INFO, "Could not obtain secondary reward. Make sure all chests have atleast 2 rewards.");
                    rewardCount = 1;
                }
            }
 
            thisclient->items[chestSlot].count--;
            if (thisclient->items[chestSlot].count < 1)
                ClearItem(thisclient->items[chestSlot]);
 
            unsigned int tempslot = thisclient->AddItem(item);
            if (tempslot != 0xffff)
            {
            BEGINPACKET( pak, 0x7bc );
            ADDBYTE (pak, 0x13);  // Status code. Congrats?
            ADDBYTE (pak, (rewardCount + 1));  // Number of items
 
            ADDBYTE (pak, tempslot);
            ADDDWORD(pak, BuildItemHead(thisclient->items[tempslot]));
            ADDDWORD(pak, BuildItemData(thisclient->items[tempslot]));
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );
            if (rewardCount > 1)
            {
                tempslot = thisclient->AddItem(itemextra);
                if (tempslot != 0xffff)
                {
                    ADDBYTE(pak, tempslot);
                    ADDDWORD(pak, BuildItemHead(thisclient->items[tempslot]));
                    ADDDWORD(pak, BuildItemData(thisclient->items[tempslot]));
                    ADDDWORD(pak, 0x00000000);
                    ADDWORD(pak, 0x0000);
                } else {
                    Log(MSG_WARNING, "Error adding second item");
                    return true;
                }
            }
 
            ADDBYTE (pak, chestSlot);
            ADDDWORD(pak, BuildItemHead(thisclient->items[chestSlot]));
            ADDDWORD(pak, BuildItemData(thisclient->items[chestSlot]));
            ADDDWORD( pak, 0x00000000 );
            ADDWORD ( pak, 0x0000 );
 
            thisclient->client->SendPacket( &pak );
            }
            return true;
        }
        break;
        
        case 0x05://Refine
        {
            BYTE item = GETBYTE((*P),3);
            BYTE material = GETBYTE((*P),4);
            if(!CheckInventorySlot( thisclient, item))
                return false;  
            if(!CheckInventorySlot( thisclient, material))
                return false;                              
            if(thisclient->items[item].count<1 || thisclient->items[material].count<1)
            {
                BEGINPACKET( pak, 0x7bc );
                ADDBYTE    ( pak, 0x12 );
                ADDBYTE    ( pak, 0x00 );
                thisclient->client->SendPacket( &pak );
                return true;
            }
            unsigned int nextlevel = ( thisclient->items[item].refine / 16 ) + 1;
            if( nextlevel > 9 )
                return true;
            unsigned int prefine = rand()%100;
            bool success = false;
            if( prefine <= upgrade[nextlevel] )
                success = true;                
            BEGINPACKET( pak, 0x7bc )                
            if( success )
            {
                thisclient->items[item].refine = nextlevel*16;
                ADDBYTE    ( pak, 0x10 );//successful
            }
            else
            {
                ClearItem( thisclient->items[item] );
                ADDBYTE    ( pak, 0x11 );//Fail
            }
            thisclient->items[material].count--; // geo edit, moved this up two lines
            if(thisclient->items[material].count<1)
                ClearItem( thisclient->items[material] );                    
            ADDBYTE    ( pak, 0x03 );//items a actualizar
            ADDBYTE    ( pak, material );
            ADDDWORD   ( pak, BuildItemHead( thisclient->items[material] ) );
            ADDDWORD   ( pak, BuildItemData( thisclient->items[material] ) );            
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
            ADDBYTE    ( pak, item );
            ADDDWORD   ( pak, BuildItemHead( thisclient->items[item] ) );
            ADDDWORD   ( pak, BuildItemData( thisclient->items[item] ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
            ADDBYTE    ( pak, 0x00 );
            ADDDWORD   ( pak, 0x002f0000 );
            ADDDWORD   ( pak, 0x00000017 );
            thisclient->client->SendPacket( &pak );                                
        }
        break;
        case 0x06: // Drill
        {
            BYTE material = GETBYTE((*P), 1);       
            BYTE item = GETBYTE((*P), 3);     
            if(!CheckInventorySlot( thisclient, item))
                return false;  
            if(!CheckInventorySlot( thisclient, material))
                return false;                               
            if( thisclient->items[material].count<=0 )
            {
                BEGINPACKET( pak, 0x7bc );
                ADDBYTE    ( pak, 0x12 );
                ADDBYTE    ( pak, 0x00 );
                thisclient->client->SendPacket( &pak );                
                return true;
            } 
            if( thisclient->items[item].socketed ) return true;//Show message                        
            thisclient->items[material].count -= 1;
            thisclient->items[item].socketed = true;
            if( thisclient->items[material].count<=0 )
                ClearItem(thisclient->items[material]);
            BEGINPACKET( pak, 0x7cb );
            ADDBYTE    ( pak, 2 );
            ADDBYTE    ( pak, material);
            ADDDWORD   ( pak, BuildItemHead( thisclient->items[material] ) );
            ADDDWORD   ( pak, BuildItemData( thisclient->items[material] ) );            
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
            ADDBYTE    ( pak, item );
            ADDDWORD   ( pak, BuildItemHead( thisclient->items[item] ) );
            ADDDWORD   ( pak, BuildItemData( thisclient->items[item] ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
            thisclient->client->SendPacket(&pak);                           
        }
        break;        
        default:
            Log( MSG_WARNING,"Modified Item unknown action: %i", action);
    }
    return true;
}

// Repair
bool CWorldServer::pakRepairItem( CPlayer* thisclient, CPacket* P )
{
            BYTE slot = GETBYTE((*P),2);
            if(!CheckInventorySlot( thisclient, slot)) return false;
            if(thisclient->items[slot].count<1) return true;
            thisclient->items[slot].lifespan = 100;
            //Still TODO: find where prices of storage and repair are and add it to the code.
            BEGINPACKET( pak, 0x7cd );
            ADDQWORD   ( pak, thisclient->CharInfo->Zulies );
            ADDBYTE    ( pak, 0x01 );
            ADDBYTE    ( pak, slot );
            ADDDWORD   ( pak, BuildItemHead( thisclient->items[slot] ));
            ADDDWORD   ( pak, BuildItemData( thisclient->items[slot] ));
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   
            ADDBYTE    ( pak, 0x00 );
            thisclient->client->SendPacket( &pak );
            thisclient->SetStats( );
    Log ( MSG_WARNING, "Repair Item pak data: %i, %i, %i", GETBYTE((*P),0), GETBYTE((*P),2), GETBYTE((*P),4));
    return true;
}

// Ride request
bool CWorldServer::pakRideRequest( CPlayer* thisclient, CPacket* P )
{ 
    if(thisclient->Shop->open)
        return true;    
    BYTE action = GETBYTE((*P),0);
    switch(action)
    {
        case 0x01://Ride Request
        {
            WORD tclientid = GETWORD((*P),1);
            WORD oclientid = GETWORD((*P),3);            
            if( tclientid != thisclient->clientid )
                return true;
            CPlayer* otherclient = GetClientByID( oclientid, thisclient->Position->Map );
            if(otherclient==NULL)
                return true;
            BEGINPACKET( pak, 0x7dd );
            ADDBYTE    ( pak, 0x01 );
            ADDWORD    ( pak, thisclient->clientid );
            ADDWORD    ( pak, oclientid );
            ADDFLOAT   ( pak, thisclient->Position->current.x*100 );
            ADDFLOAT   ( pak, thisclient->Position->current.y*100 );            
            otherclient->client->SendPacket( &pak );
        }
        break;
        case 0x02://Accept
        {
            WORD oclientid = GETWORD((*P),1);
            WORD tclientid = GETWORD((*P),3);       
            if( tclientid != thisclient->clientid )
                return true;
            CPlayer* otherclient = GetClientByID( oclientid, thisclient->Position->Map );
            if(otherclient==NULL)
                return true;
            BEGINPACKET( pak, 0x796 );
            ADDWORD    ( pak, tclientid );
            ADDFLOAT   ( pak, thisclient->Position->current.x*100 );            
            ADDFLOAT   ( pak, thisclient->Position->current.y*100 );
            ADDWORD    ( pak, 0x0000 );
            SendToVisible( &pak, otherclient );
            
            RESETPACKET( pak, 0x7dd );
            ADDBYTE    ( pak, 0x02 );
            ADDWORD    ( pak, oclientid );
            ADDWORD    ( pak, tclientid );
            SendToVisible( &pak, otherclient );         
            thisclient->Ride->Ride = true;
            thisclient->Ride->Drive = false;
            thisclient->Ride->charid = otherclient->CharInfo->charid;
            otherclient->Ride->Ride = true;
            otherclient->Ride->Drive = true;
            otherclient->Ride->charid = thisclient->CharInfo->charid;
        }
        break;
        case 0x03://Denied
        {
            WORD oclientid = GETWORD((*P),1);
            WORD tclientid = GETWORD((*P),3);       
            if( tclientid != thisclient->clientid )
                return true;
            CPlayer* otherclient = GetClientByID( oclientid, thisclient->Position->Map );
            if(otherclient==NULL)
                return true;
            BEGINPACKET( pak, 0x7dd );
            ADDBYTE    ( pak, 0x03 );
            ADDWORD    ( pak, oclientid );
            ADDWORD    ( pak, tclientid );
            otherclient->client->SendPacket( &pak );                      
        }
        break;
    }
	return true;
}

bool CWorldServer::pakWeight( CPlayer* thisclient, CPacket* P )
{
    BYTE weight = GETBYTE((*P),0) & 0xff;
    if(weight>110)
        thisclient->Status->CanAttack = false;
    else
        thisclient->Status->CanAttack = true;    
    if(weight>100)
        thisclient->Status->CanRun = false;
    else
        thisclient->Status->CanRun = true;
    thisclient->Stats->Move_Speed = thisclient->GetMoveSpeed( );
    BEGINPACKET( pak, 0x762 );
    ADDWORD    ( pak, thisclient->clientid );
    ADDBYTE    ( pak, weight );
    thisclient->client->SendPacket( &pak );
    return true;
}

// Show Monster HP or Player Regeneration
bool CWorldServer::pakShowHeal( CPlayer* thisclient, CPacket* P )
{
    WORD clientid = GETWORD((*P),0);
    CCharacter* character = MapList.Index[thisclient->Position->Map]->GetCharInMap( clientid );
    if(character==NULL) return true;    
    if(character->IsMonster())
    {
    	// SET MONSTER HEALTH
    	BEGINPACKET( pak, 0x79f );
    	ADDWORD( pak, character->clientid );
    	ADDWORD( pak, character->Stats->HP );
    	ADDWORD( pak, 0 );
        thisclient->client->SendPacket( &pak );   
        thisclient->Battle->contatk = false; 
    }
    thisclient->Battle->target = clientid;    
    return true;
}	

// Repair Tool
bool CWorldServer::pakRepairTool( CPlayer* thisclient, CPacket* P )
{
    BEGINPACKET( pak, 0x7cb );

    thisclient->items[(GETBYTE((*P), 0x0))].count -= 1;
    thisclient->items[(GETBYTE((*P), 0x2))].lifespan = 100;
    if(thisclient->items[(GETBYTE((*P), 0x0))].count <= 0){
        ClearItem(thisclient->items[(GETBYTE((*P), 0x0))]);
        RESETPACKET (pak, 0x7cb);
        ADDBYTE(pak, 1);
        ADDBYTE(pak,GETBYTE((*P), 0x2));
        ADDDWORD( pak, BuildItemHead( thisclient->items[GETBYTE((*P), 0x2)]) );
        ADDDWORD( pak, BuildItemData( thisclient->items[GETBYTE((*P), 0x2)]) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   

        thisclient->client->SendPacket(&pak);
    }
    else{
         RESETPACKET (pak, 0x7cb);
         ADDBYTE(pak, 2);
         ADDBYTE(pak,GETBYTE((*P), 0x0));
         ADDDWORD(pak, BuildItemHead(thisclient->items[GETBYTE((*P), 0x0)]));
         ADDWORD(pak, (thisclient->items[(GETBYTE((*P), 0x0))].count));//amount
         ADDWORD(pak, 0x0000);
         ADDBYTE(pak,GETBYTE((*P), 0x2));
         ADDDWORD( pak, BuildItemHead( thisclient->items[GETBYTE((*P), 0x2)]) );
         ADDDWORD( pak, BuildItemData( thisclient->items[GETBYTE((*P), 0x2)]) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );   

    thisclient->client->SendPacket(&pak);
    }
    return true;
}

// Disconnect char
bool CWorldServer::pakCharDSClient( CPlayer* thisclient, CPacket* P )
{
    unsigned int userid = GETDWORD((*P), 1 );               
    CPlayer* otherclient = GetClientByUserID( userid );
    if(otherclient==NULL) return true;
    
    BYTE action = GETBYTE((*P),0);
    switch(action)
    {
        case 1:
        {
            if(otherclient==NULL)
            {
                Log(MSG_WARNING, "userid '%s' not found online", userid );
                return true;
            }
 
            BEGINPACKET( pak, 0x707 );
            ADDWORD( pak, 0 );
            otherclient->client->SendPacket( &pak );
 
            otherclient->client->isActive = false;                              
        }
        break;
    }     
    return true;    
}


// Pack Printscreen
bool CWorldServer::pakPrintscreen( CPlayer* thisclient, CPacket* P )
{
    BEGINPACKET( pak, 0x7eb );
    ADDWORD    ( pak, thisclient->Session->userid );
    ADDWORD    ( pak, 0x0302 );
    ADDWORD    ( pak, 0x2d17 );
    thisclient->client->SendPacket( &pak );
    return true;
}

// CharServer Connected
bool CWorldServer::pakCSReady ( CPlayer* thisclient, CPacket* P )
{
	if(thisclient->Session->isLoggedIn) return false;
	if(GETDWORD((*P),0) != Config.WorldPass ) 	
    {
        Log( MSG_HACK, "CharServer Tried to connect WorldServer with wrong password "); 
        return true;
    }
	thisclient->Session->isLoggedIn=true;
	return true;
}

// CharServer - Disconnect User
bool CWorldServer::pakCSCharSelect ( CPlayer* thisclient, CPacket* P )
{
    if(!thisclient->Session->isLoggedIn) return false;
	Log( MSG_INFO, "Char server requested client kill" );	
	DWORD userid = GETDWORD( (*P), 0 );	
	CPlayer* otherclient = GetClientByUserID( userid );
	if(otherclient!=NULL)
	{
        if(otherclient->client==NULL) return true;
        otherclient->client->isActive = false;
    }
	Log( MSG_INFO, "Client killed" );
	return true;
}

bool CWorldServer::pakItemMall( CPlayer* thisclient, CPacket* P )
{
    return true;
}
