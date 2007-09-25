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


bool CWorldServer::LoadNPCData( )
{
	Log( MSG_LOAD, "NPC Data           " );    
	MYSQL_ROW row;    
	MYSQL_RES *result = DB->QStore("SELECT id,life,walkspeed,runspeed,drivespeed,weapon, subweapon,level,hp,attackpower,hitpower,defense,strength,evasion,attackspeed,AI,exp,dropid,money,item,tab1,tab2,tab3,specialtab,attackdistance,aggresive,shp,dialog FROM npc_data order by id");
    if(result==NULL) return false;
	while( row = mysql_fetch_row(result) )
    {
        CNPCData* newnpc = new (nothrow) CNPCData;
        if(newnpc==NULL)
        {
            Log( MSG_ERROR, "Error allocing memory" );
            continue;
        }
        newnpc->id = atoi(row[0]);
        newnpc->life = atoi(row[1]);
        newnpc->wspeed = atoi(row[2]);
        newnpc->rspeed = atoi(row[3]);
        newnpc->dspeed = atoi(row[4]);
        newnpc->weapon = atoi(row[5]);
        newnpc->subweapon = atoi(row[6]);
        newnpc->level = atoi(row[7]);
        newnpc->hp = atoi(row[8]);
        newnpc->atkpower = atoi(row[9]);
        newnpc->hitrate = atoi(row[10]);
        newnpc->defense = atoi(row[11]);
        newnpc->magicdefense = atoi(row[12]);
        newnpc->dodge = atoi(row[13]);
        newnpc->atkspeed = atof(row[14]);
        newnpc->AI = atoi(row[15]);
        newnpc->exp = atoi(row[16]);
        newnpc->dropid = atoi(row[17]);
        newnpc->money = atoi(row[18]);
        newnpc->item = atoi(row[19]);
        newnpc->tab1 = atoi(row[20]);
        newnpc->tab2 = atoi(row[21]);
        newnpc->tab3 = atoi(row[22]);
        newnpc->specialtab = atoi(row[23]);
        newnpc->atkdistance = atof(row[24])/100;
        newnpc->aggresive = atoi(row[25]);
        newnpc->shp = atoi(row[26]);  
        newnpc->dialogid = atoi(row[27]);  
        NPCData.push_back( newnpc );
    }     
	DB->QFree( );	
	return true;	
}

bool CWorldServer::LoadSkillData( )
{
	Log( MSG_LOAD, "Skills data            " );    
	MYSQL_ROW row;    
	MYSQL_RES *result = DB->QStore("SELECT id,level,sp,type,range,target,power,duration,mp, success,weapon,class,rskills,lskills,buff1,buffv11,buffv12, buff2,buffv21,buffv22,buff3,buffv31,buffv32,clevel,aoe,aoeradius,script,value1 FROM skills_data order by id");
    if(result==NULL) return false;
	while( row = mysql_fetch_row(result) )
    {
        CSkills* newskill = new (nothrow) CSkills;
        if(newskill==NULL)
        {
            Log(MSG_ERROR, "Error allocing memory" );
            DB->QFree( );
            return false;
        }
        newskill->id=atoi(row[0]);
        newskill->level=atoi(row[1]);
        newskill->sp=atoi(row[2]);  
        newskill->type=atoi(row[3]);
        newskill->range=atoi(row[4])/100;          
        newskill->target=atoi(row[5]);
        newskill->atkpower=atoi(row[6]);
        newskill->duration=atoi(row[7]);
        newskill->mp=atoi(row[8]);
        newskill->success=atoi(row[9]); 
        // Buffs get loaded              
        newskill->buff[0]=atoi(row[14]);
        newskill->value1[0]=atoi(row[15]);
        newskill->value2[0]=atoi(row[16]);
        newskill->buff[1]=atoi(row[17]);
        newskill->value1[1]=atoi(row[18]);
        newskill->value2[1]=atoi(row[19]);
        newskill->buff[2]=atoi(row[20]);
        newskill->value1[2]=atoi(row[21]);
        newskill->value2[2]=atoi(row[22]);
        
        
        newskill->clevel=atoi(row[23]);            
        newskill->aoe=atoi(row[24]);
        newskill->aoeradius=atoi(row[25])/100;
        newskill->script=atoi(row[26]);
        newskill->svalue1=atoi(row[27]);
        newskill->nbuffs = 0; 
        if(newskill->buff[0]!=0)
            newskill->nbuffs += 1;
        if(newskill->buff[1]!=0)
            newskill->nbuffs += 2;            
        if(newskill->buff[2]!=0)
            newskill->nbuffs += 4;                        
        char *tmp;
        //weapon list
        if((tmp = strtok( row[10] , "|"))==NULL)
            newskill->weapon[0]=0;
        else
            newskill->weapon[0]=atoi(tmp);            
        for(unsigned int i=1;i<5; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
                newskill->weapon[i]=0;            
            else
                newskill->weapon[i]=atoi(tmp);            
        }
        //class list
        if((tmp = strtok( row[11] , "|"))==NULL)
            newskill->c_class[0]=0;
        else
            newskill->c_class[0]=atoi(tmp);            
        for(unsigned int i=1;i<4; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
                newskill->c_class[i]=0;            
            else
                newskill->c_class[i]=atoi(tmp);            
        }
        //rskill list
        if((tmp = strtok( row[12] , "|"))==NULL)
            newskill->rskill[0]=0;
        else
            newskill->rskill[0]=atoi(tmp);            
        for(unsigned int i=1;i<3; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
                newskill->rskill[i]=0;            
            else
                newskill->rskill[i]=atoi(tmp);            
        }        
        
        //rskill level list
        if((tmp = strtok( row[13] , "|"))==NULL)
            newskill->lskill[0]=0;
        else
            newskill->lskill[0]=atoi(tmp);            
        for(unsigned int i=1;i<3; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
                newskill->lskill[i]=0;            
            else
                newskill->lskill[i]=atoi(tmp);            
        }   
        SkillList.push_back( newskill ); 
	}
	DB->QFree( );	    
	return true;	
}

bool CWorldServer::LoadTeleGateData( )
{
	Log( MSG_LOAD, "Telegates data       " );     
	MYSQL_ROW row;    
	MYSQL_RES *result = DB->QStore("SELECT id,x,y,map FROM list_telegates");	
	if(result==NULL) return false;
	while( row = mysql_fetch_row(result) )
    {
		CTeleGate* thisgate = new (nothrow) CTeleGate;
        if(thisgate==NULL)
        {
            Log(MSG_ERROR, "Error allocing memory       " );
            DB->QFree( );
            return false;
        }		
		thisgate->id = atoi(row[0]);
		thisgate->dest.x = (float)atof(row[1]);
		thisgate->dest.y = (float)atof(row[2]);
		thisgate->destMap = atoi(row[3]);
		TeleGateList.push_back( thisgate );
	}
	DB->QFree( );   
	return true;	
}

bool CWorldServer::LoadQuestData( )
{
    Log( MSG_LOAD, "Quest data        " );    
	MYSQL_ROW row;
	MYSQL_RES *result = DB->QStore("SELECT id,questid,mobs,items,itemsreward,exp,zulie,itemid,numitems,finalid,nitems, citems,script,value1,value2,value3,startItems FROM quest_data order by id");
    if(result==NULL) return false;
	while( row = mysql_fetch_row(result) )
    {    
		CQuest* thisquest = new (nothrow) CQuest;
        if(thisquest==NULL)
        {
            Log(MSG_ERROR, "Error allocing memory" );
            DB->QFree( );
            return false;
        }	
        thisquest->id = atoi(row[0]);
        thisquest->questid = atoi(row[1]);
        if( thisquest->questid == 0 )
        {
            delete thisquest;
            continue;
        }                                     
        char *tmp;
        //load quest mobs 
        if((tmp = strtok( row[2] , "|"))==NULL)
            thisquest->mobs[0]=0;
        else
            thisquest->mobs[0]=atoi(tmp);            
        for(int i=1;i<10; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
                thisquest->mobs[i]=0;            
            else
                thisquest->mobs[i]=atoi(tmp);            
        }
        //load quest items 
        if((tmp = strtok( row[3] , "|"))==NULL)
            thisquest->items[0]=0;
        else
            thisquest->items[0]=atoi(tmp);            
        for(int i=1;i<10; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
                thisquest->items[i]=0;            
            else
                thisquest->items[i]=atoi(tmp);            
        }    
        //load quest start items 
        if((tmp = strtok( row[16] , "|"))==NULL)
            thisquest->startItems[0]=0;
        else
            thisquest->startItems[0]=atoi(tmp);                            
        for(int i=1;i<10; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
                thisquest->startItems[i]=0;            
            else
                thisquest->startItems[i]=atoi(tmp);            
        }
        
         //load quests variables
        tmp = strtok( row[16] , "#");
        if((tmp = strtok( NULL , "#"))==NULL)
            thisquest->QVoffset=0;
        else
            thisquest->QVoffset=atoi(tmp);
        if((tmp = strtok( NULL , "#"))==NULL)
            thisquest->QVvalue=0;
        else
            thisquest->QVvalue=atoi(tmp); 
        //load reward items
        if((tmp = strtok( row[4] , "|"))==NULL)
        {
            thisquest->Itemreward[0] = 0;
            thisquest->ItemType[0] = 0;            
        }
        else
        {
            thisquest->Itemreward[0] = atoi(tmp)%1000;    
            thisquest->ItemType[0] = atoi(tmp)/1000;         
        }
        for(int i=1;i<10; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
            {
                thisquest->Itemreward[i]=0;   
                thisquest->ItemType[i]=0;                         
            }
            else
            {
                thisquest->Itemreward[i] = atoi(tmp)%1000;            
                thisquest->ItemType[i] = atoi(tmp)/1000;                         
            }            
        }    
       //Load our items
        if((tmp = strtok( row[7] , "|"))==NULL)
            thisquest->itemid[0]=0;
        else
            thisquest->itemid[0]=atoi(tmp);            
        for(int i=1;i<10; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
                thisquest->itemid[i]=0;            
            else
                thisquest->itemid[i]=atoi(tmp);            
        }           
        //Load our items count
        if((tmp = strtok( row[8] , "|"))==NULL)
            thisquest->numitems[0]=0;
        else
            thisquest->numitems[0]=atoi(tmp);            
        for(int i=1;i<10; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
                thisquest->numitems[i]=0;            
            else
                thisquest->numitems[i]=atoi(tmp);            
        }   
        //Load our items reward count
        if((tmp = strtok( row[10] , "|"))==NULL)
            thisquest->CountItem[0]=0;
        else
            thisquest->CountItem[0]=atoi(tmp);            
        for(int i=1;i<10; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
                thisquest->CountItem[i]=0;            
            else
                thisquest->CountItem[i]=atoi(tmp);            
        }     
        //Load our quest items
        if((tmp = strtok( row[11] , "|"))==NULL)
            thisquest->CItem[0]=0;
        else
            thisquest->CItem[0]=atoi(tmp);            
        for(int i=1;i<5; i++) 
        {
            if((tmp = strtok( NULL , "|"))==NULL)
                thisquest->CItem[i]=0;            
            else
                thisquest->CItem[i]=atoi(tmp);            
        }                                                  
        thisquest->ExpReward = atoi( row[5] );
        thisquest->ZulieReward = atoi( row[6] );  
        thisquest->finalid = atoi( row[9] );   
        thisquest->script = atoi( row[12] );
        thisquest->value1 = atoi( row[13] );
        thisquest->value2 = atoi( row[14] );
        thisquest->value3 = atoi( row[15] );                        
		QuestList.push_back( thisquest );
	}
	DB->QFree( );	 
	return true;    
}

bool CWorldServer::LoadRespawnData( )
{
	Log( MSG_LOAD, "RespawnZones data        " );     
	MYSQL_ROW row;
	MYSQL_RES *result = DB->QStore("SELECT id,x,y,map,radius,type FROM list_respawnzones");
    if(result==NULL) return false;    	
	while( row = mysql_fetch_row(result) )
    {
		CRespawnPoint* thisrespawnpoint = new (nothrow) CRespawnPoint;
        if(thisrespawnpoint==NULL)
        {
            Log(MSG_ERROR, "Error allocing memory" );
            DB->QFree( );
            return false;
        }	
		thisrespawnpoint->id = atoi(row[0]);
		thisrespawnpoint->dest.x = (float)atof(row[1]);
		thisrespawnpoint->dest.y = (float)atof(row[2]);
		thisrespawnpoint->destMap = atoi(row[3]);
		thisrespawnpoint->radius = atoi(row[4]);
		thisrespawnpoint->masterdest = (atoi(row[5]) == 1);
		MapList.Index[thisrespawnpoint->destMap]->RespawnList.push_back( thisrespawnpoint );
	}
	DB->QFree( );
	return true;       
}

bool CWorldServer::LoadChestData( )
{
    Log(MSG_LOAD, "Chest Data       ");
    MYSQL_ROW row;
    MYSQL_RES *result = DB->QStore("SELECT chestid,reward,rewardtype,prob FROM chest_data order by id");
    if(result==NULL) return false;
    while( row = mysql_fetch_row(result) )
    {
        CChest* newchest = new (nothrow) CChest;
        if(newchest==NULL)
        {
            Log( MSG_ERROR, "Error allocing memory" );
            continue;
        }
        newchest->chestid = atoi(row[0]);
 
        UINT value = 0;
        bool First = true;
        // items
        while((value=atoi(strtok(First?row[1]:NULL, "|")))!=0)
        {
            First = false;
            CReward* Reward = new (nothrow) CReward;
            if(Reward==NULL)
            {
                Log(MSG_WARNING, "\nError allocing memory [chestdata]" );
                continue;
            }
            Reward->id = value;
            newchest->Rewards.push_back( Reward );
        }
 
        value = 0;
        // Reward Type
        for(UINT j=0;j<newchest->Rewards.size();j++)
        {
            value = atoi(strtok((j==0?row[2]:NULL), "|"));
            if(value==0)
            {
                newchest->Rewards.erase(newchest->Rewards.begin() + j);
                Log(MSG_WARNING, "reward type not set! chestid: %i - reward id: %i - reward deleted", newchest->chestid, newchest->Rewards.at(j)->id );
            }
            else
            {
                newchest->Rewards.at(j)->type = value;
            }
        }
 
        newchest->probmax = 0;
        value = 0;
        // probability
        for(UINT j=0;j<newchest->Rewards.size();j++)
        {
            value = atoi(strtok((j==0?row[3]:NULL), "|"));
            if(value==0)
            {
                newchest->Rewards.at(j)->prob = 1;
                Log(MSG_WARNING, "Probability is not complete, chestid: %i - probability set to 1", newchest->chestid );
            }
            else
            {
                newchest->Rewards.at(j)->prob = value;
            }
            newchest->probmax += newchest->Rewards.at(j)->prob;
        }
 
        ChestList.push_back( newchest );
    }
    DB->QFree( );
}

bool CWorldServer::LoadMonsterSpawn( )
{
	Log( MSG_LOAD, "SpawnZones data      " );    
	MYSQL_ROW row;
	MYSQL_RES *result = DB->QStore("SELECT id,map,montype,min,max,respawntime,points FROM list_spawnareas");
	if(result==NULL) return false;
	while(row = mysql_fetch_row(result))
    {
        bool flag = true;
		char* tmp;
		CSpawnArea* thisspawn = new (nothrow) CSpawnArea;
        if(thisspawn==NULL)
        {
            Log(MSG_ERROR, "Error allocing memory" );
            DB->QFree( );
            return false;
        }
		thisspawn->id=atoi(row[0]);
		thisspawn->map=atoi(row[1]);
		thisspawn->montype=atoi(row[2]);
		thisspawn->min=atoi(row[3]);
		thisspawn->max=atoi(row[4]);
		thisspawn->respawntime=atoi(row[5]);
		thisspawn->amon = 0;
		char* points;
		points = row[6];
		thisspawn->pcount = atoi(strtok( points , ",|"));
		thisspawn->points = new (nothrow) fPoint[thisspawn->pcount];
		if(thisspawn->points==NULL)
        {
            Log(MSG_ERROR, "Error allocing memory       " );
            delete thisspawn;
            DB->QFree( );
            return false;
        }
		thisspawn->lastRespawnTime = clock();
		for(int i=0; i<thisspawn->pcount; i++) 
        {
			if ((tmp = strtok(NULL, ",|"))==NULL) 
            {
				Log( MSG_ERROR, "Spawn area %i have invalid points",thisspawn->id );
				flag = false;				
				break;
			}
			float x=(float)atof(tmp);
			if ((tmp = strtok(NULL, ",|"))==NULL) 
            {
				Log( MSG_ERROR, "Spawn area %i have invalid points",thisspawn->id );
				flag = false;
				break;
			}
			float y=(float)atof(tmp);
			thisspawn->points[i].x = x;
			thisspawn->points[i].y = y;
		}
        if(flag)
        {
            thisspawn->thisnpc = GetNPCDataByID( thisspawn->montype );
            if(thisspawn->thisnpc==NULL)
            {
                Log( MSG_WARNING, "Invalid montype - Spawn %i will not be added", thisspawn->id );
                delete thisspawn;
                continue;
            }
    		MapList.Index[thisspawn->map]->MonsterSpawnList.push_back( thisspawn );
        }
	}
	DB->QFree( );
	return true;            
}

bool CWorldServer::LoadNPCs( )
{
	Log( MSG_LOAD, "NPC spawn        " );     
	MYSQL_ROW row;
	MYSQL_RES *result = DB->QStore("SELECT type,map,dir,x,y FROM list_npcs");
	if(result==NULL) return false;
	while(row = mysql_fetch_row(result))
    {
		CNPC* thisnpc = new (nothrow) CNPC;
        if(thisnpc==NULL)
        {
            Log(MSG_ERROR, "Error allocing memory" );
            DB->QFree( ); 
            return false;
        }
		thisnpc->clientid = GetNewClientID();
		thisnpc->npctype = atoi(row[0]);
		thisnpc->posMap = atoi(row[1]);
		thisnpc->dir = (float)atof(row[2]);
		thisnpc->pos.x = (float)atof(row[3]);
		thisnpc->pos.y = (float)atof(row[4]);
		thisnpc->thisnpc = GetNPCDataByID( thisnpc->npctype );
		if( thisnpc->thisnpc == NULL)
		{
            delete thisnpc;
            continue;
        }
		MapList.Index[thisnpc->posMap]->AddNPC( thisnpc );
	}
	DB->QFree( );  
	return true;	
}

bool CWorldServer::LoadDropsData( )
{
	Log( MSG_LOAD, "Drops Data       " );     
    FILE* fh = NULL;
    fh = fopen("data/drops_data.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/drops_data.csv" );
        return false;
    }    
    char line[500];
    fgets( line, 500, fh );// this is the column name    
    while(!feof(fh))
    {        
        memset( &line, '\0', 500 );
        fgets( line, 500, fh );
        CMDrops* newdrop = new (nothrow) CMDrops;
        if(newdrop==NULL)
        {
            fclose(fh);
            continue;
        }
        newdrop->id = GetUIntValue(",", &line);
        char* items = GetStrValue(",");
        char* prob = GetStrValue(",");
        newdrop->level_min = GetUIntValue(",");
        newdrop->level_max = GetUIntValue(",");
        newdrop->level_boss = GetUIntValue(",");
        UINT value = 0;    
        bool First = true;             
        // items
        while((value=GetUIntValue("|", First?items:NULL))!=0)
        {
            First = false;
            CDropInfo* DropInfo = new (nothrow) CDropInfo;
            if(DropInfo==NULL)
            {
                Log(MSG_WARNING, "\nError allocing memory [dropinfo]" );
                continue;            
            }                                       
            if(value<20000)
            {
                DropInfo->type = value/1000;
                DropInfo->item = value%1000;                                     
            }
            else
            {
                DropInfo->type = value/1000000;
                DropInfo->item = value%1000000;                                                      
            }       
            newdrop->Drops.push_back( DropInfo );                                      
        }   
        newdrop->probmax = 0;   
        value = 0;
        // probability
        for(UINT j=0;j<newdrop->Drops.size();j++)
        {
            value = GetUIntValue("|",(j==0?prob:NULL));
            if(value==0)
            {
                newdrop->Drops.at(j)->prob = 1;                
                if(newdrop->Drops.at(j)->type<10 || newdrop->Drops.at(j)->type==14)
                    newdrop->Drops.at(j)->prob *= Config.DROP_RATE;             
                Log(MSG_WARNING, "Probability is not complete, dropid: %i - temporal probability will be 1 * rate", newdrop->id );
            }
            else
            {
                newdrop->Drops.at(j)->prob = value;
                if(newdrop->Drops.at(j)->type<10 || newdrop->Drops.at(j)->type==14)
                    newdrop->Drops.at(j)->prob *= Config.DROP_RATE;                
            }
            newdrop->probmax += newdrop->Drops.at(j)->prob;
        }    
        // sort time
        for(UINT j=0;j<newdrop->Drops.size();j++)
        {
            for(UINT k=j;k<newdrop->Drops.size();k++)
            {
                if(newdrop->Drops.at(j)>newdrop->Drops.at(k))
                {
                    CDropInfo* DropInfo = newdrop->Drops.at(j);
                    newdrop->Drops.at(j) = newdrop->Drops.at(k);
                    newdrop->Drops.at(k) = DropInfo;
                }                                
            }
        }     
        MDropList.push_back( newdrop );
	}
	fclose(fh); 
	return true;	
}

bool CWorldServer::LoadMonsters( )
{
	Log( MSG_LOAD, "Monsters Spawn       " );     
	// Do our monster spawnin
    for(UINT i=0;i<MapList.Map.size();i++)
    {
        CMap* thismap = MapList.Map.at(i);
        for(UINT j=0;j<thismap->MonsterSpawnList.size();j++)
        {
            CSpawnArea* thisspawn = thismap->MonsterSpawnList.at(j);
    		thisspawn->mapdrop = GetDropData( thisspawn->map );
            thisspawn->mobdrop = GetDropData( thisspawn->thisnpc->dropid );                                     
            for(UINT k=0;k<thisspawn->max;k++)
            {
                fPoint position = RandInPoly( thisspawn->points, thisspawn->pcount );
                thismap->AddMonster(  thisspawn->montype, position, 0, thisspawn->mobdrop, thisspawn->mapdrop, thisspawn->id );
            }
        }
    }	 
	return true;     
}

bool CWorldServer::LoadUpgrade( )
{
	Log( MSG_LOAD, "Refine Data      " );        
    FILE* fh = NULL;
    fh = fopen("data/refine_data.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/refine_data.csv" );
        return false;
    }    
    char line[50];
    fgets( line, 50, fh );// this is the column name    
    while(!feof(fh))
    {        
        memset( &line, '\0', 50 );
        fgets( line, 50, fh );
        upgrade[GetUIntValue(",",&line)] = GetUIntValue(","); 
    }
    fclose(fh);  
	return true;
}

bool CWorldServer::CleanConnectedList( )
{
    Log( MSG_LOAD, "Cleaning Connected Clients         " ); 
    DB->QExecute("UPDATE accounts set online=false");
    return true;
}

bool CWorldServer::LoadEquip( )
{
    Log( MSG_LOAD, "Equip Data         " );    
    FILE* fh = NULL;
    fh = fopen("data/equip_data.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/equip_data.csv" );
        return false;
    }    
    char line[500];
    fgets( line, 500, fh );// this is the column name    
    while(!feof(fh))
    {        
        memset( &line, '\0', 500 );
        fgets( line, 500, fh );
        CEquip* newequip = new (nothrow) CEquip;
        if(newequip==NULL)
        {
            Log(MSG_WARNING, "\nError allocing memory: equip" );
            fclose(fh);
            return false;
        }
        newequip->id = GetUIntValue(",", &line);
        newequip->equiptype = GetUIntValue(",");
        newequip->type = GetUIntValue(",");
        newequip->price = GetUIntValue(",");
        newequip->pricerate = GetUIntValue(",");
        newequip->weight = GetUIntValue(",");
        newequip->quality = GetUIntValue(",");
        newequip->level = GetUIntValue(",");
        newequip->material = GetUIntValue(",");
        char* occupation = GetStrValue(",");
        char* condition1 = GetStrValue(",");
        char* condition2 = GetStrValue(",");
        char* stat1 = GetStrValue(",");
        char* stat2 = GetStrValue(",");
        newequip->defense = GetUIntValue(",");
        newequip->magicresistence = GetUIntValue(",");
        newequip->attackdistance = GetUIntValue(",");
        if(newequip->equiptype==SHOE)
        {
            newequip->movespeed = newequip->attackdistance;
        }
        else
        {
            newequip->movespeed = 0;
        }
        newequip->attackpower = GetUIntValue(",");
        newequip->attackspeed =GetUIntValue(",");
        newequip->itemgrade = GetUIntValue(",");
        for(int i=0;i<3;i++)
            newequip->occupation[i] = GetUIntValue("|", i==0?occupation:NULL);
        for(int i=0;i<3;i++)
            newequip->condition1[i] = GetUIntValue("|", i==0?condition1:NULL);
        for(int i=0;i<3;i++)
            newequip->condition2[i] = GetUIntValue("|", i==0?condition2:NULL);
        for(int i=0;i<2;i++)
            newequip->stat1[i] = GetUIntValue("|", i==0?stat1:NULL);
        for(int i=0;i<2;i++)
            newequip->stat2[i] = GetUIntValue("|", i==0?stat2:NULL);
        EquipList[newequip->equiptype].Data.push_back( newequip );
        EquipList[newequip->equiptype].Index[newequip->id] = newequip; // Index to read more quickly the data
    }
    fclose(fh);
    return true;
}

bool CWorldServer::LoadJemItem( )
{
    Log( MSG_LOAD, "Jem Data         " );    
    FILE* fh = NULL;
    fh = fopen("data/jemitem_data.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/jemitem_data.csv" );
        return false;
    }    
    char line[500];
    fgets( line, 500, fh );// this is the column name    
    while(!feof(fh))
    {        
        memset( &line, '\0', 500 );
        fgets( line, 500, fh );
        CJemData* thisjem = new (nothrow) CJemData;
        if(thisjem==NULL)  
        {
            Log(MSG_WARNING, "\nError allocing memory: jemitem" );
            fclose(fh);
            continue;
        }
        thisjem->id = GetUIntValue(",", &line);
        thisjem->type = GetUIntValue(",");
        thisjem->price = GetUIntValue(",");
        thisjem->pricerate = GetUIntValue(",");
        thisjem->weight = GetUIntValue(",");
        thisjem->quality = GetUIntValue(",");
        thisjem->material = GetUIntValue(",");
        char* stat1 = GetStrValue(",");
        char* stat2 = GetStrValue(",");    
        for(int i=0;i<2;i++)
            thisjem->stat1[i] = GetUIntValue("|",i==0?stat1:NULL);
        for(int i=0;i<2;i++)
            thisjem->stat2[i] = GetUIntValue("|",i==0?stat2:NULL);
        JemList.Data.push_back( thisjem );
        JemList.Index[thisjem->id] = thisjem;
    }
    fclose(fh);
    return true;
}

bool CWorldServer::LoadNaturalItem( )
{
    Log( MSG_LOAD, "Natural Data         " );    
    FILE* fh = NULL;
    fh = fopen("data/natural_data.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/natural_data.csv" );
        return false;
    }    
    char line[500];
    fgets( line, 500, fh );// this is the column name    
    while(!feof(fh))
    {        
        memset( &line, '\0', 500 );
        fgets( line, 500, fh );
        CNaturalData* thisnatural = new (nothrow) CNaturalData;
        if(thisnatural==NULL)
        {
            Log(MSG_WARNING, "\nError allocing memory: natural" );
            fclose(fh);
            return false;
        }
        thisnatural->id = GetUIntValue(",", &line);
        thisnatural->type = GetUIntValue(",");
        thisnatural->price = GetUIntValue(",");
        thisnatural->pricerate = GetUIntValue(",");
        thisnatural->weight = GetUIntValue(",");
        thisnatural->quality = GetUIntValue(",");
        thisnatural->pricevalue = GetUIntValue(",");
        NaturalList.Data.push_back( thisnatural );
        NaturalList.Index[thisnatural->id] = thisnatural;
    }
    fclose(fh);   
    return true;
}

bool CWorldServer::LoadPatItem( )
{
    Log( MSG_LOAD, "Consumible Data         " );    
    FILE* fh = NULL;
    fh = fopen("data/pat_data.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/pat_data.csv" );
        return false;
    }    
    char line[500];
    fgets( line, 500, fh );// this is the column name    
    while(!feof(fh))
    {        
        memset( &line, '\0', 500 );
        fgets( line, 500, fh );
        CPatData* newpat = new (nothrow) CPatData;
        if(newpat==NULL)
        {
            Log(MSG_WARNING, "\nError allocing memory: pat" );
            fclose(fh);
            return false;
        }
        newpat->id = GetUIntValue(",", &line);
        newpat->type = GetUIntValue(",");
        newpat->price = GetUIntValue(",");
        newpat->pricerate = GetUIntValue(",");
        newpat->weight = GetUIntValue(",");
        newpat->quality = GetUIntValue(",");
        newpat->material = GetUIntValue(",");
        newpat->partversion = GetUIntValue(",");
        newpat->maxfuel = GetUIntValue(",");
        newpat->fuelcons = GetUIntValue(",");
        newpat->speed = GetUIntValue(",");
        newpat->attackdistance = GetUIntValue(",");
        newpat->attackpower = GetUIntValue(",");
        newpat->attackspeed = GetUIntValue(",");
        PatList.Data.push_back( newpat );
        PatList.Index[newpat->id] = newpat;
    }               
    fclose(fh);
    return true;    
}

bool CWorldServer::LoadProductItem( )
{
    Log( MSG_LOAD, "Product Data         " );        
    FILE* fh = NULL;
    fh = fopen("data/product_data.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/product_data.csv" );
        return false;
    }    
    char line[500];
    fgets( line, 500, fh );// this is the column name    
    while(!feof(fh))
    {        
        memset( &line, '\0', 500 );
        fgets( line, 500, fh );
        CProductData* newproduct = new (nothrow) CProductData;
        if(newproduct==NULL)
        {
            Log(MSG_WARNING, "\nError allocing memory: product" );
            fclose(fh);
            return false;
        }
        newproduct->id = GetUIntValue(",", &line);
        char* items = GetStrValue(",");
        char* amount = GetStrValue(",");
        for(int i=0;i<50;i++)
            newproduct->item[i] = GetUIntValue("|",i==0?items:NULL);
        for(int i=0;i<50;i++)
            newproduct->amount[i] = GetUIntValue("|",i==0?amount:NULL);
        ProductList.Data.push_back( newproduct );
        ProductList.Index[newproduct->id] = newproduct;     
    }
    fclose(fh);
    return true;
}

bool CWorldServer::LoadSellData( )
{
    Log( MSG_LOAD, "Sell Data         " );        
    FILE* fh = NULL;
    fh = fopen("data/sell_data.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/sell_data.csv" );
        return false;
    }    
    char line[500];
    fgets( line, 500, fh );// this is the column name    
    while(!feof(fh))
    {
        memset( &line, '\0', 500 );
        fgets( line, 500, fh );        
        CCSellData* newsell = new (nothrow) CCSellData;
        if(newsell==NULL)
        {
            Log(MSG_WARNING, "\nError Allocing memory: sell" );
            fclose(fh);
            return false;
        }
        newsell->id = GetUIntValue(",", &line);
        char *items = GetStrValue(",");
        for(int i=0;i<48;i++)
            newsell->item[i] = GetUIntValue("|",i==0?items:NULL);
        SellList.Data.push_back( newsell );
        SellList.Index[newsell->id] = newsell;     
    }    
    fclose(fh);
    return true;    
}

bool CWorldServer::LoadConsItem( )
{
    Log( MSG_LOAD, "Consumible Data         " );    
    FILE* fh = NULL;
    fh = fopen("data/useitem_data.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/useitem_data.csv" );
        return false;
    }    
    char line[500];
    fgets( line, 500, fh );// this is the column name    
    while(!feof(fh))
    {        
        memset( &line, '\0', 500 );
        fgets( line, 500, fh );
        CUseData* newuse = new (nothrow) CUseData;
        if(newuse==NULL)
        {
            Log(MSG_WARNING, "\nError allocing memory: use" );
            fclose(fh);
            return false;
        }
        newuse->id = GetUIntValue(",", &line);
        newuse->restriction = GetUIntValue(",");
        newuse->type = GetUIntValue(",");
        newuse->price = GetUIntValue(",");
        newuse->pricerate = GetUIntValue(",");
        newuse->weight = GetUIntValue(",");
        newuse->quality = GetUIntValue(",");
        newuse->pricevalue = GetUIntValue(",");        
        char* usecondition = GetStrValue(",");
        char* useeffect = GetStrValue(",");  
        for(int i=0;i<2;i++)
            newuse->usecondition[i] = GetUIntValue("|",i==0?usecondition:NULL);
        for(int i=0;i<2;i++)
            newuse->useeffect[i] = GetUIntValue("|",i==0?useeffect:NULL);
        UseList.Data.push_back( newuse );
        UseList.Index[newuse->id] = newuse;           
    }        
    fclose(fh);
    return true;    
}

bool CWorldServer::LoadZoneData( )
{
    Log( MSG_LOAD, "Zone Data                   " );
    FILE* fh = fopen( "data/zone_data.csv", "r" );
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/zone_data.csv" );
        return false;
    }
    char line[500];
    fgets( line, 500, fh );// this is the column name
    while(!feof(fh))
    {        
        memset( &line, '\0', 500 );
        fgets( line, 500, fh );
        CMap* newzone = new (nothrow) CMap( );
        if(newzone==NULL)
        {
            Log(MSG_WARNING, "\nError allocing memory: CMap" );
            fclose(fh);
            return false;
        }
        newzone->id = GetUIntValue(",", &line);
        newzone->dayperiod = GetUIntValue(",");     
        newzone->morningtime = GetUIntValue(",");
        newzone->daytime = GetUIntValue(",");
        newzone->eveningtime = GetUIntValue(",");
        newzone->nighttime = GetUIntValue(",");
        newzone->allowpvp = GetUIntValue(",");
        newzone->allowpat = GetUIntValue(",")==0?true:false;
        newzone->ghost = GetUIntValue(","); 
        
        //LMA begin
        //CF Mode
        //20070621-211100
        //map is cf (for jelly bean)
       newzone->is_cf=GetUIntValue(",");
       newzone->id_temp_mon = 0;
       newzone->id_def_mon = 0;
       newzone->min_lvl = 0;
       newzone->mon_lvl = 0;
       newzone->mon_exp = 0;
       newzone->percent=0;
       
       //mode 1: one monster temporarily, then the "real" one :)
        if (newzone->is_cf==1)
        {
           Log( MSG_INFO, "Map %u is CF mode 1 !",newzone->id);
           newzone->min_lvl = GetUIntValue(",");
           newzone->id_temp_mon = GetUIntValue(",");
           newzone->id_def_mon = GetUIntValue(",");
           newzone->mon_lvl = GetUIntValue(",");
           newzone->mon_exp = GetUIntValue(",");
           newzone->percent = GetUIntValue(",");
        }
        else
        {
            newzone->is_cf=0;
        }
        //LMA end
        
        newzone->MapTime = 0;
        newzone->LastUpdate = clock( );
        newzone->CurrentTime = 0;
        newzone->MonsterSpawnList.clear();
        MapList.Map.push_back(newzone);
        MapList.Index[newzone->id] = newzone;  
    }         
    fclose(fh);
    return true;    
}

bool CWorldServer::LoadItemStats( )
{
    Log( MSG_LOAD, "Item Stats         " );    
    FILE* fh = NULL;
    fh = fopen("data/stat.csv", "r");
    if(fh==NULL)
    {
        Log(MSG_ERROR, "\nError loading file data/stat.csv" );
        return false;
    }    
    char line[500];
    fgets( line, 500, fh );// this is the column name    
    while(!feof(fh))
    {        
        memset( &line, '\0', 500 );
        fgets( line, 500, fh );    
        UINT id = GetUIntValue(",", &line );
        if(id>499) continue;
        StatsList[id].stat[0] = GetUIntValue(",");
        StatsList[id].value[0] = GetUIntValue(",");        
        StatsList[id].stat[1] = GetUIntValue(",");
        StatsList[id].value[1] = GetUIntValue(",");                
    }
    fclose(fh);
    return true;
}
