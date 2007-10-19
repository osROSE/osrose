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


// Parse our commands to their appropriate function
bool CWorldServer::pakGMCommand( CPlayer* thisclient, CPacket* P )
{
            
	char* tmp;
	char* command = strtok( (char*)&(*P).Buffer[1] , " ");
	if (command==NULL) return true;
	if (strcmp(command, "here")==0)
	{
        if(Config.Command_Here > thisclient->Session->accesslevel)  
            return true;    

        return pakGMTele(thisclient, thisclient->Position->Map, thisclient->Position->current.x, thisclient->Position->current.y);
    }
    else
	if (strcmp(command, "tele")==0) // **** TELEPORT TO MAX AND X Y POINTS *****
    {
        if(Config.Command_Tele > thisclient->Session->accesslevel)
	                    return true;
		if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned map=atoi(tmp);
		if ((tmp = strtok(NULL, " "))==NULL) return true; float x=(float)atoi(tmp);
		if ((tmp = strtok(NULL, " "))==NULL) return true; float y=(float)atoi(tmp);
        
        Log( MSG_GMACTION, " %s : /tele %i,%u,%u" , thisclient->CharInfo->charname, map, round(x), round(y));
        return pakGMTele(thisclient, map, x, y);
    }
   else if (strcmp(command, "allskill")==0) /* Give all Skill to a Player - By CrAshInSiDe */
   {
        if(Config.Command_AllSkill > thisclient->Session->accesslevel)
        return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; char* name=tmp;
        Log( MSG_GMACTION, " %s : /allskill %s", thisclient->CharInfo->charname, name);
        return pakGMAllSkill(thisclient, name);
        }
   else if (strcmp(command, "delskills")==0) /* Remove all Skill from a Player - modified by rl2171 */
   {
        if(Config.Command_DelSkills > thisclient->Session->accesslevel)
        return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; char* name=tmp;
        Log( MSG_GMACTION, " %s : /delskills %s", thisclient->CharInfo->charname, name);
        return pakGMDelSkills(thisclient, name);
        }
/* GM List {By CrAshInSiDe} */
    else if(strcmp(command, "gmlist")==0) 
    {
        if(Config.Command_GmList > thisclient->Session->accesslevel)
            return true;       
        SendPM(thisclient, "The currently GM connected is:");
        int count=1;
        int hiddenam=0;
        char line0[200];
        while(count <= (ClientList.size()-1)) 
        {
            CPlayer* whoclient = (CPlayer*)ClientList.at(count)->player;
            if(whoclient->Session->accesslevel > 100) 
            {
                sprintf(line0, "%s - GM[%i]", whoclient->CharInfo->charname, whoclient->Session->accesslevel);                                      
            } 
 
            if(whoclient->isInvisibleMode != true) 
            {
                SendPM(thisclient, line0 );
            } 
            else 
            {
                hiddenam++;
            }
            count++;
        }
        sprintf(line0, "There are currently %i GM connected!", ((ClientList.size()-1)-hiddenam));
        Log( MSG_GMACTION, " %s : /gmlist" , thisclient->CharInfo->charname);
        SendPM(thisclient, line0 );
        return true;
    }
 
    /// geo edit for /savetown // 30 sep 07
    else if (strcmp(command, "savetown")==0)
    {
        if ((tmp = strtok(NULL, " ")) == NULL) tmp = 0; int loc=atoi(tmp);
        unsigned int spot = 0;
        switch(loc)
        {
            case 1:  //AP
            {
                spot = 1;
                break;
            }           
            case 2:  //Zant
            {
                spot = 2;
                break;
            }           
            case 3:  //Junon
            {
                spot = 4;
                break;
            }           
            case 4:  //Eucar
            {
                spot = 5;
                break;
            }           
            case 5:  //Xita
            {
                spot = 68;
                break;
            }           
        }   
        if(spot>0)
        {
            thisclient->Position->saved = spot;
            SendPM(thisclient, "Saved in Town");
        }
        else
        {      
            SendPM(thisclient, "Please input a number after the savetown command, below is a list of places and their appropriate number");
            SendPM(thisclient, "1 = Adventurers plains");
            SendPM(thisclient, "2 = The city of Zant");
            SendPM(thisclient, "3 = Junon Polis");
            SendPM(thisclient, "4 = The city of Eucar");
            SendPM(thisclient, "5 = Xita Refuge");
            SendPM(thisclient, "Example; /savetown 3");
        }
           
        return true;
    }
    /// end geo edit
   else if (strcmp(command, "mute")==0) //==== Mute a player ==== [by Paul_T]
   {
        if(Config.Command_Mute > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL)
                return true; 
        char* id=tmp;
        if ((tmp = strtok(NULL, " "))==NULL)
                return true;  
        int min=atoi(tmp);
        Log( MSG_GMACTION, " %s : /mute %s %i" , thisclient->CharInfo->charname, id, min);
        return pakGMMute(thisclient, id, min);       
    }
    else if (strcmp(command, "reborn")==0) //==== Reborn ==== (By Core)
   {
         if(Config.Command_Reborn > thisclient->Session->accesslevel)
                        return true;
 
        Log( MSG_GMACTION, " %s : /reborn" , thisclient->CharInfo->charname);   
        return pakGMReborn(thisclient);  
   }
   else if (strcmp(command, "event")==0) //==== Trigger Events (credit Welson)
   {
        if(Config.Command_Event > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL)
                return true; 
        int npctype=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
                return true;  
        int dialog=atoi(tmp);        
        if ((tmp = strtok(NULL, " "))==NULL)
                return true;  
        int type=atoi(tmp);
        Log( MSG_GMACTION, " %s : /event %i %i %i" ,thisclient->CharInfo->charname, npctype,dialog,type);
        return pakGMEventType(thisclient,npctype,dialog,type);
    }    
    else if (strcmp(command, "save")==0) // *** SAVE USER DATA *****
    {        
             if(Config.Command_Save > thisclient->Session->accesslevel)
	                    return true;
           thisclient->savedata();
           return true;		
	}
    else if (strcmp(command, "reload")==0) // *** RELOAD CONFIG.INI ******
    { 
         if(Config.Command_Reload > thisclient->Session->accesslevel)
	                    return true;
            LoadConfigurations( (char*)filename.c_str()  );
        Log( MSG_GMACTION, " %s : /reload" , thisclient->CharInfo->charname);
	        return true;
	}          
    else if (strcmp(command, "ann")==0) // *** SEND A ANNOUNCEMENT ***
    {
        if(Config.Command_Ann > thisclient->Session->accesslevel)
	       return true;
        Log( MSG_GMACTION, " %s : /ann %s" , thisclient->CharInfo->charname, &P->Buffer[5] );        
		return pakGMAnn(thisclient, P);						
    }
    /*else if(strcmp(command, "bodysize")==0)    
    {
        if ((tmp = strtok(NULL, " ")) == NULL) return true; UINT size=atoi(tmp);  
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 36);
        ADDWORD(pak, size);
        ADDWORD(pak, 0);
        thisclient->client->SendPacket(&pak);
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        thisclient->client->SendPacket(&pak);
        SendSysMsg(thisclient, "Body size changed!");                  
    }*/
    /*else if(strcmp(command, "headsize")==0)    
    {
        if ((tmp = strtok(NULL, " ")) == NULL) return true; UINT size=atoi(tmp);  
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 35);
        ADDWORD(pak, size);
        ADDWORD(pak, 0);
        thisclient->client->SendPacket(&pak);
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        thisclient->client->SendPacket(&pak);
        SendSysMsg(thisclient, "Head size changed!");                  
    }*/
    else if(strcmp(command, "face")==0)
    {
        if(Config.Command_Face > thisclient->Session->accesslevel)
	       return true;
        if ((tmp = strtok(NULL, " ")) == NULL) return true; UINT face=atoi(tmp);
        if (face > 12) return true;
        UINT style;
        if ((tmp = strtok(NULL, " ")) == NULL) style = 0; else style = atoi(tmp);
        if (style > 3) style = 3;
        thisclient->CharInfo->Face = ((face*7) + style + 1);
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 8);
        ADDWORD(pak, thisclient->CharInfo->Face);
        ADDWORD(pak, 0);
        thisclient->client->SendPacket(&pak);
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        thisclient->client->SendPacket(&pak);
        SendPM(thisclient, "Face changed!");
        DB->QExecute("UPDATE characters SET face=%i WHERE id=%i", thisclient->CharInfo->Face, thisclient->CharInfo->charid);
    }
    else if(strcmp(command, "hair")==0)
    {
        if(Config.Command_Hair > thisclient->Session->accesslevel)
	       return true;
        if ((tmp = strtok(NULL, " ")) == NULL) return true; UINT hair=atoi(tmp);
        if (hair > 6) return true;
        thisclient->CharInfo->Hair = (hair*5);
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 9);
        ADDWORD(pak, thisclient->CharInfo->Hair);
        ADDWORD(pak, 0);
        thisclient->client->SendPacket(&pak);
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        thisclient->client->SendPacket(&pak);
        DB->QExecute("UPDATE characters SET hairStyle=%i WHERE id=%i", thisclient->CharInfo->Hair, thisclient->CharInfo->charid);
        SendPM(thisclient, "Hair changed!");
    }
    else if(strcmp(command, "pvp")==0)    
    {
        if(Config.Command_Pvp > thisclient->Session->accesslevel)
	       return true;
        CMap* map = MapList.Index[thisclient->Position->Map];
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 34);
        if(map->allowpvp!=0)
        {
            map->allowpvp = 0;        
            ADDWORD(pak, 2);
            SendSysMsg(thisclient, "PVP off");  
        } 
        else 
        {
            map->allowpvp = 1;
            ADDWORD(pak, 51);
            SendSysMsg(thisclient, "PVP on");        
        }
        ADDWORD(pak, 0);
        SendToMap(&pak, thisclient->Position->Map); 
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        SendToMap(&pak, thisclient->Position->Map);         
    }
    else if(strcmp(command, "who")==0) 
    {
        if(Config.Command_Who > thisclient->Session->accesslevel)
            return true;
        int count=1;
        int gmcount=0;
        int playercount=0;
        char line0[200];
        while(count <= (ClientList.size()-1)) 
        {
            CPlayer* whoclient = (CPlayer*)ClientList.at(count)->player;
            if(whoclient->isInvisibleMode != true) 
            {
                    if(whoclient->Session->accesslevel > 100) 
                    {
                        gmcount++;                                      
                    } 
                    else 
                    {
                        playercount++;
                    }
            }
            count++;
        }
        sprintf(line0, "There are currently %i players and %i game moderators currently connected.", playercount, gmcount);
        Log( MSG_GMACTION, " %s : /who" , thisclient->CharInfo->charname);
        SendPM(thisclient, line0 );
        return true;                                              
    } 
    else if (strcmp(command, "go")==0) // AtCommandGo
    {
        if(Config.Command_go > thisclient->Session->accesslevel)
	       return true;
        if ((tmp = strtok(NULL, " ")) == NULL) tmp = 0; int loc=atoi(tmp);
        if(Config.Command_Go > thisclient->Session->accesslevel)
            return true;
        int x = 0;
        int y = 0;
        int map = 0;
        if(loc == 1)
        {
            map = 22;
            x = 5098;
            y = 5322;
        }
        else if(loc == 2)
        {
            map = 1;
            x = 5240;
            y = 5192;
        }
        else if (loc == 3)
        {
            map = 2;
            x = 5516;
            y = 5236;
        }
        else if (loc == 4)
        {
            map = 51;
            x = 5357;
            y = 5013;
        }
        else if (loc == 5)
        {
            map = 61;
            x = 5434;
            y = 4569;
        }
        else if (loc == 6)
        {
            map = 6;
            x = 5243;
            y = 5240;
        }
        else if (loc == 7)
        {
            map = 24;
            x = 5379;
            y = 5329;
        }
        else if (loc == 8)
        {
            map = 31;
            x = 5516;
            y = 5437;
        }
        else if (loc == 9)
        {
            map = 29;
            x = 5093;
            y = 5144;
        }
else if (loc == 10)
        {
             if (thisclient->Stats->Level<160) // by Terr0risT
             {
             SendPM(thisclient, "You need to be a least Level 160 to visit Sikuku Prison Underground!");
             return true;
             }
             else
             {
            map = 65;
            x = 5485;
            y = 5285;
             }
        }
        else
        {
            SendPM(thisclient, "Please input a number after the go command, below is a list of places and their appropriate number");
            SendPM(thisclient, "1 = Adventurers plains");
            SendPM(thisclient, "2 = The city of Zant");
            SendPM(thisclient, "3 = Junon Polis");
            SendPM(thisclient, "4 = The city of Eucar");
            SendPM(thisclient, "5 = Xita Refuge");
            SendPM(thisclient, "6 = Training grounds");
            SendPM(thisclient, "7 = El Verloon Desert");
            SendPM(thisclient, "8 = Goblin Cave (B1)");
            SendPM(thisclient, "9 = Desert of the Dead");
            SendPM(thisclient, "10 = Sikuku Underground Prison");
            SendPM(thisclient, "Example; /go 3");
        }
        if ( thisclient->Stats->HP < (thisclient->Stats->MaxHP / 2) || thisclient->Stats->HP < 1 || thisclient->Session->inGame == false )
        {
             SendPM(thisclient, "You need at least 50% HP in order to warp");
             return true;
        }

        if( (x != 0) && (y != 0) && (map != 0) )
        {
            fPoint coord;
            coord.x = x;
            coord.y = y;
            MapList.Index[map]->TeleportPlayer( thisclient, coord, false );
            Log( MSG_GMACTION, " %s : /go %i" , thisclient->CharInfo->charname, loc);
        }
        return true;
    }
    //******************************* START RESPAWN ***************************
    else if (strcmp(command, "SSPAWN")==0)
    { //STARTPOINT IDMOB CANTMIN CANTMAX RESPAWNTIME(s)  (3 points minim)
    if(Config.Command_SSpawn > thisclient->Session->accesslevel)
	                    return true;
        thisclient->GMRespawnPoints.map = thisclient->Position->Map;                
        if ((tmp = strtok(NULL, " "))==NULL)
        {
            thisclient->GMRespawnPoints.b=0;    
            return true; 
        }
        thisclient->GMRespawnPoints.mobID=atoi(tmp); 
        if ((tmp = strtok(NULL, " "))==NULL)
        {
            thisclient->GMRespawnPoints.b=0;    
            return true; 
        }
        thisclient->GMRespawnPoints.min=atoi(tmp);  
        if ((tmp = strtok(NULL, " "))==NULL)
        {
            thisclient->GMRespawnPoints.b=0;    
            return true; 
        }
        thisclient->GMRespawnPoints.max=atoi(tmp);   
        if ((tmp = strtok(NULL, " "))==NULL)
        {
            thisclient->GMRespawnPoints.b=0;    
            return true; 
        }
        thisclient->GMRespawnPoints.n=0;        
        thisclient->GMRespawnPoints.respawntime=atoi(tmp);                  
        thisclient->GMRespawnPoints.b=1;
		BEGINPACKET( pak, 0x702 );
		ADDSTRING( pak, "STARTPOINT" );
		ADDBYTE( pak, 0 );            
		thisclient->client->SendPacket(&pak);
		thisclient->GMRespawnPoints.d=0;
        return true;
    }
    else if (strcmp(command, "SET")==0) 
    {
         if(Config.Command_Set > thisclient->Session->accesslevel)
	                    return true;
        if(thisclient->GMRespawnPoints.b==1 && thisclient->GMRespawnPoints.n<50)        
        {         
            int n=thisclient->GMRespawnPoints.n;   
            thisclient->GMRespawnPoints.points[n].x=floor(thisclient->Position->destiny.x);
            thisclient->GMRespawnPoints.points[n].y=floor(thisclient->Position->destiny.y);            
            thisclient->GMRespawnPoints.n++;
            char text[10];
            sprintf(text,"--POINT #%i",thisclient->GMRespawnPoints.n);
		    BEGINPACKET( pak, 0x702 );
		    ADDSTRING( pak,text );
		    ADDBYTE( pak, 0 );            
		    thisclient->client->SendPacket(&pak);
            return true;
        }
        return true;
    }
    else if (strcmp(command, "ESPAWN")==0) 
    {
         if(Config.Command_ESpawn > thisclient->Session->accesslevel)
	                    return true;
        if(thisclient->GMRespawnPoints.n>3 && thisclient->GMRespawnPoints.b==1)
        {
            if ((tmp = strtok(NULL, " "))==NULL)
                return true; 
            int id=atoi(tmp);
            if ((tmp = strtok(NULL, " "))==NULL)
                return true; 
            int agressive=atoi(tmp);            
            int n=thisclient->GMRespawnPoints.n;
            char points[1000] = "";
            sprintf(&points[0],"%i",n);
            for(int i=0;i<n;i++)
            {
                sprintf(&points[strlen(points)],"|%.0f,%.0f",thisclient->GMRespawnPoints.points[i].x,thisclient->GMRespawnPoints.points[i].y);                
            }
            DB->QExecute("INSERT into list_spawnareas (id,map,montype,respawntime,points,min,max,agressive) values (%i,%i,%i,%i,'%s',%i,%i,%i)",
            id,thisclient->GMRespawnPoints.map, thisclient->GMRespawnPoints.mobID,thisclient->GMRespawnPoints.respawntime,
            points,thisclient->GMRespawnPoints.min,thisclient->GMRespawnPoints.max,agressive);
            thisclient->GMRespawnPoints.b=0;
            thisclient->GMRespawnPoints.d=1;    
		    BEGINPACKET( pak, 0x702 );
		    ADDSTRING( pak,"ENDSPAWN" );
		    ADDBYTE( pak, 0 );       
		    thisclient->client->SendPacket(&pak);
		    ReloadMobSpawn(thisclient, id );
            return true;
        }        
        return true;
    }    else if (strcmp(command, "DSPAWN")==0) 
    {
         if(Config.Command_DSpawn > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL)
            return true; 
        int id=atoi(tmp);         
        if ((tmp = strtok(NULL, " "))==NULL)
            return true; 
        thisclient->GMRespawnPoints.mobID=atoi(tmp); 
        if ((tmp = strtok(NULL, " "))==NULL)
            return true; 
        thisclient->GMRespawnPoints.min=atoi(tmp);  
        if ((tmp = strtok(NULL, " "))==NULL)
            return true; 
        thisclient->GMRespawnPoints.max=atoi(tmp);    
        if ((tmp = strtok(NULL, " "))==NULL)
            return true; 
        thisclient->GMRespawnPoints.respawntime=atoi(tmp);                  
        if ((tmp = strtok(NULL, " "))==NULL)
            return true; 
        int agressive = atoi(tmp);        
        if(thisclient->GMRespawnPoints.n>3 && thisclient->GMRespawnPoints.d==1)
        {
            int n=thisclient->GMRespawnPoints.n;
            char points[1000] = "";
            sprintf(&points[0],"%i",n);
            for(int i=0;i<n;i++)
            {
                sprintf(&points[strlen(points)],"|%.0f,%.0f",thisclient->GMRespawnPoints.points[i].x,thisclient->GMRespawnPoints.points[i].y);                
            }
            DB->QExecute("INSERT into list_spawnareas (id,map,montype,respawntime,points,min,max,agressive) values (%i,%i,%i,%i,'%s',%i,%i,%i)",
            id,thisclient->GMRespawnPoints.map,
            thisclient->GMRespawnPoints.mobID,thisclient->GMRespawnPoints.respawntime,
            points,thisclient->GMRespawnPoints.min,thisclient->GMRespawnPoints.max,agressive);
            thisclient->GMRespawnPoints.b=0;
		    BEGINPACKET( pak, 0x702 );
		    ADDSTRING( pak, "DUPESPAWN" );
		    ADDBYTE( pak, 0 );              
		    thisclient->client->SendPacket(&pak);
		    ReloadMobSpawn(thisclient, id );
            return true;
        }        
        return true;
    }  
    else if(strcmp(command, "DELETESPAWN")==0)
    {
         if(Config.Command_DelSpawn > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL)
             return true; 
        int id=atoi(tmp);    
        DB->QExecute("DELETE from list_spawnareas where id=%i",id);
		BEGINPACKET( pak, 0x702 );
		ADDSTRING( pak, "DELETESPAWN" );
		ADDBYTE( pak, 0 );             
        thisclient->client->SendPacket(&pak);  
        CSpawnArea* thisspawn = GetSpawnArea( id );  
        if(thisspawn==NULL)
            return true;        
        CMap* map = MapList.Index[thisspawn->map];
        for(UINT i=0;i<map->MonsterList.size();i++)
        {
            CMonster* thismon = map->MonsterList.at(i);
            BEGINPACKET( pak, 0x794 );
            ADDWORD    ( pak, thismon->clientid );
            SendToVisible( &pak, thismon );
            MapList.Index[thisspawn->map]->DeleteMonster( thismon );
        }  
        DeleteSpawn( thisspawn );
        Log( MSG_GMACTION, " %s : /deletespawn %i" , thisclient->CharInfo->charname, id);               
    }//******************************** FINISH RESPAWN ***************************
    else if(strcmp(command, "p")==0)  //*** READ THE PACKET.TXT AND SEND IT
    {
         if(Config.Command_Pak > thisclient->Session->accesslevel)
	                    return true;
         if ((tmp = strtok(NULL, " "))==NULL)
             return true; 	        
            char buffer;              
            std::string filename = "packet/packet";
            filename += tmp;
            filename += ".txt";               
            FILE *packet1 = fopen(filename.c_str(),"r");                                            
            if(packet1==NULL)
            {
                printf("Error opening packet.txt!\n");
                return true;
            }
            unsigned int command = 0;
            fread( &command, 1, 2, packet1 );
            BEGINPACKET(pak,command);
            while((fscanf(packet1,"%c",&buffer))!=EOF)
                    ADDBYTE(pak,buffer);
    		thisclient->client->SendPacket( &pak );    
            fclose(packet1);     
    }     
    else if (strcmp(command, "level")==0)
    {
         if(Config.Command_Level > thisclient->Session->accesslevel)
                        return true;
        char* name;
        if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned level=atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) name = thisclient->CharInfo->charname; else name = tmp;
        Log( MSG_GMACTION, " %s : /level %i %s" , thisclient->CharInfo->charname, level, name);
        return pakGMLevel( thisclient, level, name );
    }
    else if (strcmp(command, "info")==0) 
    {       
            if(Config.Command_Info > thisclient->Session->accesslevel)
	                    return true;
            Log( MSG_GMACTION, " %s : /info" , thisclient->CharInfo->charname);        
            thisclient->GetPlayerInfo( );		    
            return true;
	}	
    else if (strcmp(command, "exp")==0)
    {
         if(Config.Command_Exp > thisclient->Session->accesslevel)
                        return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned exp=atoi(tmp);
        char* name;
        if ((tmp = strtok(NULL, " "))==NULL)
            name = thisclient->CharInfo->charname;
        else
            name = tmp;
        CPlayer* otherclient = GetClientByCharName( name );
        if (otherclient == NULL)
            return true;
        otherclient->CharInfo->Exp += exp;
        BEGINPACKET( pak, 0x79b );
        ADDDWORD   ( pak, otherclient->CharInfo->Exp );
        ADDWORD    ( pak, otherclient->CharInfo->stamina );
        ADDWORD    ( pak, 0 );
        otherclient->client->SendPacket( &pak );
        Log( MSG_GMACTION, " %s : /exp %i %s" , thisclient->CharInfo->charname, exp, name);
    }
    else if (strcmp(command, "mon")==0) {
         if(Config.Command_Mon > thisclient->Session->accesslevel)
	                    return true;
		if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned montype=atoi(tmp);
		if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned moncount=atoi(tmp);
		Log( MSG_GMACTION, " %s : /mon %i,%i" , thisclient->CharInfo->charname, montype, moncount);
		return pakGMMon( thisclient, montype, moncount );
	}
    else if (strcmp(command, "mon2")==0)
    {    //Spawn "x" monsters
         if(Config.Command_Mon2 > thisclient->Session->accesslevel)
	                    return true;
		if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned montype=atoi(tmp);
		if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned moncount=atoi(tmp);
		
		for (int k=0;k<moncount;k++)
	    {
             Log( MSG_GMACTION, " %s : /mon2 %i,%i" , thisclient->CharInfo->charname, montype+k, moncount); 
             pakGMMon( thisclient, montype+k, 1 );            
        }
				
		return true;
	} 
    else if (strcmp(command, "kick")==0)
    {
        if(Config.Command_Kick > thisclient->Session->accesslevel)
	       return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; char* name=tmp;
        Log( MSG_GMACTION, " %s : /kick %s" , thisclient->CharInfo->charname, name);        
        return pakGMKick( thisclient, name );
    }
    else if (strcmp(command, "job")==0) // *** Change Job *****
    { 
         if(Config.Command_Job > thisclient->Session->accesslevel)
	       return true;
               char* tmp;
               if ((tmp = strtok(NULL, " "))==NULL) return true; char* job=tmp;
               Log( MSG_GMACTION, " %s : /job %s" , thisclient->CharInfo->charname, job);
               if(thisclient->CharInfo->Job != 0 || thisclient->Stats->Level<10)
                    return true;
               if(strcmp(job,"soldier")==0)
               {
                    BEGINPACKET(pak, 0x721);
                    ADDWORD    (pak, 0x0004);
                    ADDWORD    (pak, 0x006f);
                    ADDWORD    (pak, 0x0000);
                    thisclient->client->SendPacket(&pak);

                    RESETPACKET( pak, 0x730);
                    ADDBYTE(pak, 0x05);
                    ADDBYTE(pak, 0x00);    
                    ADDWORD(pak, 0xf52f);
                    ADDWORD(pak, 0x2964);
                	thisclient->client->SendPacket( &pak );                                            
                    thisclient->CharInfo->Job=111;                       
               }
               else
               if(strcmp(job,"muse")==0)
               {
                    BEGINPACKET(pak, 0x721);
                    ADDWORD    (pak, 0x0004);
                    ADDWORD    (pak, 0x00D3);
                    ADDWORD    (pak, 0x0000);
                    thisclient->client->SendPacket(&pak); 

                    RESETPACKET( pak, 0x730);
                    ADDBYTE(pak, 0x05);
                    ADDBYTE(pak, 0x00);    
                    ADDWORD(pak, 0xdc3b);
                    ADDWORD(pak, 0x20cd);
                	thisclient->client->SendPacket( &pak );                       
                    thisclient->CharInfo->Job=211;                                            
               }   
               else
               if(strcmp(job,"hawker")==0)
               {
                     BEGINPACKET(pak, 0x721);
                     ADDWORD    (pak, 0x0004);
                     ADDWORD    (pak, 0x0137);
                     ADDWORD    (pak, 0x0000);
                     thisclient->client->SendPacket(&pak);  

                     RESETPACKET( pak, 0x730);
                     ADDBYTE(pak, 0x05);
                     ADDBYTE(pak, 0x00);    
                     ADDWORD(pak, 0x1104);
                     ADDWORD(pak, 0x5150);
                	 thisclient->client->SendPacket( &pak );                                             
                     thisclient->CharInfo->Job=311;                                      
               }                                            
               else               
               if(strcmp(job,"dealer")==0)
               {
                    BEGINPACKET(pak, 0x721);
                    ADDWORD    (pak, 0x0004);
                    ADDWORD    (pak, 0x019b);
                    ADDWORD    (pak, 0x0000);
                    thisclient->client->SendPacket(&pak); 

                    RESETPACKET( pak, 0x730);
                    ADDBYTE(pak, 0x05);
                    ADDBYTE(pak, 0x00);    
                    ADDWORD(pak, 0x4c1c);
                    ADDWORD(pak, 0xef69);
                	thisclient->client->SendPacket( &pak );                        
                    thisclient->CharInfo->Job=411;                                            
          }             
     }        
     else if (strcmp(command, "set")==0) 
     {      
        if(Config.Command_Set > thisclient->Session->accesslevel)
                    return true; 
        int refine;
        if ((tmp = strtok(NULL, " "))==NULL) 
            return true; 
        int id=atoi(tmp);                         
        if ((tmp = strtok(NULL, " "))==NULL) 
            refine = 0; 
        else
            refine=atoi(tmp); 
        refine *= 16;
        BEGINPACKET( pak, 0);
        if(EquipList[2].Index[id]!=NULL)
        {  
            thisclient->items[2].itemnum = id;
    		thisclient->items[2].itemtype = 2;
    		thisclient->items[2].refine =refine;
    		thisclient->items[2].durability = 50;
    		thisclient->items[2].lifespan = 100;
    		thisclient->items[2].count = 1;
    		thisclient->items[2].stats = 0;
    		thisclient->items[2].socketed = false;
    		thisclient->items[2].appraised = true;	
        	thisclient->items[2].gem = 0;   
            thisclient->UpdateInventory( 2 );                  	
            
    	    RESETPACKET( pak, 0x7a5);     
    	    ADDWORD( pak, thisclient->clientid );
    	    ADDWORD( pak, 0x0002);							
    	    ADDWORD( pak, id);	// ITEM NUM
    	    ADDWORD( pak, BuildItemRefine( thisclient->items[2] ) );	// REFINE
    	    ADDWORD( pak, thisclient->Stats->Move_Speed );	// REFINE
    	    SendToVisible( &pak,thisclient );             
        }
    	if(EquipList[3].Index[id]!=NULL)
    	{
    		thisclient->items[3].itemnum = id;
    		thisclient->items[3].itemtype = 3;
    		thisclient->items[3].refine =refine;
    		thisclient->items[3].durability = 50;
    		thisclient->items[3].lifespan = 100;
    		thisclient->items[3].count = 1;
    		thisclient->items[3].stats = 0;
    		thisclient->items[3].socketed = false;
    		thisclient->items[3].appraised = true;	
        	thisclient->items[3].gem = 0;    
            thisclient->UpdateInventory( 3 );               
            
	        RESETPACKET( pak, 0x7a5);     
    	    ADDWORD( pak, thisclient->clientid );
    	    ADDWORD( pak, 0x0003);							
    	    ADDWORD( pak, id);	// ITEM NUM
    	    ADDWORD( pak, BuildItemRefine( thisclient->items[3] ));	// REFINE
    	    ADDWORD( pak, thisclient->Stats->Move_Speed );	// REFINE
    	    SendToVisible( &pak,thisclient );                
        }
        if(EquipList[4].Index[id]!=NULL)
        {
    		thisclient->items[5].itemnum = id;
    		thisclient->items[5].itemtype = 4;
    		thisclient->items[5].refine =refine;
    		thisclient->items[5].durability = 50;
    		thisclient->items[5].lifespan = 100;
    		thisclient->items[5].count = 1;
    		thisclient->items[5].stats = 0;
    		thisclient->items[5].socketed = false;
    		thisclient->items[5].appraised = true;	
        	thisclient->items[5].gem = 0;   
                   		
            thisclient->UpdateInventory( 4 );	
            
    	    RESETPACKET( pak, 0x7a5);     
     	    ADDWORD( pak, thisclient->clientid );            	               
	        ADDWORD( pak, 5);							
    	    ADDWORD( pak, id);	// ITEM NUM
    	    ADDWORD( pak, BuildItemRefine( thisclient->items[5] ));	// REFINE
    	    ADDWORD( pak, thisclient->Stats->Move_Speed );	// REFINE 2602
    	    SendToVisible( &pak,thisclient );             		
        }
        if(EquipList[5].Index[id]!=NULL)		
        {
    		thisclient->items[6].itemnum = id;
    		thisclient->items[6].itemtype = 5;
    		thisclient->items[6].refine =refine;
    		thisclient->items[6].durability = 50;
    		thisclient->items[6].lifespan = 100;
    		thisclient->items[6].count = 1;
      		thisclient->items[6].stats = 0;
    		thisclient->items[6].socketed = false;
    		thisclient->items[6].appraised = true;	
        	thisclient->items[6].gem = 0;  
                             
            thisclient->UpdateInventory( 6 );            

    	    RESETPACKET( pak, 0x7a5);     
    	    ADDWORD( pak, thisclient->clientid );
    	    ADDWORD( pak, 6);							
    	    ADDWORD( pak, id);	// ITEM NUM
    	    ADDWORD( pak, BuildItemRefine( thisclient->items[6] ));	// REFINE
    	    ADDWORD( pak, thisclient->Stats->Move_Speed);	// REFINE
    	    SendToVisible( &pak,thisclient );
        }  	        
        thisclient->SetStats( );	         	   
    }
    else if (strcmp(command, "cha")==0) 
    {
         if(Config.Command_Cha > thisclient->Session->accesslevel)
	                    return true;
        int slot;
        int tipo;
        int id;
        int stats;
        int socket;
        int refine;
        if ((tmp = strtok(NULL, " "))==NULL)return true;
        if(strcmp(tmp, "mask")==0)
        {
            slot=1;    
            tipo=1;
        }
        else                    
        if(strcmp(tmp, "cap")==0)
        {
            slot=2;                    
            tipo=2;
        }
        else            
        if(strcmp(tmp, "suit")==0)
        {
            slot=3; 
            tipo=3;
        }
        else            
        if(strcmp(tmp, "back")==0)
        {
            slot=4;                                   
            tipo=6;
        }
        else            
        if(strcmp(tmp, "glov")==0)
        {
            slot=5;              
            tipo=4;
        }
        else            
        if(strcmp(tmp, "shoe")==0)
        {
            slot=6;                                                         
            tipo=5;
        }
        else                            
        if(strcmp(tmp, "weap")==0)
        {
            slot=7;           
            tipo=8;
        }
        else            
        if(strcmp(tmp, "shield")==0)
        {
            slot=8;     
            tipo=9;
        }
        else
            return true;                                                                                            
        if ((tmp = strtok(NULL, " "))==NULL)return true;
            id=atoi(tmp);       
        if ((tmp = strtok(NULL, " "))==NULL)
            stats=0;
        else
            stats=atoi(tmp);      
        if ((tmp = strtok(NULL, " "))==NULL)
            socket=0;
        else
            socket=1;                            
        if ((tmp = strtok(NULL, " "))==NULL)
            refine=0;
        else
            refine=atoi(tmp);                                        
	   thisclient->items[slot].itemnum = id;
	   thisclient->items[slot].itemtype = tipo;
	   thisclient->items[slot].refine = refine;
	   thisclient->items[slot].durability = 50;
	   thisclient->items[slot].lifespan = 100;
	   thisclient->items[slot].count = 1;
	   thisclient->items[slot].stats = stats;
	   thisclient->items[slot].socketed = socket;
	   thisclient->items[slot].appraised = true; 
	   thisclient->items[slot].gem = 0;               
	   BEGINPACKET( pak, 0x7a5);     
	   ADDWORD( pak, thisclient->clientid );
	   ADDWORD( pak, slot);							
	   ADDWORD( pak, id);	// ITEM NUM
	   ADDWORD( pak, BuildItemRefine(  thisclient->items[slot] ));	// REFINE
	   ADDWORD( pak, thisclient->Stats->Move_Speed );	// REFINE 2602	   
	   SendToVisible( &pak,thisclient );        	         
	   
      thisclient->UpdateInventory( slot );	   	   
       thisclient->SetStats( );                                  
		return true;	
    }    
    else if (strcmp(command, "item")==0)//Modified by Hiei (added refine/socket/stats)
    {
         if(Config.Command_Item > thisclient->Session->accesslevel)
	                    return true;
        UINT itemrefine, itemstats, itemls, itemsocket;
        if ((tmp = strtok(NULL, " "))==NULL) return true; UINT itemid =atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true; UINT itemtype =atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true; UINT itemamount =atoi(tmp);            
        if ((tmp = strtok(NULL, " "))==NULL)
            itemrefine =0;
        else
            itemrefine = atoi(tmp)<10?atoi(tmp)*16:9*16;
        if ((tmp = strtok(NULL, " "))==NULL)
            itemls =100;
        else
            itemls = atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            itemsocket =0;
        else
            itemsocket =atoi(tmp)==0?false:true;
        if ((tmp = strtok(NULL, " "))==NULL)
            itemstats =0;
        else
            itemstats =atoi(tmp);
        Log( MSG_GMACTION, " %s : /item %i,%i,%i,%i,%i,%i" , thisclient->CharInfo->charname, itemid, itemtype, itemamount , itemrefine , itemsocket ,itemstats);
        return pakGMItem( thisclient , itemid , itemtype , itemamount , itemrefine , itemls, itemstats , itemsocket );        
    }
    else if (strcmp(command, "levelup")==0) 
    {
         if(Config.Command_LevelUp > thisclient->Session->accesslevel)
	            return true;
        Log( MSG_GMACTION, " %s : /levelup" , thisclient->CharInfo->charname);
		thisclient->CharInfo->Exp += thisclient->GetLevelEXP();
	} 
    else if (strcmp(command, "drop")==0) 
    {
         if(Config.Command_Drop > thisclient->Session->accesslevel)
	             return true;
		if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned itemtype=atoi(tmp);
		if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned itemid=atoi(tmp);
        Log( MSG_GMACTION, " %s : /drop %i,%i" , thisclient->CharInfo->charname, itemtype, itemid);

		CDrop* thisdrop = new CDrop;
		assert(thisdrop);
		thisdrop->clientid = GetNewClientID();
		thisdrop->type = 2; 
		thisdrop->pos.x = thisclient->Position->current.x;
		thisdrop->pos.y = thisclient->Position->current.y;
		thisdrop->posMap = thisclient->Position->Map;
		thisdrop->droptime = time(NULL);

		ClearItem(thisdrop->item);
		thisdrop->item.itemnum = itemid;
		thisdrop->item.itemtype = itemtype;
		thisdrop->item.count = 1;
		thisdrop->item.refine = 0;
		thisdrop->item.durability = 35;
		thisdrop->item.lifespan = 100;
		thisdrop->item.appraised = true;
		thisdrop->item.socketed = false;
		thisdrop->item.stats = 0;
		thisdrop->item.gem = 0;

		thisdrop->amount = 1;
		thisdrop->owner = 0;
		CMap* map = MapList.Index[thisdrop->posMap];
		map->AddDrop( thisdrop );
	}    
    else if(strcmp(command, "givezuly")==0) 
    {
         if(Config.Command_GiveZuly > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; char* name=tmp;
		if((tmp = strtok(NULL, " "))==NULL) return true; int zuly=atoi(tmp);
        Log( MSG_GMACTION, " %s : /givezuly %s, %i" , thisclient->CharInfo->charname, name, zuly);		
		  return pakGMZulygive(thisclient, name, zuly);
	}
    else if(strcmp(command, "npc")==0) 
    {
         if(Config.Command_Npc > thisclient->Session->accesslevel)
	                    return true;
		if((tmp = strtok(NULL, " "))==NULL) return true; unsigned npcid=atoi(tmp);
		unsigned npcdialog = 0;
		if((tmp = strtok(NULL, " "))!=NULL) 
            npcdialog=atoi(tmp);
        Log( MSG_GMACTION, " %s : /npc %i, %i" , thisclient->CharInfo->charname, npcid, npcdialog);
        return pakGMNpc(thisclient, npcid, npcdialog);
	}
    else if(strcmp(command, "givefairy")==0) 
    {
         if(Config.Command_GiveFairy > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; char* name=tmp;
	    if((tmp = strtok(NULL, " "))==NULL) return true; unsigned mode=atoi(tmp);
	    Log( MSG_GMACTION, " %s : /givefairy %s, %i" , thisclient->CharInfo->charname, name, mode);
	    return pakGMFairyto(thisclient, name, mode);
	}
	else if(strcmp(command, "fairymode")==0) 
    {
        if(Config.Command_ManageFairy > thisclient->Session->accesslevel)
	                    return true;
	    if((tmp = strtok(NULL, " "))==NULL) return true; unsigned mode=atoi(tmp);
	    Log( MSG_GMACTION, " %s : /fairymode mode: %i" , thisclient->CharInfo->charname, mode);
	    return pakGMManageFairy(thisclient, mode);
	}
	else if(strcmp(command, "fairywait")==0) 
    {
        if(Config.Command_ChangeFairyWait > thisclient->Session->accesslevel)
	                    return true;
	    if((tmp = strtok(NULL, " "))==NULL) return true; unsigned value=atoi(tmp);
	    Log( MSG_GMACTION, " %s : /fairywait value: %i" , thisclient->CharInfo->charname, value);
	    return pakGMChangeFairyWait(thisclient, value);
	}
	else if(strcmp(command, "fairystay")==0) 
    {
        if(Config.Command_ChangeFairyStay > thisclient->Session->accesslevel)
	                    return true;
	    if((tmp = strtok(NULL, " "))==NULL) return true; unsigned value=atoi(tmp);
	    Log( MSG_GMACTION, " %s : /fairystay value: %i" , thisclient->CharInfo->charname, value);
	    return pakGMChangeFairyStay(thisclient, value);
	}
	else if(strcmp(command, "fairytestmode")==0) 
    {
        if(Config.Command_ChangeFairyTestMode > thisclient->Session->accesslevel)
	                    return true;
	    if((tmp = strtok(NULL, " "))==NULL) return true; unsigned mode=atoi(tmp);
	    Log( MSG_GMACTION, " %s : /fairytestmode mode: %i" , thisclient->CharInfo->charname, mode);
	    return pakGMChangeFairyTestMode(thisclient, mode);
	}
    else if (strcmp(command, "move")==0) 
    {
         if(Config.Command_Move > thisclient->Session->accesslevel)
	                    return true;
		if ((tmp = strtok(NULL, " "))==NULL) return true; char* name=tmp;
		if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned map=atoi(tmp);
		if ((tmp = strtok(NULL, " "))==NULL) return true; float x=(float)atoi(tmp);
		if ((tmp = strtok(NULL, " "))==NULL) return true; float y=(float)atoi(tmp);
		Log( MSG_GMACTION, " %s : /move %s,%i,%i,%i" , thisclient->CharInfo->charname, name, map, x, y);
		return pakGMTeleOtherPlayer(thisclient, name, map, x, y); 
	}
    else if (strcmp(command, "goto")==0) 
    {
         if(Config.Command_Goto > thisclient->Session->accesslevel)
	                    return true;
		if ((tmp = strtok(NULL, " "))==NULL) return true; char* name=tmp;
		Log( MSG_GMACTION, " %s : /goto %s" , thisclient->CharInfo->charname, name);
		return pakGMTeleToPlayer(thisclient, name);
	}
    else if (strcmp(command, "teletome")==0)
    {
         if(Config.Command_TeleToMe > thisclient->Session->accesslevel)
	                    return true;
		if ((tmp = strtok(NULL, " "))==NULL) return true; char* name=tmp;
		Log( MSG_GMACTION, " %s : /teletome %s" , thisclient->CharInfo->charname, name);
        if(strcmp(name,"all")==0) 
        {
            return pakGMTeleAllHere(thisclient);    
        }		        
		return pakGMTelePlayerHere(thisclient, name);
    }
    else if(strcmp(command, "CharInfo")==0) 
    {
         if(Config.Command_PlayerInfo > thisclient->Session->accesslevel)
	                    return true;
			if((tmp = strtok(NULL, " "))==NULL) return true; char* name=tmp;
			Log( MSG_GMACTION, " %s : /CharInfo %s" , thisclient->CharInfo->charname, name);
			return pakGMInfo(thisclient, name);
	}
else if (strcmp(command, "give2")==0)
{   
             if(Config.Command_Item > thisclient->Session->accesslevel)
                      return true;
        UINT itemrefine, itemstats, itemls, itemsocket;
        if ((tmp = strtok(NULL, " "))==NULL) return true; char* name=tmp;
        if ((tmp = strtok(NULL, " "))==NULL) return true; UINT itemid =atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true; UINT itemtype =atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true; UINT itemamount =atoi(tmp);            
        if ((tmp = strtok(NULL, " "))==NULL)
            itemrefine =0;
        else
            itemrefine = atoi(tmp)<10?atoi(tmp)*16:9*16;
        if ((tmp = strtok(NULL, " "))==NULL)
            itemls =100;
        else
            itemls = atoi(tmp);
        if ((tmp = strtok(NULL, " "))==NULL)
            itemsocket =0;
        else
            itemsocket =atoi(tmp)==0?false:true;
        if ((tmp = strtok(NULL, " "))==NULL)
            itemstats =0;
        else
            itemstats =atoi(tmp); 
        Log( MSG_GMACTION, " %s : /give2 %s,%i,%i,%i,%i,%i,%i,%i" , thisclient->CharInfo->charname, name, itemid , itemtype , itemamount , itemrefine , itemls, itemstats , itemsocket); 
        return pakGMItemtoplayer( thisclient , name , itemid , itemtype , itemamount , itemrefine , itemls, itemstats , itemsocket ); 
}
    else if (strcmp(command, "ban")==0)
    {
         if(Config.Command_Ban > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; char* name=tmp;
        Log( MSG_GMACTION, " %s : /ban %s" , thisclient->CharInfo->charname, name);
        return pakGMBan( thisclient, name );
	}            
    else if (strcmp(command, "ani")==0)
    {
         if(Config.Command_Ani > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; 
            unsigned anid =atoi(tmp);
            Log( MSG_GMACTION, " %s : /ani %i" , thisclient->CharInfo->charname, anid);
            pakGMDoEmote( thisclient, anid );
    }
    else if(strcmp(command,"summon")==0)
    {
        if(Config.Command_Summon > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; 
        unsigned int summon = atoi(tmp);   
        Log( MSG_GMACTION, " %s : /summon %i" , thisclient->CharInfo->charname, summon);
        fPoint position = RandInCircle( thisclient->Position->current, 5 );
        CMap* map = MapList.Index[thisclient->Position->Map];
        map->AddMonster( summon, position, thisclient->clientid );
        //Start Animation
        BEGINPACKET( pak, 0x7b2 );
        ADDWORD    ( pak, thisclient->clientid );
        ADDWORD    ( pak, 2802 );                        
        SendToVisible( &pak, thisclient );
        //Finish Animation
        RESETPACKET( pak, 0x7bb );
        ADDWORD    ( pak, thisclient->clientid );
        SendToVisible( &pak, thisclient );    
        //????
        RESETPACKET( pak, 0x7b9);
        ADDWORD    ( pak, thisclient->clientid);
        ADDWORD    ( pak, 1286 );
	    SendToVisible( &pak, thisclient );   
	    // Add our Mob to the mobs list                       
    }
    else if (strcmp(command, "reloadquest")==0)
    {
         if(Config.Command_ReloadQuest > thisclient->Session->accesslevel)
	                    return true;
        QuestList.clear( );
        LoadQuestData( );
        Log( MSG_GMACTION, " %s : /reloadquest" , thisclient->CharInfo->charname);        
        return true;
    }
	else if (strcmp(command, "shutdown")==0)
	{        
             if(Config.Command_Shutdown > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; 
            unsigned int minutes =atoi(tmp);        
        char text[200];
        sprintf( text, "Server will shutdown in %i minutes, Please logout NOW to be sure your information is saved correctly.",
           minutes );        
        BEGINPACKET( pak, 0x702 );
	    ADDSTRING  ( pak, thisclient->CharInfo->charname );
    	ADDSTRING  ( pak, "> " );        
    	ADDSTRING  ( pak, text );
    	ADDBYTE    ( pak, 0x00 );
    	SendToAll  ( &pak );
        pthread_create( &WorldThread[SHUTDOWN_THREAD], NULL, ShutdownServer, (PVOID)minutes); 
        Log( MSG_GMACTION, " %s : /shutdown %u" , thisclient->CharInfo->charname, shutdown);        
    }     
    else if(strcmp(command, "dquest")==0)
    {                          
         if(Config.Command_DQuest > thisclient->Session->accesslevel)
	                    return true;
        char line0[200];
                        
        if( thisclient->questdebug )
        {
            Log(MSG_INFO,"Quest Debug deactivated");
            sprintf(line0, "Quest Debug deactivated");
            thisclient->questdebug = false;
        }
        else
        {
            Log(MSG_INFO,"Quest Debug activated");
            sprintf(line0, "Quest Debug activated");
            thisclient->questdebug = true;
        }
        
        SendPM(thisclient, line0 );        
    }     
    else if(strcmp(command, "iquest")==0)    
    {
         if(Config.Command_IQuest > thisclient->Session->accesslevel)
	                    return true;
        int n=1;
        if ((tmp = strtok(NULL, " "))==NULL) return true; 
        unsigned int itemquest =atoi(tmp); 
        if ((tmp = strtok(NULL, " "))!=NULL)
            n=atoi(tmp);
        if( thisclient->questdebug )
        {
            for(int i=0;i<n;i++)
            {
                BEGINPACKET( pak, 0x731 )
                ADDWORD    ( pak, itemquest );
                thisclient->client->SendPacket( &pak );            
            }
        }
    } 
	else if (strcmp(command, "shoptype")==0)
	{    
         if(Config.Command_ShopType > thisclient->Session->accesslevel)
	                    return true;    
        if ((tmp = strtok(NULL, " "))==NULL) return true; 
            unsigned int shoptype =atoi(tmp);           
        thisclient->Shop->ShopType = shoptype;
        Log( MSG_GMACTION, " %s : /shoptype %i" , thisclient->CharInfo->charname, shoptype);        
    }     
    else if (strcmp(command, "stat")==0) /// Code By Minoc
    {
         if(Config.Command_Stat > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; char* statname =(char*)tmp;
        if ((tmp = strtok(NULL, " "))==NULL) return true; int statvalue    = atoi(tmp);
        if (statvalue > Config.MaxStat)  /// code by PurpleYouko for setting max stat to 300
            statvalue = Config.MaxStat;  /// extra code to match setting in worldserver.conf by Atomsk
        Log( MSG_GMACTION, " %s : /stat %s,%i" , thisclient->CharInfo->charname, statname, statvalue);
        return pakGMStat( thisclient , statname , statvalue);
    }
    else if (strcmp(command, "statp")==0)
    {
        if (Config.Command_Stat > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; int statvalue = atoi(tmp);
        char* name;
        if ((tmp = strtok(NULL, " "))==NULL)
            name = thisclient->CharInfo->charname;
        else
            name = tmp;
        CPlayer* otherclient = GetClientByCharName(name);
        if (otherclient == NULL)
            return true;
        if ((int)otherclient->CharInfo->StatPoints + statvalue < 0)
            otherclient->CharInfo->StatPoints = 0;
        else
            otherclient->CharInfo->StatPoints += statvalue;
        BEGINPACKET( pak, 0x720 );
        ADDWORD( pak, 32 );
        ADDWORD( pak, statvalue );
        ADDWORD( pak, 0 );
        otherclient->client->SendPacket( &pak );
        RESETPACKET( pak, 0x0730 );
        ADDWORD( pak, 5 );
        ADDWORD( pak, 0xa24d );
        ADDWORD( pak, 0x40b3 );
        otherclient->client->SendPacket( &pak );
        Log(MSG_GMACTION, " %s : /stats %i %s", thisclient->CharInfo->charname, statvalue, name);
        return true;
    }
    else if (strcmp(command, "skillp")==0)
    {
        if (Config.Command_Stat > thisclient->Session->accesslevel)
            return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; int skillp = atoi(tmp);
        char* name;
        if ((tmp = strtok(NULL, " "))==NULL)
            name = thisclient->CharInfo->charname;
        else
            name = tmp;
        CPlayer* otherclient = GetClientByCharName(name);
        if (otherclient == NULL)
            return true;
        if ((int)otherclient->CharInfo->SkillPoints + skillp < 0)
            otherclient->CharInfo->SkillPoints = 0;
        else
            otherclient->CharInfo->SkillPoints += skillp;
        BEGINPACKET( pak, 0x720 );
        ADDWORD( pak, 37 );
        ADDWORD( pak, skillp );
        ADDWORD( pak, 0 );
        otherclient->client->SendPacket( &pak );
        RESETPACKET( pak, 0x0730 );
        ADDWORD( pak, 5 );
        ADDWORD( pak, 0xa24d );
        ADDWORD( pak, 0x40b3 );
        otherclient->client->SendPacket( &pak );
        Log(MSG_GMACTION, " %s : /skillp %i %s", thisclient->CharInfo->charname, skillp, name);
        return true;
    }
    else if (strcmp(command, "killinrange")==0)
    {
        if(Config.Command_KillInRange > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned range=atoi(tmp);
        Log( MSG_GMACTION, " %s : /killinrange %i" , thisclient->CharInfo->charname, range);
        return pakGMKillInRange( thisclient, range );
    }    
    else if (strcmp(command, "gotomap")==0) // *** TELEPORT WITH MAP ID *****
	{	 // credits to Blackie
	   if(Config.Command_GoToMap > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; unsigned map=atoi(tmp);
        Log( MSG_GMACTION, " %s : /gotomap %i" , thisclient->CharInfo->charname, map);        
		return pakGMGotomap(thisclient, map);
    }     
    else if(strcmp(command, "heal")==0) 
    {
        if(Config.Command_Heal > thisclient->Session->accesslevel)
	                    return true;
        Log( MSG_GMACTION, " %s : /heal", thisclient->CharInfo->charname );
        return pakGMHeal( thisclient );
    }     
    else if(strcmp(command, "serverinfo")==0) 
    {
         if(Config.Command_ServerInfo > thisclient->Session->accesslevel)
	                    return true;
        Log( MSG_GMACTION, " %s : /serverinfo" , thisclient->CharInfo->charname);        
		return pakGMServerInfo( thisclient );
    }       
    else if(strcmp(command, "targetinfo")==0) 
    {
         if(Config.Command_TargetInfo > thisclient->Session->accesslevel)
	                    return true;
        return GMShowTargetInfo( thisclient );
    }
    else if (strcmp(command, "hide")==0)
    {
         if(Config.Command_Hide > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL) 
            return true; 
        int mode= atoi( tmp );
        return pakGMHide( thisclient, mode );
    }    
    else if (strcmp(command, "class")==0)
    {
         if(Config.Command_Class > thisclient->Session->accesslevel)
	                    return true;
        if ((tmp = strtok(NULL, " "))==NULL) 
            return true; 
        char* classid=(char*)tmp;
        Log( MSG_GMACTION, " %s : /class %s" , thisclient->CharInfo->charname, classid);
        return pakGMClass( thisclient, classid );
    }        
    else if(strcmp(command, "convert")==0)    
    {
        if(Config.Command_Convert > thisclient->Session->accesslevel) 
	       return true;
	    if(thisclient->Battle->target==0) return true;
        if ((tmp = strtok(NULL, " " ))==NULL)
            return true;
        UINT newmon = atoi(tmp);
        if(newmon==0) return true;
        CMonster* thismon = GetMonsterByID( thisclient->Battle->target, thisclient->Position->Map );	
        if(thismon==NULL)
        {
            CNPC* thisnpc = GetNPCByID( thisclient->Battle->target, thisclient->Position->Map );
            if(thisnpc==NULL) return true;
            MapList.Index[thisclient->Position->Map]->ConverToMonster( thisnpc, newmon );    
        }
        else
        {
            MapList.Index[thisclient->Position->Map]->ConverToMonster( thismon, newmon );    
        }
        return true;
    }
    else if (strcmp(command, "b")==0) 
    {
        if(Config.Command_Broadcast > thisclient->Session->accesslevel) 
            return true;
        time_t seconds;
        seconds = time (NULL);
        if((thisclient->CharInfo->LastGlobal+Config.Command_GlobalTime) <= seconds) 
        {
            thisclient->CharInfo->LastGlobal = time (NULL);
            char outputmsg[200];
            sprintf( outputmsg, "%s %s", Config.Command_GlobalPrefix, &P->Buffer[3] );
            Log( MSG_INFO, "%s> %s %s", thisclient->CharInfo->charname, Config.Command_GlobalPrefix, &P->Buffer[3]);
            SendGlobalMSG(thisclient, outputmsg);
        } 
        else 
        {
            long int remaining = (Config.Command_GlobalTime-(seconds-thisclient->CharInfo->LastGlobal));
            char buffer2[200];
            sprintf ( buffer2, "Please wait %i seconds before sending another global message.", remaining );
            SendPM(thisclient, buffer2);
        }
        return true;
    } 
    else if(strcmp(command, "who2")==0) 
    {
        if(Config.Command_Who2 > thisclient->Session->accesslevel)
            return true;       
        SendPM(thisclient, "The following players are currently connected;");
        int count=1;
        int hiddenam=0;
        char line0[200];
        while(count <= (ClientList.size()-1)) 
        {
            CPlayer* whoclient = (CPlayer*)ClientList.at(count)->player;
            if(whoclient->Session->accesslevel > 100) 
            {
                sprintf(line0, "%s - GM[%i]", whoclient->CharInfo->charname, whoclient->Session->accesslevel);                                      
            } 
            else 
            {
                sprintf(line0, "%s", whoclient->CharInfo->charname);
            }
            if(whoclient->isInvisibleMode != true) 
            {
                SendPM(thisclient, line0 );
            } 
            else 
            {
                hiddenam++;
            }
            count++;
        }
        sprintf(line0, "There are currently %i players connected!", ((ClientList.size()-1)-hiddenam));
        Log( MSG_GMACTION, " %s : /who2" , thisclient->CharInfo->charname);
        SendPM(thisclient, line0 );
        return true;
    }
    else if(strcmp(command, "rate")==0) //incomplete
    {
        if(Config.Command_Rate > thisclient->Session->accesslevel)  
           return true;  
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        char* type = tmp;            
        if(strcmp(type,"drop")==0)
        {
            if ((tmp = strtok(NULL, " "))==NULL) return true;            
            Config.DROP_RATE = atoi(tmp);
        }
        else
        if(strcmp(type,"exp")==0)
        {
            if ((tmp = strtok(NULL, " "))==NULL) return true;            
            Config.EXP_RATE = atoi(tmp);
        }        
    }
    else if(strcmp(command, "moveto")==0)    
    {
        if(Config.Command_Moveto > thisclient->Session->accesslevel)  
            return true;  
        fPoint position;
        if ((tmp = strtok(NULL, " "))==NULL) return true;  
        position.x = atof(tmp);
        if ((tmp = strtok(NULL, " "))==NULL) return true;          
        position.y = atof(tmp);        
        if ((tmp = strtok(NULL, " "))==NULL) return true;          
        position.z = atof(tmp);        
        pakGMMoveTo( thisclient, position );    
    }
    else if(strcmp(command, "settime")==0)
    {
        if(Config.Command_Settime > thisclient->Session->accesslevel)  
            return true;  
        if ((tmp = strtok(NULL, " "))==NULL) return true;
        unsigned int time = atoi(tmp);
        if(MapList.Index[thisclient->Position->Map]!=NULL)
        {
            switch(time)
            {
                case MORNING:
                    MapList.Index[thisclient->Position->Map]->MapTime = MapList.Index[thisclient->Position->Map]->morningtime;
                    SendSysMsg( thisclient, "Time changed!, rejoin to see change [MORNING]" );
                break;
                case DAY:
                    MapList.Index[thisclient->Position->Map]->MapTime = MapList.Index[thisclient->Position->Map]->daytime;
                    SendSysMsg( thisclient, "Time changed!, rejoin to see change [DAY]" );                    
                break;
                case EVENING:
                    MapList.Index[thisclient->Position->Map]->MapTime = MapList.Index[thisclient->Position->Map]->eveningtime;
                    SendSysMsg( thisclient, "Time changed!, rejoin to see change [EVENING]" );                    
                break;
                case NIGHT:
                    MapList.Index[thisclient->Position->Map]->MapTime = MapList.Index[thisclient->Position->Map]->nighttime;
                    SendSysMsg( thisclient, "Time changed!, rejoin to see change [NIGHT]" );                    
                break;
                default:
                    SendSysMsg( thisclient, "Time: 0=MORNING | 1=DAY | 2=EVENING | 3=NIGHT" );
            }
        }             
    }
    else if(strcmp(command, "transx")==0)
    {
        if(Config.Command_Transx > thisclient->Session->accesslevel)  
            return true; 
        thisclient->CharInfo->Sex = thisclient->CharInfo->Sex==0?1:0;
        BEGINPACKET( pak, 0x720 );
        ADDWORD( pak, 2 );
        ADDWORD( pak, thisclient->CharInfo->Sex );
        ADDWORD( pak, 0 );
        thisclient->client->SendPacket( &pak );
        RESETPACKET( pak, 0x0730 );
        ADDWORD( pak, 5 );
        ADDWORD( pak, 0xa24d );
        ADDWORD( pak, 0x40b3 );
        thisclient->client->SendPacket( &pak );        
    }
    else if (strcmp(command, "partylvl")==0)
    {
        if(Config.Command_Partylvl > thisclient->Session->accesslevel)
	       return true;
        if ((tmp = strtok(NULL, " "))==NULL) return true; int level= atoi( tmp );
        return pakGMPartylvl( thisclient, level );
    }
    else if(strcmp(command, "pdmg")==0)
    {
        if(Config.Command_Pdmg > thisclient->Session->accesslevel)
	                    return true;
	    if((tmp = strtok(NULL, " "))==NULL) return true; unsigned rate=atoi(tmp);
	    Log( MSG_GMACTION, " Rate for Player Dmg is now set at %i by %s" , rate, thisclient->CharInfo->charname);
	    return pakGMChangePlayerDmg(thisclient, rate);
	}
	else if(strcmp(command, "mdmg")==0)
    {
        if(Config.Command_Mdmg > thisclient->Session->accesslevel)
	                    return true;
	    if((tmp = strtok(NULL, " "))==NULL) return true; unsigned rate=atoi(tmp);
	    Log( MSG_GMACTION, " Rate for Monster Dmg is now set at %i by %s" , rate, thisclient->CharInfo->charname);
	    return pakGMChangeMonsterDmg(thisclient, rate);
	}
    else if(strcmp(command, "rules")==0)  // Rules Command by Matt
    {
        if(Config.Command_Rules > thisclient->Session->accesslevel)
	                    return true;
            SendPM(thisclient, "Please follow the following rules to ensure you have a fun time on this server!");
            SendPM(thisclient, "Rule 1: No hacking");
            SendPM(thisclient, "Rule 2: Do not abuse any bugs in the game");
            SendPM(thisclient, "Rule 3: Be respectful to all players and GMs");
            SendPM(thisclient, "Rule 4: Do not beg for items");
            SendPM(thisclient, "Rule 5: Do not harass any players or GM's");
            SendPM(thisclient, "Rule 6: Do not abuse any bugs in the game");
    }
    else if(strcmp(command, "cfmode")==0)
    {
        if(Config.Command_Cfmode > thisclient->Session->accesslevel)
	                    return true;
	    if((tmp = strtok(NULL, " "))==NULL) return true; unsigned mode=atoi(tmp);
	    Log( MSG_GMACTION, " Clan field mode changed to %i by %s" , mode, thisclient->CharInfo->charname);
	    return pakGMChangeCfmode(thisclient, mode);
	}
	else if(strcmp(command, "whoattacksme")==0)
	{
	    return pakGMWhoAttacksMe(thisclient);
    }
    else if(strcmp(command, "atkModif")==0)
    {
        if(300 > thisclient->Session->accesslevel)
	                    return true;
	    if((tmp = strtok(NULL, " "))==NULL) return true; unsigned attkSpeedModif=atoi(tmp);
	    Log( MSG_GMACTION, " AttkSpeedModif changed to %i by %s" , attkSpeedModif, thisclient->CharInfo->charname);
	    return pakGMChangeAtkSpeedModif(thisclient, attkSpeedModif);
	}
	else if(strcmp(command, "hitModif")==0)
    {
        if(300 > thisclient->Session->accesslevel)
	                    return true;
	    if((tmp = strtok(NULL, " "))==NULL) return true; unsigned hitDelayModif=atoi(tmp);
	    Log( MSG_GMACTION, " HitDelayModif changed to %i by %s" , hitDelayModif, thisclient->CharInfo->charname);
	    return pakGMChangeHitDelayModif(thisclient, hitDelayModif);
	}
	else if(strcmp(command, "speedModif")==0)
    {
        if(300 > thisclient->Session->accesslevel)
	                    return true;
	    if((tmp = strtok(NULL, " "))==NULL) return true; unsigned mSpeedModif=atoi(tmp);
	    Log( MSG_GMACTION, " mSpeedModif changed to %i by %s" , mSpeedModif, thisclient->CharInfo->charname);
	    return pakGMChangeMSpeedModif(thisclient, mSpeedModif);
	}
	// buff - debuff by Drakia
    else if (strcmp(command, "buff")==0) {
        if (thisclient->Session->accesslevel < 300)
            return true;
        UINT strength;
        if ((tmp = strtok(NULL, " ")) == NULL) strength = 0; else strength = atoi(tmp);
        if (strength < 15) strength = 15;
        Log( MSG_GMACTION, "buff : character [ %s ] Strength [ %d ]", thisclient->CharInfo->charname, strength);
        pakGMBuff(thisclient, strength);
        return true;
     }
     else if ( strcmp(command, "debuff")==0) {
        if (thisclient->Session->accesslevel < 300)
            return true;
        Log(MSG_GMACTION, "debuff : character [ %s ]", thisclient->CharInfo->charname);
        pakGMDebuff(thisclient);
        return true;
     }
    // MaxStats - by rl2171
    else if(strcmp(command, "maxstats")==0) 
    {
        if(Config.Command_MaxStats > thisclient->Session->accesslevel)
            return true;
        Log( MSG_GMACTION, " %s : /MaxStats", thisclient->CharInfo->charname );
        return pakGMMaxStats( thisclient );
    }     

    // mystat - by PurpleYouko
    else if(strcmp(command, "mystat")==0)
    {
         if ((tmp = strtok(NULL, " "))==NULL)
        {
            SendPM(thisclient, "Please input after the mystat command, below is a list of commands");
            SendPM(thisclient, "ap = Attack Power");
            SendPM(thisclient, "acc = Accuracy");
            SendPM(thisclient, "dodge = Dodge");
            SendPM(thisclient, "def = Defense");
            SendPM(thisclient, "crit = Critical");
            SendPM(thisclient, "mspd = Movement Speed");
            SendPM(thisclient, "Example; /mystat ap");
         return true;
        }
         
         char buffer[200];
         if(strcmp(tmp, "ap")==0)
         {
             sprintf ( buffer, "My Attack Power is %i", thisclient->Stats->Attack_Power );
             SendPM(thisclient, buffer);
         }
         else if(strcmp(tmp, "acc")==0)
         {
             sprintf ( buffer, "My Accuracy is %i", thisclient->Stats->Accury );
             SendPM(thisclient, buffer); 
         }
         else if(strcmp(tmp, "dodge")==0) 
         {
             sprintf ( buffer, "My dodge is %i", thisclient->Stats->Dodge);
             SendPM(thisclient, buffer);  
         }
         else if(strcmp(tmp, "def")==0) 
         {
             sprintf ( buffer, "My defense is %i", thisclient->Stats->Defense);
             SendPM(thisclient, buffer);  
         }
         else if(strcmp(tmp, "crit")==0) 
         {
             sprintf ( buffer, "My critical is %i", thisclient->Stats->Critical);
             SendPM(thisclient, buffer);  
         }
         else if(strcmp(tmp, "mspd")==0) 
         {
             sprintf ( buffer, "My move speed is %i", thisclient->Stats->Move_Speed);
             SendPM(thisclient, buffer);  
         }
    }
   
    else
    {
		Log( MSG_WARNING, "Invalid GM Command '%s' by '%s'", command, thisclient->CharInfo->charname);
		//Wrong Command Alert {By CrAshInSiDe}
        SendPM(thisclient, "Invalid Command");
	}    
	return true;	
}

// GM: Announcment
bool CWorldServer::pakGMAnn( CPlayer* thisclient, CPacket *P )
{
	BEGINPACKET( pak, 0x702 );
	ADDSTRING( pak, thisclient->CharInfo->charname );
	ADDSTRING( pak, "> " );
	ADDSTRING( pak, (&P->Buffer[5]));
	ADDBYTE( pak, 0x00);
	SendToAll( &pak );

	return true;
}

// GM: Spawn x mobs
bool CWorldServer::pakGMMon( CPlayer* thisclient, int montype, int moncount )
{  
	for (int i=0; i<moncount; i++) 
    {
        fPoint position = RandInCircle( thisclient->Position->current, 10 );
        CMap* map = MapList.Index[thisclient->Position->Map];
        map->AddMonster( montype, position, 0, NULL, NULL, 0 , true );
	}	
	return true;
}

// GM: Teleport somewhere
bool CWorldServer::pakGMTele( CPlayer* thisclient, int map, float x, float y )
{
    fPoint coord;
    coord.x = x;
    coord.y = y;
    MapList.Index[map]->TeleportPlayer( thisclient, coord, false );
	return true;
}

// [by Paul_T] [Thanks to AridTag for the packet :D]
bool CWorldServer::pakGMMute( CPlayer* thisclient, char* name, int time)
{
    CPlayer* otherclient = GetClientByCharName ( name ); 
    BEGINPACKET( pak, 0x70d );
    ADDBYTE    ( pak, 0 );
    ADDWORD    ( pak, time );
    ADDSTRING  ( pak, thisclient->CharInfo->charname );
    ADDBYTE    ( pak, 0 );
    otherclient->client->SendPacket( &pak );
    return true;     
}

// GM: Item   - Modified by Hiei (added refine/socket/stats)
bool CWorldServer::pakGMItem( CPlayer* thisclient, UINT itemid, UINT itemtype, UINT itemamount, UINT itemrefine, UINT itemls, UINT itemstats, UINT itemsocket )
{
    CItem item;
    item.count            = itemamount;
    item.durability        = 40;
    item.itemnum        = itemid;
    item.itemtype        = itemtype;
    item.lifespan        = 100;  // changed from itemls to 100, no need for lifespan on a item that is spawned - rl2171
    item.refine            = itemrefine;
    item.stats            = itemstats;
    item.socketed        = itemsocket;
    item.appraised        = 1;
    item.gem = 0;          
    unsigned newslot = thisclient->GetNewItemSlot( item );
    if (newslot != 0xffff) 
    {
        thisclient->items[newslot] = item;
        thisclient->UpdateInventory( newslot );        
        char buffer[200];
        sprintf( buffer, "Item added! (ID: %i) (Type: %i) (Refine: %i) (Socket: %i)", item.itemnum, item.itemtype, item.refine, item.socketed );
        BEGINPACKET ( pak, 0x702 );
        ADDSTRING( pak, buffer );
        ADDBYTE( pak, 0 );
        thisclient->client->SendPacket( &pak );

    }else{
        BEGINPACKET( pak, 0x7a7 );
        ADDWORD( pak, thisclient->clientid );        
        ADDBYTE( pak, 5 );
        thisclient->client->SendPacket( &pak );

        RESETPACKET ( pak, 0x702 );
        ADDSTRING( pak, "No free slot !" );
        ADDBYTE( pak, 0 );
        thisclient->client->SendPacket( &pak );
    }
    return true;
}

// LMA: We give an item from a Quest...
bool CWorldServer::pakGMItemQuest( CPlayer* thisclient, UINT itemid, UINT itemtype, UINT itemamount, UINT itemrefine, UINT itemls, UINT itemstats, UINT itemsocket, char buffer2[200] )
{
    CItem item;
    item.count            = itemamount;
    item.durability        = 40;
    item.itemnum        = itemid;
    item.itemtype        = itemtype;
    item.lifespan        = 100;  // changed from itemls to 100, no need for lifespan on a item that is spawned - rl2171
    item.refine            = itemrefine;
    item.stats            = itemstats;
    item.socketed        = itemsocket;
    item.appraised        = 1;
    item.gem = 0;          
    unsigned newslot = thisclient->GetNewItemSlot( item );
    if (newslot != 0xffff) 
    {
        thisclient->items[newslot] = item;
        thisclient->UpdateInventory( newslot );        
        char buffer[200];
        sprintf( buffer, "%s", buffer2);
        BEGINPACKET ( pak, 0x702 );
        ADDSTRING( pak, buffer );
        ADDBYTE( pak, 0 );
        thisclient->client->SendPacket( &pak );

    }else{
        BEGINPACKET( pak, 0x7a7 );
        ADDWORD( pak, thisclient->clientid );        
        ADDBYTE( pak, 5 );
        thisclient->client->SendPacket( &pak );

        RESETPACKET ( pak, 0x702 );
        ADDSTRING( pak, "No slot available in inventory!" );
        ADDBYTE( pak, 0 );
        thisclient->client->SendPacket( &pak );
    }
    return true;
}

// GM: Kick
bool CWorldServer::pakGMKick( CPlayer* thisclient, char* name )
{
    CPlayer* otherclient = GetClientByCharName( name );
    if(otherclient==NULL)
        return true;
    BEGINPACKET( pak, 0x702 );
    ADDSTRING( pak, "You were disconnected from the server !" );
    ADDBYTE( pak, 0 );
    otherclient->client->SendPacket( &pak );

    RESETPACKET( pak, 0x707 );
    ADDWORD( pak, 0 );
    otherclient->client->SendPacket( &pak );

    otherclient->client->isActive = false;

    return true;
}

// GM: Ban
bool CWorldServer::pakGMBan( CPlayer* thisclient, char* name )
{
    CPlayer* otherclient = GetClientByCharName( name );
    if(otherclient==NULL)
        return true;
    otherclient->Session->accesslevel = -1;
    DB->QExecute( "UPDATE accounts SET accesslevel='0' WHERE id=%i", otherclient->Session->userid);
    BEGINPACKET( pak, 0x702 );
    ADDSTRING( pak, "You were banned from the server !" );
    ADDBYTE( pak, 0 );
    otherclient->client->SendPacket( &pak );

    RESETPACKET( pak, 0x707 );
    ADDWORD( pak, 0 );
    otherclient->client->SendPacket( &pak );

    otherclient->client->isActive = false;

    return true;
}

// Reborn command credits Core
bool CWorldServer::pakGMReborn(CPlayer* thisclient)
{  
     if(thisclient->Stats->Level < 250) //Level 250 can be changed to any level you want
     {
        GServer->SendPM(thisclient, "You have to be lvl 250 to reborn !");
     }  
     
     else
     { 
        for(int i=0;i<MAX_SKILL;i++)  //For keeping the skills, remove this line
        {  //For keeping the skills, remove this line
            thisclient->cskills[i].id = 0;  //For keeping the skills, remove this line
            thisclient->cskills[i].level = 1;  //For keeping the skills, remove this line
        }  //For keeping the skills, remove this line
    
        for(int i=0;i<MAX_QUICKBAR;i++)
            thisclient->quickbar[i] = 0;
     
         thisclient->p_skills = 0;  //For keeping the skills, remove
         thisclient->CharInfo->SkillPoints = 0; 
         thisclient->CharInfo->StatPoints = 0; 
         thisclient->CharInfo->Job = 0; 
     
         thisclient->Stats->Level = 1;
         thisclient->CharInfo->Exp = 0;
     
         thisclient->ActiveQuest = 0;
         thisclient->MyQuest.clear();
         
/*Update Reborn Command {By CrAshInSiDe*/
        int x = 5098;
        int y = 5322;
        int map = 22;
      if( (x != 0) && (y != 0) && (map != 0) )
        {
            fPoint coord;
            coord.x = x;
            coord.y = y;
            MapList.Index[map]->TeleportPlayer( thisclient, coord, false );
            }
         

 // Uncomment below if you want to use the Nobles part
 /*        
         char newcharname[65];
         strcpy (newcharname,"[Nobles]");
         strcat (newcharname, thisclient->CharInfo->charname);
         GServer->DB->QExecute(" UPDATE characters SET char_name = '%s' WHERE id = '%i' ",newcharname, thisclient->CharInfo->charid);
*/
     
         BEGINPACKET( pak, 0x702 );
         ADDSTRING( pak, "You were disconnected from the server !" );
         ADDBYTE( pak, 0 );
         thisclient->client->SendPacket( &pak );
 
         RESETPACKET( pak, 0x707 );
         ADDWORD( pak, 0 );
         thisclient->client->SendPacket( &pak );
 
         thisclient->client->isActive = false;
      }
 
     return true;
}

//Event function credits Welson
bool CWorldServer::pakGMEventType(CPlayer* thisclient, int npctype, int dialog, int type)
{
    CNPC* thisnpc = GetNPCByType(npctype);
    if(thisnpc == NULL)
    {
        delete thisnpc;
        return true;
    }
    if(type<0) type = 0;
    char buffer[200];
    sprintf( buffer, "You activated Event %i for NPC: %i, Dialog: %i", type, npctype, dialog);
    thisnpc->dialog = dialog;
    thisnpc->event = type;

    BEGINPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[Server]" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
	ADDBYTE    ( pak, 0 );
    thisclient->client->SendPacket(&pak);

    RESETPACKET( pak, 0x790 );
    ADDWORD    ( pak, thisnpc->clientid );
    ADDBYTE    ( pak, thisnpc->event );
	ADDBYTE    ( pak, 0 );
    thisclient->client->SendPacket(&pak);
    
    
	return true;
}

// GM: Add/Remove/Drop/Set zuly
bool CWorldServer::pakGMZuly( CPlayer* thisclient, int mode, int amount, char* charname )
{
	CPlayer* otherclient = GetClientByCharName(charname);
	if(otherclient==NULL)
	   return true;
	if (mode == 1)
	{
		//ADD
		otherclient->CharInfo->Zulies += amount;
		BEGINPACKET( pak, 0x71d );
		ADDQWORD( pak, otherclient->CharInfo->Zulies );
		otherclient->client->SendPacket( &pak );
	}
	if (mode == 2)
	{
		//REMOVE
		otherclient->CharInfo->Zulies -= amount;
		BEGINPACKET( pak, 0x71d );
		ADDQWORD( pak, otherclient->CharInfo->Zulies );
		otherclient->client->SendPacket( &pak );
	}
	if (mode == 3)
	{
		//DROP
		CDrop* thisdrop = new CDrop;
		assert(thisdrop);
		thisdrop->clientid = GetNewClientID();
		thisdrop->type = 1; // ZULY
		thisdrop->pos.x = otherclient->Position->current.x;
		thisdrop->pos.y = otherclient->Position->current.y;
		thisdrop->posMap = otherclient->Position->Map;
		thisdrop->droptime = time(NULL);
		thisdrop->amount = amount;
		thisdrop->owner = 0;
		CMap* map = MapList.Index[thisdrop->posMap];
		map->AddDrop( thisdrop );
	}
	if (mode == 4)
	{
		//SET
		otherclient->CharInfo->Zulies = amount;
		BEGINPACKET( pak, 0x71d );
		ADDQWORD( pak, otherclient->CharInfo->Zulies );
		otherclient->client->SendPacket( &pak );
	}
	return true;
}

// Change player Level 
bool CWorldServer::pakGMLevel( CPlayer* thisclient, int level, char* name )
{
    CPlayer* otherclient = GetClientByCharName( name );
    if (otherclient == NULL)
        return true;
    if (((int)otherclient->Stats->Level + level) > 0)
        otherclient->Stats->Level += level;
    else
        otherclient->Stats->Level = 1;
    if (otherclient->Stats->Level > 250)  
        otherclient->Stats->Level = 250;  
    otherclient->CharInfo->Exp = 0;
    if (level < 0) {
        otherclient->CharInfo->StatPoints = 0;
        otherclient->CharInfo->SkillPoints = 0;
        if(otherclient->Stats->Level > 1)
        {
            for(int s = 2; s <= otherclient->Stats->Level; s++)
            {
                otherclient->CharInfo->StatPoints += 10;
                otherclient->CharInfo->StatPoints += s / 2;
                if (s >= 10)
                    otherclient->CharInfo->SkillPoints++;
            }
        }
        pakGMStat(otherclient, "str", 15);
        pakGMStat(otherclient, "dex", 15);
        pakGMStat(otherclient, "con", 15);
        pakGMStat(otherclient, "int", 15);
        pakGMStat(otherclient, "cha", 10);
        pakGMStat(otherclient, "sen", 10);
    } else {
        for(int s = (otherclient->Stats->Level - level + 1); s <= otherclient->Stats->Level; s++)
        {
            otherclient->CharInfo->StatPoints += 10;
            otherclient->CharInfo->StatPoints += s / 2;
            if (s >= 10)
                otherclient->CharInfo->SkillPoints++;
        }
    }
    BEGINPACKET( pak, 0x79e );
    ADDWORD( pak, otherclient->clientid );
    ADDWORD( pak, otherclient->Stats->Level );
    ADDDWORD( pak, otherclient->CharInfo->Exp );
    ADDWORD( pak, otherclient->CharInfo->StatPoints );
    ADDWORD( pak, otherclient->CharInfo->SkillPoints );
    otherclient->client->SendPacket( &pak );

    RESETPACKET( pak, 0x79e );
    ADDWORD( pak, otherclient->clientid );
    SendToVisible( &pak, otherclient, false );

    otherclient->SetStats( );
    otherclient->Stats->HP = otherclient->Stats->MaxHP;
    otherclient->Stats->MP = otherclient->Stats->MaxMP;
    return true;
}


// Reload Mob Spawn
bool CWorldServer::ReloadMobSpawn( CPlayer* thisclient, int id )
{
	CSpawnArea* thisspawn = new (nothrow) CSpawnArea;
	if(thisspawn==NULL)
	{
        Log(MSG_WARNING, "Error allocing memory" );
        return true;
    }
    thisspawn->thisnpc = GetNPCDataByID( thisclient->GMRespawnPoints.mobID );
    if(thisspawn->thisnpc==NULL)
    {
        SendSysMsg( thisclient, "Invalid Respawn" );
        return true;	
    }
	thisspawn->id = id;
	thisspawn->map = thisclient->GMRespawnPoints.map;
	thisspawn->montype = thisclient->GMRespawnPoints.mobID;
	thisspawn->min = thisclient->GMRespawnPoints.min;
	thisspawn->max = thisclient->GMRespawnPoints.max;
	thisspawn->respawntime = thisclient->GMRespawnPoints.respawntime;		
	thisspawn->pcount = thisclient->GMRespawnPoints.n;
	thisspawn->points = thisclient->GMRespawnPoints.points;  
    CMap* map = MapList.Index[thisspawn->map];
	for(int j=0; j<thisspawn->max; j++) 
    {        
        fPoint position = RandInPoly( thisspawn->points, thisspawn->pcount );             
        map->AddMonster( thisspawn->montype, position, 0, thisspawn->mobdrop, thisspawn->mapdrop, thisspawn->id, true );                
	}
	thisspawn->lastRespawnTime=clock();	
    return true;    
}

// Teleport To player to other place
bool CWorldServer::pakGMTeleOtherPlayer( CPlayer *thisclient, char* name, int map, float x, float y )
{
	CPlayer* otherclient = GetClientByCharName( name );
	if (otherclient != NULL)
	{
        fPoint coord;
        coord.x = x;
        coord.y = y;
        MapList.Index[map]->TeleportPlayer( otherclient, coord, false );
	}
    else
	{
        SendSysMsg( thisclient, "User does not exist or is not online!" );
	}
	return true;
}

// Teleport Me to player
bool CWorldServer::pakGMTeleToPlayer( CPlayer* thisclient, char* name )
{
	CPlayer* otherclient = GetClientByCharName( name );
	if (otherclient != NULL)
	{
        MapList.Index[otherclient->Position->Map]->TeleportPlayer( thisclient, otherclient->Position->current, false );
	}
    else 
	{
        SendSysMsg( thisclient, "User does not exist or is not online!" );
	}
	return true;
}

// Teleport Player Here
bool CWorldServer::pakGMTelePlayerHere( CPlayer* thisclient, char* name )
{
	CPlayer* otherclient = GetClientByCharName( name );
	if (otherclient != NULL)
	{        
        CMap* map = MapList.Index[thisclient->Position->Map];
        map->TeleportPlayer( otherclient, thisclient->Position->current, false );
	}
    else 
	{
        SendSysMsg( thisclient, "User does not exist or is not online!");
	}
	return true;
}

// Show Player info
bool CWorldServer::pakGMInfo(CPlayer* thisclient, char* name)
{
	CPlayer* otherclient = GetClientByCharName(name);
    if(otherclient==NULL)
        return true;
	char  buffer[200];
	char* jobname;

	if(otherclient != 0) {
		if((otherclient->CharInfo->Job - 0) == 0) { jobname = "Visitor"; }
		else if((otherclient->CharInfo->Job - 0) == 111) { jobname = "Soldier"; }
		else if((otherclient->CharInfo->Job - 0) == 121) { jobname = "Knight"; }
		else if((otherclient->CharInfo->Job - 0) == 122) { jobname = "Champion"; }
		else if((otherclient->CharInfo->Job - 0) == 211) { jobname = "Muse"; }
		else if((otherclient->CharInfo->Job - 0) == 221) { jobname = "Mage"; }
		else if((otherclient->CharInfo->Job - 0) == 222) { jobname = "Cleric"; }
		else if((otherclient->CharInfo->Job - 0) == 311) { jobname = "Hawker"; }
		else if((otherclient->CharInfo->Job - 0) == 321) { jobname = "Raider"; }
		else if((otherclient->CharInfo->Job - 0) == 322) { jobname = "Scout"; }
		else if((otherclient->CharInfo->Job - 0) == 411) { jobname = "Dealer"; }
		else if((otherclient->CharInfo->Job - 0) == 421) { jobname = "Bourgeois"; }
		else if((otherclient->CharInfo->Job - 0) == 422) { jobname = "Artisan"; }
		else { jobname = "Unknown"; }

		BEGINPACKET(pak, 0x702);

		sprintf(buffer, "Info about <%s>" , otherclient->CharInfo->charname);
		ADDSTRING(pak, buffer);
		ADDBYTE(pak, 0);
		thisclient->client->SendPacket(&pak);

		RESETPACKET(pak, 0x702);
		sprintf(buffer, "Account: %s | ID: %i" , otherclient->Session->username, otherclient->Session->userid);
		ADDSTRING(pak, buffer);
		ADDBYTE(pak, 0);
		thisclient->client->SendPacket(&pak);

		RESETPACKET(pak, 0x702);
		sprintf(buffer, "Level: %i | Job: %s" , (otherclient->Stats->Level-0) , jobname);
		ADDSTRING(pak, buffer);
		ADDBYTE(pak, 0);
		thisclient->client->SendPacket(&pak);

		RESETPACKET(pak, 0x702);
		sprintf(buffer,"Map: %i , X: %i , Y: %i", (otherclient->Position->Map-0), (int)otherclient->Position->current.x, (int)otherclient->Position->current.y);
		ADDSTRING(pak, buffer);
		ADDBYTE(pak, 0);
		thisclient->client->SendPacket(&pak);
	}
	else {
		BEGINPACKET(pak, 0x702);
		ADDSTRING(pak, "User does not exist or is not online!");
		ADDBYTE(pak, 0)
		thisclient->client->SendPacket(&pak);
	}
	return true;
}

// Add Fairy
bool CWorldServer::pakGMFairyto(CPlayer* thisclient, char* name, int mode)
{
	if (GServer->Config.FairyMode== 0){
        BEGINPACKET(pak, 0x702);
		ADDSTRING(pak, "Fairy feature is de-activated.");
		ADDBYTE(pak, 0);
		thisclient->client->SendPacket(&pak);                            
        return true;
    }
	CPlayer* otherclient = GetClientByCharName (name);
	if(otherclient==NULL){ 
        BEGINPACKET(pak, 0x702);
		ADDSTRING(pak, "User does not exist or is not online.");
		ADDBYTE(pak, 0);
		thisclient->client->SendPacket(&pak);
        return true;
    }
    if(mode == 0 && otherclient->Fairy == false){
        BEGINPACKET(pak, 0x702);
		ADDSTRING(pak, "User already not fairied.");
		ADDBYTE(pak, 0);
		thisclient->client->SendPacket(&pak);
        return true;
    }
    if(mode == 1 && otherclient->Fairy == true){ 
        BEGINPACKET(pak, 0x702);
		ADDSTRING(pak, "User already fairied.");
		ADDBYTE(pak, 0);
		thisclient->client->SendPacket(&pak);
        return true;
    }
    if(!otherclient->Fairy && mode == 1){
          int FairyIndex=100;                  
          for (int i=0; i<GServer->Config.FairyMax; i++){
               if (GServer->FairyList.at(i)->assigned == false){
                   FairyIndex=i;
                   i=GServer->Config.FairyMax;
               }
          } 
          if (FairyIndex == 100){
              BEGINPACKET(pak, 0x702);
		      ADDSTRING(pak, "No free Fairy.");
		      ADDBYTE(pak, 0);
		      thisclient->client->SendPacket(&pak);
		      return true;
          }  
          BEGINPACKET(pak, 0x702);
		  ADDSTRING(pak, "User fairied.");
		  ADDBYTE(pak, 0);
		  thisclient->client->SendPacket(&pak);                                           
          int ListIndex;
          for (int i=0; i<ClientList.size(); i++){
              if (GServer->ClientList.at(i)->player == otherclient){ 
                 ListIndex = i; 
                 i = GServer->ClientList.size();
              }  
          }
          otherclient->Fairy = true; 
          otherclient->FairyListIndex = FairyIndex;
          GServer->FairyList.at(FairyIndex)->ListIndex = ListIndex;
          GServer->FairyList.at(FairyIndex)->assigned = true;
          GServer->FairyList.at(FairyIndex)->LastTime = clock();
          GServer->FairyList.at(FairyIndex)->WaitTime = GServer->Config.FairyWait;
          GServer->DoFairyStuff( otherclient, mode );
          otherclient->SetStats();
          Log( MSG_INFO, "HP: %i  MP: %i  ATK: %i   DEF: %i   CRI: %i  MSPD: %i", otherclient->Stats->MaxHP, otherclient->Stats->MaxMP, otherclient->Stats->Attack_Power, otherclient->Stats->Defense, otherclient->Stats->Critical, otherclient->Stats->Move_Speed);
    }
    if(otherclient->Fairy && mode == 0){               
          GServer->DoFairyFree(otherclient->FairyListIndex);
          GServer->FairyList.at(otherclient->FairyListIndex)->WaitTime = GServer->Config.FairyWait;
          otherclient->Fairy = false;
          otherclient->FairyListIndex = 0;
          GServer->DoFairyStuff( otherclient, mode );
          BEGINPACKET(pak, 0x702);
		  ADDSTRING(pak, "User unfairied.");
		  ADDBYTE(pak, 0);
		  otherclient->client->SendPacket(&pak); 
          otherclient->SetStats();
          Log( MSG_INFO, "HP: %i  MP: %i  ATK: %i   DEF: %i   CRI: %i  MSPD: %i", otherclient->Stats->MaxHP, otherclient->Stats->MaxMP, otherclient->Stats->Attack_Power, otherclient->Stats->Defense, otherclient->Stats->Critical, otherclient->Stats->Move_Speed);  
    }      
    otherclient->SetStats();
	return true;
}

// Activate de-activate Fairy mode in game
bool CWorldServer::pakGMManageFairy(CPlayer* thisclient, int mode)
{
    BEGINPACKET (pak, 0x702); 
	if(mode == 0){
        if (GServer->Config.FairyMode != mode){
	        GServer->Config.FairyMode = 0;
            ADDSTRING(pak, "You have de-activated the Fairy mode ingame."); 
        }else{  
            ADDSTRING(pak, "The Fairy mode is already de-activated.");
        } 
    }else{       
      	if (GServer->Config.FairyMode != mode){
	        GServer->Config.FairyMode = 1;
            for (int i=0; i<GServer->Config.FairyMax; i++)
            {
                GServer->DoFairyFree(i);
                GServer->FairyList.at(i)->WaitTime = GServer->Config.FairyWait;    
            }
            ADDSTRING(pak, "You have activated the Fairy mode ingame."); 
        }else{  
            ADDSTRING(pak, "The Fairy mode is already activated.");
        }  
    } 	
    ADDBYTE(pak, 0); 
    thisclient->client->SendPacket(&pak);
     
	return true;
}

// Change FairyWait
bool CWorldServer::pakGMChangeFairyWait(CPlayer* thisclient, int newvalue)
{
    GServer->Config.FairyWait = newvalue;
    for (int i=0; i<GServer->FairyList.size(); i++){
        GServer->FairyList.at(i)->WaitTime = newvalue;
    }
    BEGINPACKET (pak, 0x702); 
    ADDSTRING(pak, "You have changed the time between each Fairies");
    ADDBYTE(pak, 0); 
    thisclient->client->SendPacket(&pak);   
	return true;
}

// Change FairyStay
bool CWorldServer::pakGMChangeFairyStay(CPlayer* thisclient, int newvalue)
{
    GServer->Config.FairyStay = newvalue; 
    BEGINPACKET (pak, 0x702); 
    ADDSTRING(pak, "You have changed the time of Fairies for Buffing ");
    ADDBYTE(pak, 0); 
    thisclient->client->SendPacket(&pak);   
	return true;
}

// Change Fairy Test mode. 
// 0 -> normal random x * Fairywait
// 1 -> Test mode activated: 1 * Fairywait only
bool CWorldServer::pakGMChangeFairyTestMode(CPlayer* thisclient, int mode)
{
    GServer->Config.FairyTestMode = mode; 
    BEGINPACKET (pak, 0x702); 
    ADDSTRING(pak, "You have changed the Fairy test mode.");
    ADDBYTE(pak, 0); 
    thisclient->client->SendPacket(&pak);   
	return true;
}

// Give Zuly
bool CWorldServer::pakGMZulygive(CPlayer* thisclient, char* name, int zuly)
{
    CPlayer* otherclient = GetClientByCharName (name);
    if(otherclient==NULL)
        return true;
	otherclient->CharInfo->Zulies += zuly;
	BEGINPACKET(pak, 0x7a7);
	ADDWORD(pak, otherclient->clientid);
	ADDWORD(pak, 0);
	ADDBYTE(pak, 0);
	ADDDWORD(pak, 0xccccccdf);
	ADDDWORD(pak, zuly);
    ADDDWORD( pak, 0xcccccccc );
    ADDWORD ( pak, 0xcccc );
	otherclient->client->SendPacket(&pak);

	return true;
}

// Spawn a NPC
bool CWorldServer::pakGMNpc(CPlayer* thisclient, int npcid,int dialogid)
{
	CNPC* thisnpc = new CNPC;
	assert(thisnpc);
	thisnpc->clientid = GetNewClientID();
	thisnpc->dir = 0;
	thisnpc->npctype = npcid;
	thisnpc->pos = thisclient->Position->current;
	thisnpc->posMap = thisclient->Position->Map;
	thisnpc->thisnpc = GetNPCDataByID( npcid );
	thisnpc->thisnpc->dialogid = dialogid;
	if( thisnpc->thisnpc==NULL ) return true;
	CMap* map = MapList.Index[thisclient->Position->Map];	
	map->AddNPC( thisnpc );
    char buffer[200];
    sprintf( buffer, "NPC Spawned! (NPC: %i) (Dialog: %i)", npcid, dialogid );
    BEGINPACKET ( pak, 0x702 );
    ADDSTRING( pak, buffer );
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket( &pak );	
	return true;
}

// Give Item to Player
bool CWorldServer::pakGMItemtoplayer(CPlayer* thisclient, char* name , UINT itemid, UINT itemtype, UINT itemamount, UINT itemrefine, UINT itemls, UINT itemstats, UINT itemsocket) 
{ 
    CItem item;
    item.count            = itemamount;
    item.durability        = 40;
    item.itemnum        = itemid;
    item.itemtype        = itemtype;
    item.lifespan        = 100; //itemls Set lifespan to 100
    item.refine            = itemrefine;
    item.stats            = itemstats;
    item.socketed        = itemsocket;
    item.appraised        = 1;
    item.gem = 0;  
    
   CPlayer* otherclient = GetClientByCharName ( name ); 
 
   if(otherclient != NULL) { 
      unsigned newslot = otherclient->GetNewItemSlot( item ); 
      if(newslot != 0xffff) { 
         otherclient->items[newslot] = item; 
         otherclient->UpdateInventory( newslot );
 
         BEGINPACKET (pak, 0x702); 
         ADDSTRING(pak, "You have recieved an item from a GM !"); 
         ADDBYTE(pak, 0); 
         otherclient->client->SendPacket(&pak); 
 
         RESETPACKET (pak, 0x702); 
         ADDSTRING(pak, "Item has been given!"); 
         ADDBYTE(pak, 0); 
         thisclient->client->SendPacket(&pak); 
      } 
      else { 
         BEGINPACKET (pak, 0x702); 
         ADDSTRING(pak, "No free slot !"); 
         ADDBYTE(pak, 0); 
         thisclient->client->SendPacket(&pak); 
      } 
   } 
 
   return true; 
}

// Do Emotion
bool CWorldServer::pakGMDoEmote( CPlayer* thisclient, int emotionid )
{
    ClearBattle( thisclient->Battle );
	BEGINPACKET( pak, 0x781 );
	ADDDWORD( pak, emotionid );
	ADDWORD( pak, thisclient->clientid );
	SendToVisible( &pak, thisclient );
	return true;
}

// Change GM Stats (Coded by Minoc)
bool CWorldServer::pakGMStat( CPlayer* thisclient, char* statname, int statvalue )
{
    int statid;
    if (strcmp( statname, "Str" )==0 || strcmp( statname, "str" )==0)
    {
        thisclient->Attr->Str = statvalue;
        statid = 0;
    }
    else if (strcmp( statname, "Dex" )==0 || strcmp( statname, "dex" )==0)
    {
        thisclient->Attr->Dex = statvalue;
        statid = 1;
    }
    else if (strcmp( statname, "Int" )==0 || strcmp( statname, "int" )==0)
    {
        thisclient->Attr->Int = statvalue;
        statid = 2;
    }
    else if (strcmp( statname, "Con" )==0 || strcmp( statname, "con" )==0)
    {
        thisclient->Attr->Con = statvalue;
        statid = 3;
    }
    else if (strcmp( statname, "Cha" )==0 || strcmp( statname, "cha" )==0)
    {
        thisclient->Attr->Cha = statvalue;
        statid = 4;
    }
    else if (strcmp( statname, "Sen" )==0 || strcmp( statname, "sen" )==0)
    {
        thisclient->Attr->Sen = statvalue;
        statid = 5;
    }
    else
    {
        return true;
    }
    BEGINPACKET( pak, 0x7a9 );
    ADDBYTE( pak, statid );
    ADDWORD( pak, (unsigned short)statvalue );
    thisclient->client->SendPacket( &pak );
    thisclient->SetStats( );
    return true;
}

// GM: Teleport using map id  credits to Blackie
bool CWorldServer::pakGMGotomap( CPlayer* thisclient, int map )
{
    CRespawnPoint* thisrespawn = MapList.Index[map]->GetFirstRespawn( );
    if(thisrespawn==NULL) 
    {
        SendSysMsg( thisclient, "This map have no respawn" );
        return true;    
    }
    MapList.Index[map]->TeleportPlayer( thisclient, thisrespawn->dest, false );
	return true;
}

// Heal a player ( by rl2171 )
bool CWorldServer::pakGMHeal( CPlayer* thisclient )
{
	thisclient->Stats->HP = thisclient->Stats->MaxHP;
	thisclient->Stats->MP = thisclient->Stats->MaxMP;
	BEGINPACKET( pak, 0x7ec );
	ADDWORD( pak, thisclient->Stats->HP );
	ADDWORD( pak, thisclient->Stats->MP );
	thisclient->client->SendPacket( &pak );
	return true;
}

// GM: Server Information ( by rl2171 ) modified by me
bool CWorldServer::pakGMServerInfo( CPlayer* thisclient )
{
    char buffer[200];
    // Players Online
	BEGINPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[SYS]ServerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, "SERVER INFORMATION" );
	ADDBYTE( pak, 0 );                    
	thisclient->client->SendPacket( &pak );    
	sprintf( buffer, "Online Players: %i", (int)ClientList.size()-1 );// -1 (we don't count charserver)
	RESETPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[SYS]ServerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, buffer );
	ADDBYTE( pak, 0 );                    
	thisclient->client->SendPacket( &pak );
    // Exp / Zulies / Drop rates
	sprintf( buffer, "Exp %i | Zulies %i | Drops %i", Config.EXP_RATE, Config.ZULY_RATE, Config.DROP_RATE );
	RESETPACKET( pak, 0x0784 );
	ADDSTRING( pak, "[SYS]ServerInfo" );
	ADDBYTE( pak, 0 );
	ADDSTRING( pak, buffer );
	ADDBYTE( pak, 0 );                    
	thisclient->client->SendPacket( &pak );
	// Send map time
	if(MapList.Index[thisclient->Position->Map]!=NULL)
	{
    	RESETPACKET( pak, 0x0784 );
    	ADDSTRING( pak, "[SYS]ServerInfo" );	
    	ADDBYTE( pak, 0 );
    	switch(MapList.Index[thisclient->Position->Map]->CurrentTime)
    	{
            case MORNING:
                sprintf( buffer, "The Time is: Morning[%i]", MapList.Index[thisclient->Position->Map]->MapTime%MapList.Index[thisclient->Position->Map]->dayperiod );
                ADDSTRING( pak, buffer );
            break;            
            case DAY:
                sprintf( buffer, "The Time is: Day[%i]", MapList.Index[thisclient->Position->Map]->MapTime%MapList.Index[thisclient->Position->Map]->dayperiod );
                ADDSTRING( pak, buffer );
            break;
            case EVENING:
                sprintf( buffer, "The Time is: Evening[%i]", MapList.Index[thisclient->Position->Map]->MapTime%MapList.Index[thisclient->Position->Map]->dayperiod );
                ADDSTRING( pak, buffer );                
            break;
            case NIGHT:
                sprintf( buffer, "The Time is: Night[%i]", MapList.Index[thisclient->Position->Map]->MapTime%MapList.Index[thisclient->Position->Map]->dayperiod );
                ADDSTRING( pak, buffer );                
            break;
            default:
                sprintf( buffer, "Invalid Time is the End of world [%i]", MapList.Index[thisclient->Position->Map]->MapTime );
                ADDSTRING( pak, buffer );                
        }
    	ADDBYTE( pak, 0 );                    
    	thisclient->client->SendPacket( &pak );	
    }
	return true;
}

// Show Target Info
bool CWorldServer::GMShowTargetInfo( CPlayer* thisclient )
{    
    if(thisclient->Battle->target==0) return true;
    char buffer[200];
    CMonster* monster = GetMonsterByID( thisclient->Battle->target, thisclient->Position->Map );
    if(monster==NULL) return true;
    float dist = distance( thisclient->Position->current, monster->Position->current );
    sprintf( buffer, "Target Position: %.4f, %.4f", monster->Position->current.x, monster->Position->current.y );    
    BEGINPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
	ADDBYTE( pak, 0 );     
    thisclient->client->SendPacket( &pak );    
    sprintf( buffer, "Distance: %.0f",	dist );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
	ADDBYTE( pak, 0 );         
    thisclient->client->SendPacket( &pak ); 
    sprintf( buffer, "Target Defense: %i", monster->Stats->Defense );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
	ADDBYTE( pak, 0 );         
    thisclient->client->SendPacket( &pak );      
    sprintf( buffer, "Target Level: %i", monster->thisnpc->level );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
	ADDBYTE( pak, 0 );         
    thisclient->client->SendPacket( &pak );     
    sprintf( buffer, "Target Attack Power: %i", monster->Stats->Attack_Power );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
	ADDBYTE( pak, 0 );         
    thisclient->client->SendPacket( &pak );             
    sprintf( buffer, "Target Attack Speed: %.0f", monster->Stats->Attack_Speed );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
	ADDBYTE( pak, 0 );         
    thisclient->client->SendPacket( &pak );   
    sprintf( buffer, "Target Move Speed: %i", monster->Stats->Move_Speed );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
	ADDBYTE( pak, 0 );         
    thisclient->client->SendPacket( &pak );       
    sprintf( buffer, "Target HP/MAXHP: %i/%i", monster->Stats->HP, monster->Stats->MaxHP );
    RESETPACKET( pak, 0x784 );
    ADDSTRING  ( pak, "[SYS]TargetInfo" );
    ADDBYTE    ( pak, 0 );
    ADDSTRING  ( pak, buffer );
	ADDBYTE( pak, 0 );         
    thisclient->client->SendPacket( &pak );           
    return true;
}

// GM: Make yourself invisible from tomiz
bool CWorldServer::pakGMHide( CPlayer* thisclient, int mode )
{
    BEGINPACKET( pak, 0x702 );
    if ( mode == 1 )
    {
        thisclient -> isInvisibleMode = true;
        ADDSTRING( pak, "You are now invisible !" );
        Log( MSG_GMACTION, " %s : /hide invisible" , thisclient->CharInfo->charname); 
    }
    else
    {
        thisclient -> isInvisibleMode = false;
        ADDSTRING( pak, "You are now visible !" );
        Log( MSG_GMACTION, " %s : /hide visible" , thisclient->CharInfo->charname); 
    }
    ADDBYTE( pak, 0 );
    thisclient->client->SendPacket ( &pak );
    return true;
}
// GM : Change the party lvl
bool CWorldServer::pakGMPartylvl( CPlayer* partyclient, int level )
{
    if (partyclient->Party->party == NULL) return true;    
    if (level < 0 || level >50) return true;
    partyclient->Party->party->PartyLevel = level;
    if( partyclient->Party->party->PartyLevel == 50)
          partyclient->Party->party->Exp = 0;
          //Send Party Level and Party Exp
          BEGINPACKET( pak, 0x7d4 ); //
          ADDBYTE    ( pak, partyclient->Party->party->PartyLevel);
          ADDDWORD   ( pak, partyclient->Party->party->Exp );
          partyclient->Party->party->SendToMembers( &pak );
    return true;
}

// GM: Kill all mobs in a range of x-Fields
bool CWorldServer::pakGMKillInRange( CPlayer* thisclient, int range )
{
    for(UINT j = 0; j < MapList.Index[thisclient->Position->Map]->MonsterList.size(); j++) 
    {
        CMonster* thismon = MapList.Index[thisclient->Position->Map]->MonsterList.at(j);
        if( IsMonInCircle( thisclient, thismon, (float)range ))
        {
            //Kill the mob    
            thismon->Stats->HP = -1;
            BEGINPACKET( pak, 0x799 );
            ADDWORD    ( pak, thismon->clientid );
            ADDWORD    ( pak, thismon->clientid );
            ADDDWORD   ( pak, thismon->thisnpc->hp*thismon->thisnpc->level );
            ADDDWORD   ( pak, 16 );
            SendToVisible( &pak, thismon );
        	CMap* map = MapList.Index[thisclient->Position->Map];
            map->DeleteMonster( thismon );
        }
    }
    return true;
}

// GM: Change Class (from Crash)
// Block Multiclass, except for GMs  (from MonkeyRose)
bool CWorldServer::pakGMClass( CPlayer* thisclient, char* classid )
{
    int classid_new = thisclient->CharInfo->Job;
    // allow GMs to multiclass
    bool GM = ( thisclient->Session->accesslevel == 300 );
    if ( strcmp ( classid , "Visitor" ) == 0 || strcmp ( classid , "visitor" ) == 0)
    {
        if (GM)
           classid_new = 0;
    }
    else if ( strcmp ( classid , "Soldier" ) == 0 || strcmp ( classid , "soldier" ) == 0)
    {
         if ( GM || ((thisclient->Stats->Level >= 10) && (classid_new == 0 ))) // visitor
             classid_new = 111;
    }
    else if ( strcmp ( classid , "Knight" ) == 0 || strcmp ( classid , "knight" ) == 0)
    {
        if ( GM || ((thisclient->Stats->Level >= 100) && (classid_new == 111)))  // solder
             classid_new = 121;
    }
    else if ( strcmp ( classid , "Champion" ) == 0 || strcmp ( classid , "champion" ) == 0)
    {
        if ( GM || ((thisclient->Stats->Level >= 100) && (classid_new == 111)))  // solder
                classid_new = 122;
    }
    else if ( strcmp ( classid , "Muse" ) == 0 || strcmp ( classid , "muse" ) == 0)
    {
         if ( GM || ((thisclient->Stats->Level >= 10) && (classid_new == 0 ))) // visitor
                classid_new = 211;
    }
    else if ( strcmp ( classid , "Mage" ) == 0 || strcmp ( classid , "mage" ) == 0)
    {
        if ( GM || ((thisclient->Stats->Level >= 100) && (classid_new == 211)))  // muse
                classid_new = 221;
    }
    else if ( strcmp ( classid , "Cleric" ) == 0 || strcmp ( classid , "cleric" ) == 0)
    {
        if ( GM || ((thisclient->Stats->Level >= 100) && (classid_new == 211)))  // muse
                classid_new = 222;
    }
    else if ( strcmp ( classid , "Hawker" ) == 0 || strcmp ( classid , "hawker" ) == 0)
    {
         if ( GM || ((thisclient->Stats->Level >= 10) && (classid_new == 0 ))) // visitor
                classid_new = 311;
    }
    else if ( strcmp ( classid , "Raider" ) == 0 || strcmp ( classid , "raider" ) == 0)
    {
        if ( GM || ((thisclient->Stats->Level >= 100) && (classid_new == 311)))  // hawker
                classid_new = 321;
    }
    else if ( strcmp ( classid , "Scout" ) == 0 || strcmp ( classid , "scout" ) == 0)
    {
        if ( GM || ((thisclient->Stats->Level >= 100) && (classid_new == 311)))  // hawker
                classid_new = 322;
    }
    else if ( strcmp ( classid , "Dealer" ) == 0 || strcmp ( classid , "dealer" ) == 0)
    {
         if ( GM || ((thisclient->Stats->Level >= 10) && (classid_new == 0 ))) // visitor         
               classid_new = 411;
    }
    else if ( strcmp ( classid , "Bourgeois" ) == 0 || strcmp ( classid , "bourgeois" ) == 0)
    {
        if ( GM || ((thisclient->Stats->Level >= 100) && (classid_new == 411)))  // dealer
                classid_new = 421;
    }
    else if ( strcmp ( classid , "Artisan" ) == 0 || strcmp ( classid , "artisan" ) == 0)
    {
        if ( GM || ((thisclient->Stats->Level >= 100) && (classid_new == 411)))  // dealer
                classid_new = 422;
    }    
    else
    {
        return true;
    }
    bool changed = true;
    if ( thisclient->CharInfo->Job == classid_new )
       changed = false;
    thisclient->CharInfo->Job = classid_new;
    BEGINPACKET(pak, 0x0721);
    ADDWORD(pak,4);
    ADDWORD(pak, thisclient->CharInfo->Job);
    ADDWORD(pak,0);
    thisclient->client->SendPacket(&pak);
    RESETPACKET(pak, 0x0730);
    ADDWORD(pak, 5);
    ADDWORD(pak, 0xa24d);
    ADDWORD(pak, 0x40b3);
    thisclient->client->SendPacket(&pak);
    
    if ( changed )
    {
       SendPM(thisclient, "Class changed!" ); 
    } 
    else
    {
       if ( thisclient->Stats->Level < 10 )
          SendPM(thisclient, "Class change failed! You must be at least lvl 10 to change your job." ); 
       else if (classid_new == 0) // visitor
          SendPM(thisclient, "Class change failed! Pick a first job, muse, dealer, hawker, or solder" ); 
       else  
          SendPM(thisclient, "Class change failed!" );        
    }
     
    return true;
}
 


bool CWorldServer::pakGMTeleAllHere( CPlayer* thisclient )
{
    int count=1;
    while(count <= (ClientList.size()-1)) 
    {
        CPlayer* otherclient = (CPlayer*)ClientList.at(count)->player;
        if ((otherclient != NULL) && (otherclient != thisclient))
        {
            CMap* map = MapList.Index[thisclient->Position->Map];
            map->TeleportPlayer( otherclient, thisclient->Position->current, false );
        }
        count++;
    }
    return true;
}

bool CWorldServer::pakGMMoveTo( CPlayer* thisclient, fPoint position )
{
    if( thisclient->Shop->open || (!thisclient->Ride->Drive && thisclient->Ride->Ride) || !thisclient->Status->CanMove )
        return true;
    if( thisclient->Status->Stance==1 )
        thisclient->Status->Stance=3;    
    thisclient->Position->destiny = position; // PAKGMMOVETO
    ClearBattle( thisclient->Battle );
	BEGINPACKET( pak, 0x79a );
	ADDWORD    ( pak, thisclient->clientid );
	ADDWORD    ( pak, thisclient->Battle->target );	
	ADDWORD    ( pak, thisclient->Stats->Move_Speed );
	ADDFLOAT   ( pak, thisclient->Position->destiny.x*100 );
	ADDFLOAT   ( pak, thisclient->Position->destiny.y*100 );	
	ADDFLOAT   ( pak, thisclient->Position->destiny.z*100 );	
    SendToVisible( &pak, thisclient );      	
    return true;
}

//change the player dmg rate
bool CWorldServer::pakGMChangePlayerDmg(CPlayer* thisclient, int rate)
{
    GServer->Config.PlayerDmg = rate;
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "You have changed the player dmg rate.");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
	return true;
}
//change the monster dmg rate
bool CWorldServer::pakGMChangeMonsterDmg(CPlayer* thisclient, int rate)
{
    GServer->Config.MonsterDmg = rate;
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "You have changed the Monster dmg rate.");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
	return true;
}
//change the Cfmode
bool CWorldServer::pakGMChangeCfmode(CPlayer* thisclient, int mode)
{
    if(mode<0) mode = 0;
    if(mode>1) mode = 1;
    GServer->Config.Cfmode = mode;
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "You have changed the Clan Field Mode.");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
	return true;
}

bool CWorldServer::pakGMWhoAttacksMe(CPlayer* thisclient)
{
     std::cout << "You are attacked by these Mobs: ";
     CMap* map = GServer->MapList.Index[thisclient->Position->Map]; 
     for(UINT i=0;i<map->MonsterList.size();i++)
    {
        CMonster* thismon = map->MonsterList.at( i );
		float distance = GServer->distance ( thisclient->Position->current, thismon->Position->current );        		
		if (thismon->Battle->target == thisclient->clientid)
           std::cout <<"ID: "<<thismon->clientid<< "  / " <<thismon->montype<<" I "<< (GServer->IsVisible(thisclient, thismon) ? "Visible\n":"Invisible\n");
	} 
	return true;
}

//change the AtkSpeedModif || For test only
bool CWorldServer::pakGMChangeAtkSpeedModif(CPlayer* thisclient, int modif)
{
    GServer->ATTK_SPEED_MODIF = modif;
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "AttkSpeedModif Changed.");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
	return true;
}

//change the HitDelayModif || For test only
bool CWorldServer::pakGMChangeHitDelayModif(CPlayer* thisclient, int modif)
{
    GServer->HIT_DELAY_MODIF = modif;
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "HitDelayModif Changed.");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
	return true;
}

//change the mSpeedModif || For test only
bool CWorldServer::pakGMChangeMSpeedModif(CPlayer* thisclient, int modif)
{
    GServer->MOVE_SPEED_MODIF = modif;
    BEGINPACKET (pak, 0x702);
    ADDSTRING(pak, "mSpeedModif Changed.");
    ADDBYTE(pak, 0);
    thisclient->client->SendPacket(&pak);
	return true;
}

// GM Debuff players in sight. by Drakia
bool CWorldServer::pakGMDebuff(CPlayer* thisClient)
{
    for(int i = 0; i < 30; i++)
    {
        thisClient->MagicStatus[i].Duration = 0;
        thisClient->MagicStatus[i].BuffTime = 0;
    }
    thisClient->RefreshBuff();
    for (int i = 0; i < thisClient->VisiblePlayers.size(); i++)
    {
        CPlayer* targetClient = thisClient->VisiblePlayers[i];
        if (targetClient->Session->isLoggedIn == false) continue;
        if (targetClient->Stats->HP <= 0) continue;
        for(int j = 0; j < 30; j++)
        {
            targetClient->MagicStatus[j].Duration = 0;
            targetClient->MagicStatus[j].BuffTime = 0;
        }
        targetClient->RefreshBuff();
    }
    return true;
}
 
// GM Buff players in sight. by Drakia
bool CWorldServer::pakGMBuff( CPlayer* thisClient, int strength )
{
    // Buff the GM
    pakGMDebuff(thisClient);
 
    pakGMGiveBuff( thisClient, thisClient, 3906, strength); // Attack   (300s) (18)
    pakGMGiveBuff( thisClient, thisClient, 3905, strength); // Defense  (300s) (19)
    pakGMGiveBuff( thisClient, thisClient, 3908, strength); // Accuracy (420s) (20)
    pakGMGiveBuff( thisClient, thisClient, 3907, strength); // MResist  (420s) (21)
    pakGMGiveBuff( thisClient, thisClient, 3909, strength); // Dodge    (420s) (22)
    pakGMGiveBuff( thisClient, thisClient, 3904, strength); // Dash     (300s) (23)
    pakGMGiveBuff( thisClient, thisClient, 3910, strength); // Haste    (420s) (24)
    pakGMGiveBuff( thisClient, thisClient, 3911, strength); // Critical (420s) (26)
    pakGMGiveBuff( thisClient, thisClient, 3900, strength); // Max HP   (420s) (38)
    pakGMGiveBuff( thisClient, thisClient, 3901, strength); // Max MP   (420s) (39)
    // Buff all players visible
    for (int i = 0; i < thisClient->VisiblePlayers.size(); i++)
    {
        CPlayer* target = thisClient->VisiblePlayers[i];
        if ( target->Session->isLoggedIn == false ) continue;
        if ( target->Stats->HP <= 0 ) continue;
        pakGMGiveBuff( thisClient, target, 3906, strength); // Attack   (300s) (18)
        pakGMGiveBuff( thisClient, target, 3905, strength); // Defense  (300s) (19)
        pakGMGiveBuff( thisClient, target, 3908, strength); // Accuracy (420s) (20)
        pakGMGiveBuff( thisClient, target, 3907, strength); // MResist  (420s) (21)
        pakGMGiveBuff( thisClient, target, 3909, strength); // Dodge    (420s) (22)
        pakGMGiveBuff( thisClient, target, 3904, strength); // Dash     (300s) (23)
        pakGMGiveBuff( thisClient, target, 3910, strength); // Haste    (420s) (24)
        pakGMGiveBuff( thisClient, target, 3911, strength); // Critical (420s) (26)
        pakGMGiveBuff( thisClient, target, 3900, strength); // Max HP   (420s) (38)
        pakGMGiveBuff( thisClient, target, 3901, strength); // Max MP   (420s) (39)
 
    }
    return true;
}
 
// Find the skill, add the buff, and send the packet. This is where the magic happens
// And what took me so freakin long to figure out x.x
bool CWorldServer::pakGMGiveBuff(CPlayer* thisClient, CPlayer* targetClient, int skillID, int strength)
{
    CSkills* skill = GServer->GetSkillByID(skillID);
 
    GServer->AddBuffs( skill, targetClient, strength );
    BEGINPACKET( pak, 0x7b5 );
    ADDWORD    ( pak, targetClient->clientid );
    ADDWORD    ( pak, thisClient->clientid );
    ADDWORD    ( pak, skillID );
    ADDWORD    ( pak, strength );
    ADDBYTE    ( pak, skill->nbuffs );
    GServer->SendToVisible( &pak, targetClient );
    return true;
}
 
// GM Give yourself all stats maxed 
bool CWorldServer::pakGMMaxStats( CPlayer* thisclient )
{ 
 
        pakGMStat(thisclient, "str", 300);
        pakGMStat(thisclient, "dex", 300);
        pakGMStat(thisclient, "con", 300);
        pakGMStat(thisclient, "int", 300);
        pakGMStat(thisclient, "cha", 300);
        pakGMStat(thisclient, "sen", 300);
 
    return true;
} 

 
//GM: All Skills {By CrAshInSiDe} - Skills and levels updated by rl2171
    bool CWorldServer::pakGMAllSkill(CPlayer* thisclient, char* name)
{
    int classid = thisclient->CharInfo->Job;
    CPlayer* otherclient = GetClientByCharName( name );
    if(otherclient==NULL)
    return true;

if ( classid == 121 ) //Knight
{
otherclient->cskills[0].id = 201;
otherclient->cskills[0].level = 9;
otherclient->cskills[1].id = 211;
otherclient->cskills[1].level = 9;
otherclient->cskills[2].id = 251;
otherclient->cskills[2].level = 9;
otherclient->cskills[3].id = 221;
otherclient->cskills[3].level = 9;
otherclient->cskills[4].id = 261;
otherclient->cskills[4].level = 9;
otherclient->cskills[5].id = 231;
otherclient->cskills[5].level = 4;
otherclient->cskills[6].id = 241;
otherclient->cskills[6].level = 3;
otherclient->cskills[7].id = 236;
otherclient->cskills[7].level = 3;
otherclient->cskills[8].id = 311;
otherclient->cskills[8].level = 3;
otherclient->cskills[9].id = 271;
otherclient->cskills[9].level = 9;
otherclient->cskills[10].id = 281;
otherclient->cskills[10].level = 9;
otherclient->cskills[11].id = 291;
otherclient->cskills[11].level = 9;
otherclient->cskills[12].id = 301;
otherclient->cskills[12].level = 3;
otherclient->cskills[13].id = 306;
otherclient->cskills[13].level = 2;
otherclient->cskills[14].id = 426;
otherclient->cskills[14].level = 9;
otherclient->cskills[15].id = 431;
otherclient->cskills[15].level = 9;
otherclient->cskills[16].id = 441;
otherclient->cskills[16].level = 9;
otherclient->cskills[17].id = 461;
otherclient->cskills[17].level = 18;
otherclient->cskills[18].id = 481;
otherclient->cskills[18].level = 9;
otherclient->cskills[19].id = 496;
otherclient->cskills[19].level = 9;
otherclient->cskills[20].id = 491;
otherclient->cskills[20].level = 9;
otherclient->cskills[21].id = 451;
otherclient->cskills[21].level = 9;
otherclient->cskills[22].id = 506;
otherclient->cskills[22].level = 9;
otherclient->cskills[23].id = 516;
otherclient->cskills[23].level = 18;
otherclient->cskills[24].id = 721;
otherclient->cskills[24].level = 9;
otherclient->cskills[25].id = 726;
otherclient->cskills[25].level = 9;
otherclient->cskills[26].id = 731;
otherclient->cskills[26].level = 9;
otherclient->cskills[27].id = 421;
otherclient->cskills[27].level = 2;
otherclient->cskills[28].id = 416;
otherclient->cskills[28].level = 2;
otherclient->cskills[29].id = 501;
otherclient->cskills[29].level = 1;
otherclient->cskills[30].id = 516;
otherclient->cskills[30].level = 9;
otherclient->cskills[31].id = 721;
otherclient->cskills[31].level = 3;
otherclient->cskills[32].id = 726;
otherclient->cskills[32].level = 3;
otherclient->cskills[33].id = 496;
otherclient->cskills[33].level = 3;
otherclient->cskills[34].id = 491;
otherclient->cskills[34].level = 3;
// Mileage Items - knight - 1 handed
otherclient->cskills[35].id = 5601; // 847
otherclient->cskills[35].level = 1;
otherclient->cskills[36].id = 5602; // 848
otherclient->cskills[36].level = 1;

SendPM (thisclient, "Relogin For Get All Skills");
}

else if ( classid == 122 ) //Champion
{
otherclient->cskills[0].id = 201;
otherclient->cskills[0].level = 9;
otherclient->cskills[1].id = 251;
otherclient->cskills[1].level = 9;
otherclient->cskills[2].id = 221;
otherclient->cskills[2].level = 9;
otherclient->cskills[3].id = 211;
otherclient->cskills[3].level = 9;
otherclient->cskills[4].id = 231;
otherclient->cskills[4].level = 4;
otherclient->cskills[5].id = 261;
otherclient->cskills[5].level = 9;
otherclient->cskills[6].id = 416;
otherclient->cskills[6].level = 2;
otherclient->cskills[7].id = 421;
otherclient->cskills[7].level = 2;
otherclient->cskills[8].id = 316;
otherclient->cskills[8].level = 9;
otherclient->cskills[9].id = 241;
otherclient->cskills[9].level = 3;
otherclient->cskills[10].id = 326;
otherclient->cskills[10].level = 9;
otherclient->cskills[11].id = 356;
otherclient->cskills[11].level = 9;
otherclient->cskills[12].id = 386;
otherclient->cskills[12].level = 9;
otherclient->cskills[13].id = 336;
otherclient->cskills[13].level = 9;
otherclient->cskills[14].id = 366;
otherclient->cskills[14].level = 9;
otherclient->cskills[15].id = 396;
otherclient->cskills[15].level = 9;
otherclient->cskills[16].id = 346;
otherclient->cskills[16].level = 3;
otherclient->cskills[17].id = 376;
otherclient->cskills[17].level = 3;
otherclient->cskills[18].id = 406;
otherclient->cskills[18].level = 3;
otherclient->cskills[19].id = 506;
otherclient->cskills[19].level = 9;
otherclient->cskills[20].id = 516;
otherclient->cskills[20].level = 9;
otherclient->cskills[21].id = 311;
otherclient->cskills[21].level = 3;
otherclient->cskills[22].id = 501;
otherclient->cskills[22].level = 1;
otherclient->cskills[23].id = 236;
otherclient->cskills[23].level = 3;
otherclient->cskills[24].id = 351;
otherclient->cskills[24].level = 2;
otherclient->cskills[25].id = 381;
otherclient->cskills[25].level = 2;
otherclient->cskills[26].id = 411;
otherclient->cskills[26].level = 2;
otherclient->cskills[27].id = 531;
otherclient->cskills[27].level = 9;
otherclient->cskills[28].id = 541;
otherclient->cskills[28].level = 9;
otherclient->cskills[29].id = 561;
otherclient->cskills[29].level = 18;
otherclient->cskills[30].id = 611;
otherclient->cskills[30].level = 18;
otherclient->cskills[31].id = 661;
otherclient->cskills[31].level = 18;
otherclient->cskills[32].id = 711;
otherclient->cskills[32].level = 3;
otherclient->cskills[33].id = 581;
otherclient->cskills[33].level = 9;
otherclient->cskills[34].id = 631;
otherclient->cskills[34].level = 9;
otherclient->cskills[35].id = 681;
otherclient->cskills[35].level = 9;
otherclient->cskills[36].id = 696;
otherclient->cskills[36].level = 3;
otherclient->cskills[37].id = 596;
otherclient->cskills[37].level = 3;
otherclient->cskills[38].id = 646;
otherclient->cskills[38].level = 3;
otherclient->cskills[39].id = 551;
otherclient->cskills[39].level = 9;
otherclient->cskills[40].id = 716;
otherclient->cskills[40].level = 3;
otherclient->cskills[41].id = 591;
otherclient->cskills[41].level = 3;
otherclient->cskills[42].id = 691;
otherclient->cskills[42].level = 3;
otherclient->cskills[43].id = 641;
otherclient->cskills[43].level = 3;
otherclient->cskills[44].id = 651;
otherclient->cskills[44].level = 9;
otherclient->cskills[45].id = 701;
otherclient->cskills[45].level = 9;
otherclient->cskills[46].id = 601;
otherclient->cskills[46].level = 9;
// Mileage Items - 2 hand sword
otherclient->cskills[47].id = 5603; // 849
otherclient->cskills[47].level = 1;
otherclient->cskills[48].id = 5604; // 850
otherclient->cskills[48].level = 1;
// Mileage Items - spear
otherclient->cskills[49].id = 5605; // 851
otherclient->cskills[49].level = 1;
otherclient->cskills[50].id = 5606; // 852
otherclient->cskills[50].level = 1;
// Mileage Items - axe
otherclient->cskills[51].id = 5607; // 853
otherclient->cskills[51].level = 1;
otherclient->cskills[52].id = 5608; // 854
otherclient->cskills[52].level = 1;

SendPM (thisclient, "Relogin For Get All Skills");
}

else if ( classid == 221 ) //Mage
{
otherclient->cskills[0].id = 851;
otherclient->cskills[0].level = 9;
otherclient->cskills[1].id = 831;
otherclient->cskills[1].level = 18;
otherclient->cskills[2].id = 891;
otherclient->cskills[2].level = 9;
otherclient->cskills[3].id = 861;
otherclient->cskills[3].level = 9;
otherclient->cskills[4].id = 881;
otherclient->cskills[4].level = 3;
otherclient->cskills[5].id = 871;
otherclient->cskills[5].level = 3;
otherclient->cskills[6].id = 831;
otherclient->cskills[6].level = 9;
otherclient->cskills[7].id = 821;
otherclient->cskills[7].level = 9;
otherclient->cskills[8].id = 841;
otherclient->cskills[8].level = 2;
otherclient->cskills[9].id = 846;
otherclient->cskills[9].level = 2;
otherclient->cskills[10].id = 876;
otherclient->cskills[10].level = 1;
otherclient->cskills[11].id = 886;
otherclient->cskills[11].level = 1;
otherclient->cskills[12].id = 901;
otherclient->cskills[12].level = 6;
otherclient->cskills[13].id = 1046;
otherclient->cskills[13].level = 1;
otherclient->cskills[14].id = 1051;
otherclient->cskills[14].level = 18;
otherclient->cskills[15].id = 1316;
otherclient->cskills[15].level = 9;
otherclient->cskills[16].id = 1136;
otherclient->cskills[16].level = 1;
otherclient->cskills[17].id = 1071;
otherclient->cskills[17].level = 9;
otherclient->cskills[18].id = 1101;
otherclient->cskills[18].level = 3;
otherclient->cskills[19].id = 1121;
otherclient->cskills[19].level = 3;
otherclient->cskills[20].id = 1141;
otherclient->cskills[20].level = 9;
otherclient->cskills[21].id = 1106;
otherclient->cskills[21].level = 1;
otherclient->cskills[22].id = 1126;
otherclient->cskills[22].level = 1;
otherclient->cskills[23].id = 1081;
otherclient->cskills[23].level = 2;
otherclient->cskills[24].id = 1096;
otherclient->cskills[24].level = 2;
otherclient->cskills[25].id = 1151;
otherclient->cskills[25].level = 6;
otherclient->cskills[26].id = 1086;
otherclient->cskills[26].level = 9;
otherclient->cskills[27].id = 1111;
otherclient->cskills[27].level = 3;
otherclient->cskills[28].id = 1131;
otherclient->cskills[28].level = 3;
otherclient->cskills[29].id = 1116;
otherclient->cskills[29].level = 2;
otherclient->cskills[30].id = 1171;
otherclient->cskills[30].level = 2;
otherclient->cskills[31].id = 1176;
otherclient->cskills[31].level = 1;
otherclient->cskills[32].id = 1161;
otherclient->cskills[32].level = 9;
otherclient->cskills[33].id = 801;
otherclient->cskills[33].level = 18;
otherclient->cskills[34].id = 911;
otherclient->cskills[34].level = 9;
otherclient->cskills[35].id = 921;
otherclient->cskills[35].level = 3;
otherclient->cskills[36].id = 926;
otherclient->cskills[36].level = 3;
// Mileage Items - mage
otherclient->cskills[37].id = 5702; // 855
otherclient->cskills[37].level = 1;
otherclient->cskills[38].id = 5703; // 856
otherclient->cskills[38].level = 1;
// Mileage Item - Soul Recall
otherclient->cskills[39].id = 5701; // 592
otherclient->cskills[39].level = 1;
// Unique Item - Soul Recall
otherclient->cskills[40].id = 4401; // 560
otherclient->cskills[40].level = 3;

SendPM (thisclient, "Relogin For Get All Skills");
}

else if ( classid == 222 ) //Cleric
{
otherclient->cskills[0].id = 931;
otherclient->cskills[0].level = 18;
otherclient->cskills[1].id = 911;
otherclient->cskills[1].level = 9;
otherclient->cskills[2].id = 921;
otherclient->cskills[2].level = 3;
otherclient->cskills[3].id = 926;
otherclient->cskills[3].level = 3;
otherclient->cskills[4].id = 951;
otherclient->cskills[4].level = 9;
otherclient->cskills[5].id = 986;
otherclient->cskills[5].level = 6;
otherclient->cskills[6].id = 1021;
otherclient->cskills[6].level = 3;
otherclient->cskills[7].id = 1031;
otherclient->cskills[7].level = 5;
otherclient->cskills[8].id = 976;
otherclient->cskills[8].level = 1;
otherclient->cskills[9].id = 996;
otherclient->cskills[9].level = 6;
otherclient->cskills[10].id = 1006;
otherclient->cskills[10].level = 1;
otherclient->cskills[11].id = 1036;
otherclient->cskills[11].level = 3;
otherclient->cskills[12].id = 1041;
otherclient->cskills[12].level = 4;
otherclient->cskills[13].id = 961;
otherclient->cskills[13].level = 9;
otherclient->cskills[14].id = 971;
otherclient->cskills[14].level = 3;
otherclient->cskills[15].id = 981;
otherclient->cskills[15].level = 3;
otherclient->cskills[16].id = 1011;
otherclient->cskills[16].level = 6;
otherclient->cskills[17].id = 1181;
otherclient->cskills[17].level = 1;
otherclient->cskills[18].id = 1186;
otherclient->cskills[18].level = 18;
otherclient->cskills[19].id = 1296;
otherclient->cskills[19].level = 9;
otherclient->cskills[20].id = 1306;
otherclient->cskills[20].level = 9;
otherclient->cskills[21].id = 1241;
otherclient->cskills[21].level = 1;
otherclient->cskills[22].id = 1211;
otherclient->cskills[22].level = 9;
otherclient->cskills[23].id = 1206;
otherclient->cskills[23].level = 2;
otherclient->cskills[24].id = 1246;
otherclient->cskills[24].level = 6;
otherclient->cskills[26].id = 1256;
otherclient->cskills[26].level = 3;
otherclient->cskills[27].id = 1266;
otherclient->cskills[27].level = 1;
otherclient->cskills[28].id = 1276;
otherclient->cskills[28].level = 6;
otherclient->cskills[29].id = 1271;
otherclient->cskills[29].level = 1;
otherclient->cskills[30].id = 1226;
otherclient->cskills[30].level = 9;
otherclient->cskills[31].id = 1236;
otherclient->cskills[31].level = 3;
otherclient->cskills[32].id = 1261;
otherclient->cskills[32].level = 3;
otherclient->cskills[33].id = 1176;
otherclient->cskills[33].level = 1;
otherclient->cskills[34].id = 1286;
otherclient->cskills[34].level = 6;
otherclient->cskills[35].id = 1221;
otherclient->cskills[35].level = 3;
// Mileage Items - cleric
otherclient->cskills[36].id = 5704; // 857
otherclient->cskills[36].level = 1;
otherclient->cskills[37].id = 5705; // 858
otherclient->cskills[37].level = 1;
// Mileage Item - Soul Recall
otherclient->cskills[38].id = 5701; // 592
otherclient->cskills[38].level = 1;
// Unique Item - Soul Recall
otherclient->cskills[39].id = 4401; // 560
otherclient->cskills[39].level = 3;

SendPM (thisclient, "Relogin For Get All Skills");
}

else if ( classid == 321 ) //Raider
{
otherclient->cskills[0].id = 1401;
otherclient->cskills[0].level = 9;
otherclient->cskills[1].id = 1521;
otherclient->cskills[1].level = 18;
otherclient->cskills[2].id = 1541;
otherclient->cskills[2].level = 9;
otherclient->cskills[3].id = 1551;
otherclient->cskills[3].level = 9;
otherclient->cskills[4].id = 1561;
otherclient->cskills[4].level = 9;
otherclient->cskills[5].id = 1571;
otherclient->cskills[5].level = 9;
otherclient->cskills[6].id = 1581;
otherclient->cskills[6].level = 9;
otherclient->cskills[7].id = 1591;
otherclient->cskills[7].level = 9;
otherclient->cskills[8].id = 1601;
otherclient->cskills[8].level = 1;
otherclient->cskills[9].id = 1606;
otherclient->cskills[9].level = 9;
otherclient->cskills[10].id = 1616;
otherclient->cskills[10].level = 9;
otherclient->cskills[11].id = 1626;
otherclient->cskills[11].level = 1;
otherclient->cskills[12].id = 1631;
otherclient->cskills[12].level = 2;
otherclient->cskills[13].id = 1636;
otherclient->cskills[13].level = 2;
otherclient->cskills[14].id = 1641;
otherclient->cskills[14].level = 1;
otherclient->cskills[15].id = 1646;
otherclient->cskills[15].level = 1;
otherclient->cskills[16].id = 1651;
otherclient->cskills[16].level = 1;
otherclient->cskills[17].id = 1776;
otherclient->cskills[17].level = 1;
otherclient->cskills[18].id = 1781;
otherclient->cskills[18].level = 18;
otherclient->cskills[19].id = 1801;
otherclient->cskills[19].level = 9;
otherclient->cskills[20].id = 1811;
otherclient->cskills[20].level = 2;
otherclient->cskills[21].id = 1946;
otherclient->cskills[21].level = 2;
otherclient->cskills[22].id = 1816;
otherclient->cskills[22].level = 9;
otherclient->cskills[23].id = 1826;
otherclient->cskills[23].level = 9;
otherclient->cskills[24].id = 1951;
otherclient->cskills[24].level = 2;
otherclient->cskills[25].id = 1836;
otherclient->cskills[25].level = 3;
otherclient->cskills[26].id = 1841;
otherclient->cskills[26].level = 9;
otherclient->cskills[27].id = 1851;
otherclient->cskills[27].level = 1;
otherclient->cskills[28].id = 1856;
otherclient->cskills[28].level = 1;
otherclient->cskills[29].id = 1861;
otherclient->cskills[29].level = 9;
otherclient->cskills[30].id = 1871;
otherclient->cskills[30].level = 3;
otherclient->cskills[31].id = 1881;
otherclient->cskills[31].level = 2;
otherclient->cskills[32].id = 1886;
otherclient->cskills[32].level = 2;
otherclient->cskills[33].id = 1891;
otherclient->cskills[33].level = 2;
otherclient->cskills[34].id = 1896;
otherclient->cskills[34].level = 9;
otherclient->cskills[35].id = 1906;
otherclient->cskills[35].level = 9;
otherclient->cskills[36].id = 1916;
otherclient->cskills[36].level = 9;
otherclient->cskills[37].id = 1926;
otherclient->cskills[37].level = 9;
otherclient->cskills[38].id = 1936;
otherclient->cskills[38].level = 9;
otherclient->cskills[39].id = 1876;
otherclient->cskills[39].level = 2;
otherclient->cskills[40].id = 1811;
otherclient->cskills[40].level = 2;
otherclient->cskills[41].id = 1506;
otherclient->cskills[41].level = 3;
otherclient->cskills[42].id = 1511;
otherclient->cskills[42].level = 2;
otherclient->cskills[43].id = 1766;
otherclient->cskills[43].level = 3;
otherclient->cskills[44].id = 1761;
otherclient->cskills[44].level = 3;
otherclient->cskills[45].id = 1516;
otherclient->cskills[45].level = 1;
otherclient->cskills[46].id = 1771;
otherclient->cskills[46].level = 3;
// Mileage Items - raider - katar
otherclient->cskills[47].id = 5803; // 861
otherclient->cskills[47].level = 1;
otherclient->cskills[48].id = 5804; // 862
otherclient->cskills[48].level = 1;
// Mileage Items - raider - dual
otherclient->cskills[49].id = 5805; // 863
otherclient->cskills[49].level = 1;
otherclient->cskills[50].id = 5806; // 864
otherclient->cskills[50].level = 1;

SendPM (thisclient, "Relogin For Get All Skills");
}

else if ( classid == 322 ) //Scout
{
otherclient->cskills[0].id = 1401;
otherclient->cskills[0].level = 9;
otherclient->cskills[1].id = 1411;
otherclient->cskills[1].level = 18;
otherclient->cskills[2].id = 1431;
otherclient->cskills[2].level = 9;
otherclient->cskills[3].id = 1441;
otherclient->cskills[3].level = 9;
otherclient->cskills[4].id = 1451;
otherclient->cskills[4].level = 9;
otherclient->cskills[5].id = 1461;
otherclient->cskills[5].level = 6;
otherclient->cskills[6].id = 1471;
otherclient->cskills[6].level = 3;
otherclient->cskills[7].id = 1496;
otherclient->cskills[7].level = 3;
otherclient->cskills[8].id = 1476;
otherclient->cskills[8].level = 9;
otherclient->cskills[9].id = 1486;
otherclient->cskills[9].level = 9;
otherclient->cskills[10].id = 1501;
otherclient->cskills[10].level = 1;
otherclient->cskills[11].id = 1656;
otherclient->cskills[11].level = 1;
otherclient->cskills[12].id = 1661;
otherclient->cskills[12].level = 18;
otherclient->cskills[13].id = 1681;
otherclient->cskills[13].level = 9;
otherclient->cskills[14].id = 1691;
otherclient->cskills[14].level = 9;
otherclient->cskills[15].id = 1701;
otherclient->cskills[15].level = 9;
otherclient->cskills[16].id = 1711;
otherclient->cskills[16].level = 1;
otherclient->cskills[17].id = 1716;
otherclient->cskills[17].level = 3;
otherclient->cskills[18].id = 1721;
otherclient->cskills[18].level = 6;
otherclient->cskills[19].id = 1731;
otherclient->cskills[19].level = 3;
otherclient->cskills[20].id = 1736;
otherclient->cskills[20].level = 9;
otherclient->cskills[21].id = 1746;
otherclient->cskills[21].level = 9;
otherclient->cskills[22].id = 1756;
otherclient->cskills[22].level = 3;
otherclient->cskills[23].id = 1506;
otherclient->cskills[23].level = 3;
otherclient->cskills[24].id = 1766;
otherclient->cskills[24].level = 3;
otherclient->cskills[25].id = 1511;
otherclient->cskills[25].level = 2;
otherclient->cskills[26].id = 1761;
otherclient->cskills[26].level = 3;
otherclient->cskills[27].id = 1516;
otherclient->cskills[27].level = 1;
otherclient->cskills[28].id = 1771;
otherclient->cskills[28].level = 3;
otherclient->cskills[29].id = 1956;
otherclient->cskills[29].level = 2;
otherclient->cskills[30].id = 5571;
otherclient->cskills[30].level = 1;
otherclient->cskills[31].id = 5570;
otherclient->cskills[31].level = 1;
otherclient->cskills[32].id = 5574;
otherclient->cskills[32].level = 1;
// Mileage Items - scout
otherclient->cskills[33].id = 5801; // 859
otherclient->cskills[33].level = 1;
otherclient->cskills[34].id = 5802; // 860
otherclient->cskills[34].level = 1;
// Unique Items - hawker
otherclient->cskills[35].id = 4601; // 565
otherclient->cskills[35].level = 9;

SendPM (thisclient, "Relogin For Get All Skills");
}

else if ( classid == 421 ) //Bourgeois
{
otherclient->cskills[0].id = 2001;
otherclient->cskills[0].level = 9;
otherclient->cskills[1].id = 2011;
otherclient->cskills[1].level = 18;
otherclient->cskills[2].id = 2036;
otherclient->cskills[2].level = 9;
otherclient->cskills[3].id = 2031;
otherclient->cskills[3].level = 3;
otherclient->cskills[4].id = 2056;
otherclient->cskills[4].level = 2;
otherclient->cskills[5].id = 2046;
otherclient->cskills[5].level = 3;
otherclient->cskills[6].id = 2051;
otherclient->cskills[6].level = 3;
otherclient->cskills[7].id = 2061;
otherclient->cskills[7].level = 9;
otherclient->cskills[8].id = 2071;
otherclient->cskills[8].level = 1;
otherclient->cskills[9].id = 2076;
otherclient->cskills[9].level = 9;
otherclient->cskills[10].id = 2086;
otherclient->cskills[10].level = 3;
otherclient->cskills[11].id = 2091;
otherclient->cskills[11].level = 3;
otherclient->cskills[12].id = 2096;
otherclient->cskills[12].level = 2;
otherclient->cskills[13].id = 2111;
otherclient->cskills[13].level = 3;
otherclient->cskills[14].id = 2101;
otherclient->cskills[14].level = 3;
otherclient->cskills[15].id = 2106;
otherclient->cskills[15].level = 3;
otherclient->cskills[16].id = 2256;
otherclient->cskills[16].level = 9;
otherclient->cskills[17].id = 2266;
otherclient->cskills[17].level = 18;
otherclient->cskills[18].id = 2286;
otherclient->cskills[18].level = 9;
otherclient->cskills[19].id = 2296;
otherclient->cskills[19].level = 1;
otherclient->cskills[20].id = 2301;
otherclient->cskills[20].level = 1;
otherclient->cskills[21].id = 2316;
otherclient->cskills[21].level = 9;
otherclient->cskills[22].id = 2306;
otherclient->cskills[22].level = 9;
otherclient->cskills[23].id = 2401;
otherclient->cskills[23].level = 1;
otherclient->cskills[24].id = 2326;
otherclient->cskills[24].level = 9;
otherclient->cskills[25].id = 2336;
otherclient->cskills[25].level = 9;
otherclient->cskills[26].id = 2346;
otherclient->cskills[26].level = 3;
otherclient->cskills[27].id = 2356;
otherclient->cskills[27].level = 3;
otherclient->cskills[28].id = 2366;
otherclient->cskills[28].level = 3;
otherclient->cskills[29].id = 2351;
otherclient->cskills[29].level = 3;
otherclient->cskills[30].id = 2376;
otherclient->cskills[30].level = 3;
otherclient->cskills[31].id = 2371;
otherclient->cskills[31].level = 3;
otherclient->cskills[32].id = 2381;
otherclient->cskills[32].level = 9;
otherclient->cskills[33].id = 2391;
otherclient->cskills[33].level = 9;
otherclient->cskills[34].id = 2406;
otherclient->cskills[34].level = 9;
otherclient->cskills[35].id = 2416;
otherclient->cskills[35].level = 9;
otherclient->cskills[36].id = 2426;
otherclient->cskills[36].level = 3;
otherclient->cskills[37].id = 2431;
otherclient->cskills[37].level = 3;
// Mileage Items - Borg - Launcher
otherclient->cskills[38].id = 5904; // 867
otherclient->cskills[38].level = 1;
otherclient->cskills[39].id = 5905; // 868
otherclient->cskills[39].level = 1;

SendPM (thisclient, "Relogin For Get All Skills");
}

else if ( classid == 422 ) //Artisan
{
otherclient->cskills[0].id = 2046;
otherclient->cskills[0].level = 3;
otherclient->cskills[1].id = 2056;
otherclient->cskills[1].level = 2;
otherclient->cskills[2].id = 2086;
otherclient->cskills[2].level = 3;
otherclient->cskills[3].id = 2031;
otherclient->cskills[3].level = 3;
otherclient->cskills[4].id = 2011;
otherclient->cskills[4].level = 18;
otherclient->cskills[5].id = 2116;
otherclient->cskills[5].level = 9;
otherclient->cskills[6].id = 2126;
otherclient->cskills[6].level = 9;
otherclient->cskills[7].id = 2196;
otherclient->cskills[7].level = 9;
otherclient->cskills[8].id = 2206;
otherclient->cskills[8].level = 9;
otherclient->cskills[9].id = 2216;
otherclient->cskills[9].level = 9;
otherclient->cskills[10].id = 2226;
otherclient->cskills[10].level = 9;
otherclient->cskills[11].id = 2156;
otherclient->cskills[11].level = 9;
otherclient->cskills[12].id = 2166;
otherclient->cskills[12].level = 9;
otherclient->cskills[13].id = 2176;
otherclient->cskills[13].level = 9;
otherclient->cskills[14].id = 2186;
otherclient->cskills[14].level = 9;
otherclient->cskills[15].id = 2001;
otherclient->cskills[15].level = 9;
otherclient->cskills[16].id = 2036;
otherclient->cskills[16].level = 9;
otherclient->cskills[17].id = 2076;
otherclient->cskills[17].level = 9;
otherclient->cskills[18].id = 2071;
otherclient->cskills[18].level = 1;
otherclient->cskills[19].id = 2061;
otherclient->cskills[19].level = 9;
otherclient->cskills[20].id = 2091;
otherclient->cskills[20].level = 3;
otherclient->cskills[21].id = 2096;
otherclient->cskills[21].level = 2;
otherclient->cskills[22].id = 2111;
otherclient->cskills[22].level = 3;
otherclient->cskills[23].id = 2101;
otherclient->cskills[23].level = 3;
otherclient->cskills[24].id = 2106;
otherclient->cskills[24].level = 3;
otherclient->cskills[25].id = 2136;
otherclient->cskills[25].level = 9;
otherclient->cskills[26].id = 2146;
otherclient->cskills[26].level = 9;
otherclient->cskills[27].id = 2236;
otherclient->cskills[27].level = 9;
otherclient->cskills[28].id = 2246;
otherclient->cskills[28].level = 9;
otherclient->cskills[29].id = 2441;
otherclient->cskills[29].level = 18;
otherclient->cskills[30].id = 2486;
otherclient->cskills[30].level = 9;
otherclient->cskills[31].id = 2496;
otherclient->cskills[31].level = 9;
otherclient->cskills[32].id = 2626;
otherclient->cskills[32].level = 9;
otherclient->cskills[33].id = 2461;
otherclient->cskills[33].level = 1;
otherclient->cskills[34].id = 2466;
otherclient->cskills[34].level = 9;
otherclient->cskills[35].id = 2526;
otherclient->cskills[35].level = 5;
otherclient->cskills[36].id = 2536;
otherclient->cskills[36].level = 4;
otherclient->cskills[37].id = 2546;
otherclient->cskills[37].level = 4;
otherclient->cskills[38].id = 2556;
otherclient->cskills[38].level = 3;
otherclient->cskills[39].id = 2566;
otherclient->cskills[39].level = 5;
otherclient->cskills[40].id = 2586;
otherclient->cskills[40].level = 1;
otherclient->cskills[41].id = 2596;
otherclient->cskills[41].level = 1;
otherclient->cskills[42].id = 2481;
otherclient->cskills[42].level = 3;
otherclient->cskills[43].id = 2476;
otherclient->cskills[43].level = 3;
otherclient->cskills[44].id = 2506;
otherclient->cskills[44].level = 9;
otherclient->cskills[45].id = 2516;
otherclient->cskills[45].level = 9;
otherclient->cskills[46].id = 2606;
otherclient->cskills[46].level = 2;
otherclient->cskills[47].id = 2616;
otherclient->cskills[47].level = 9;
otherclient->cskills[48].id = 2576;
otherclient->cskills[48].level = 1;
otherclient->cskills[49].id = 2636;
otherclient->cskills[49].level = 9;
otherclient->cskills[50].id = 2646;
otherclient->cskills[50].level = 2;
otherclient->cskills[51].id = 2656;
otherclient->cskills[51].level = 7;
// Mileage Items - artisan
otherclient->cskills[52].id = 5902; // 865
otherclient->cskills[52].level = 1;
otherclient->cskills[53].id = 5903; // 866
otherclient->cskills[53].level = 1;
// Unique Items - artisan
otherclient->cskills[54].id = 4801; // 561
otherclient->cskills[54].level = 9;
otherclient->cskills[55].id = 4811; // 562
otherclient->cskills[55].level = 9;

SendPM (thisclient, "Relogin For Get All Skills");
}

/*
// Mileage Items - For which jobs?
otherclient->cskills[xx].id = 5501; // 590 - all
otherclient->cskills[xx].level = 1;
otherclient->cskills[xx].id = 5502; // 591 - all
otherclient->cskills[xx].level = 1;
otherclient->cskills[xx].id = 5901; // 593  (can learn, but not showing)
otherclient->cskills[xx].level = 1;
// Unique Items - For which jobs?
otherclient->cskills[xx].id = 4401; // 567 - all
otherclient->cskills[xx].level = 1;
otherclient->cskills[xx].id = 4402; // 568 - all
otherclient->cskills[xx].level = 1;
otherclient->cskills[xx].id = xxxx; // 563 - all
otherclient->cskills[xx].level = x;
otherclient->cskills[xx].id = xxxx; // 564 - all
otherclient->cskills[xx].level = x;
otherclient->cskills[xx].id = xxxx; // 566 - dealer
otherclient->cskills[xx].level = x;
*/

else
{
    SendPM(thisclient, "Can't add skills for this class");
}

    return true;
}
 
//GM: DeleteSkills {modified from allskill command - rl2171}
    bool CWorldServer::pakGMDelSkills(CPlayer* thisclient, char* name)
{

    CPlayer* otherclient = GetClientByCharName( name );
    if(otherclient==NULL)
    return true;
    
{
otherclient->cskills[0].id = 0;
otherclient->cskills[0].level = 1;
otherclient->cskills[1].id = 0;
otherclient->cskills[1].level = 1;
otherclient->cskills[2].id = 0;
otherclient->cskills[2].level = 1;
otherclient->cskills[3].id = 0;
otherclient->cskills[3].level = 1;
otherclient->cskills[4].id = 0;
otherclient->cskills[4].level = 1;
otherclient->cskills[5].id = 0;
otherclient->cskills[5].level = 1;
otherclient->cskills[6].id = 0;
otherclient->cskills[6].level = 1;
otherclient->cskills[7].id = 0;
otherclient->cskills[7].level = 1;
otherclient->cskills[8].id = 0;
otherclient->cskills[8].level = 1;
otherclient->cskills[9].id = 0;
otherclient->cskills[9].level = 1;
otherclient->cskills[10].id = 0;
otherclient->cskills[10].level = 1;
otherclient->cskills[11].id = 0;
otherclient->cskills[11].level = 1;
otherclient->cskills[12].id = 0;
otherclient->cskills[12].level = 1;
otherclient->cskills[13].id = 0;
otherclient->cskills[13].level = 1;
otherclient->cskills[14].id = 0;
otherclient->cskills[14].level = 1;
otherclient->cskills[15].id = 0;
otherclient->cskills[15].level = 1;
otherclient->cskills[16].id = 0;
otherclient->cskills[16].level = 1;
otherclient->cskills[17].id = 0;
otherclient->cskills[17].level = 1;
otherclient->cskills[18].id = 0;
otherclient->cskills[18].level = 1;
otherclient->cskills[19].id = 0;
otherclient->cskills[19].level = 1;
otherclient->cskills[20].id = 0;
otherclient->cskills[20].level = 1;
otherclient->cskills[21].id = 0;
otherclient->cskills[21].level = 1;
otherclient->cskills[22].id = 0;
otherclient->cskills[22].level = 1;
otherclient->cskills[23].id = 0;
otherclient->cskills[23].level = 1;
otherclient->cskills[24].id = 0;
otherclient->cskills[24].level = 1;
otherclient->cskills[25].id = 0;
otherclient->cskills[25].level = 1;
otherclient->cskills[26].id = 0;
otherclient->cskills[26].level = 1;
otherclient->cskills[27].id = 0;
otherclient->cskills[27].level = 1;
otherclient->cskills[28].id = 0;
otherclient->cskills[28].level = 1;
otherclient->cskills[29].id = 0;
otherclient->cskills[29].level = 1;
otherclient->cskills[30].id = 0;
otherclient->cskills[30].level = 1;
otherclient->cskills[31].id = 0;
otherclient->cskills[31].level = 1;
otherclient->cskills[32].id = 0;
otherclient->cskills[32].level = 1;
otherclient->cskills[33].id = 0;
otherclient->cskills[33].level = 1;
otherclient->cskills[34].id = 0;
otherclient->cskills[34].level = 1;
otherclient->cskills[35].id = 0;
otherclient->cskills[35].level = 1;
otherclient->cskills[36].id = 0;
otherclient->cskills[36].level = 1;
otherclient->cskills[37].id = 0;
otherclient->cskills[37].level = 1;
otherclient->cskills[38].id = 0;
otherclient->cskills[38].level = 1;
otherclient->cskills[39].id = 0;
otherclient->cskills[39].level = 1;
otherclient->cskills[40].id = 0;
otherclient->cskills[40].level = 1;
otherclient->cskills[41].id = 0;
otherclient->cskills[41].level = 1;
otherclient->cskills[42].id = 0;
otherclient->cskills[42].level = 1;
otherclient->cskills[43].id = 0;
otherclient->cskills[43].level = 1;
otherclient->cskills[44].id = 0;
otherclient->cskills[44].level = 1;
otherclient->cskills[45].id = 0;
otherclient->cskills[45].level = 1;
otherclient->cskills[46].id = 0;
otherclient->cskills[46].level = 1;
otherclient->cskills[47].id = 0;
otherclient->cskills[47].level = 1;
otherclient->cskills[48].id = 0;
otherclient->cskills[48].level = 1;
otherclient->cskills[49].id = 0;
otherclient->cskills[49].level = 1;
otherclient->cskills[50].id = 0;
otherclient->cskills[50].level = 1;
otherclient->cskills[51].id = 0;
otherclient->cskills[51].level = 1;
otherclient->cskills[52].id = 0;
otherclient->cskills[52].level = 1;
otherclient->cskills[53].id = 0;
otherclient->cskills[53].level = 1;
otherclient->cskills[54].id = 0;
otherclient->cskills[54].level = 1;
otherclient->cskills[55].id = 0;
otherclient->cskills[55].level = 1;

SendPM (thisclient, "Relogin to remove All Skills");
}
    return true;
}    
