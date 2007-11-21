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

// from Paul_T
bool CWorldServer::SendPM( CPlayer* thisclient, char msg[200] )
{
           BEGINPACKET( pak, 0x0784 );
           ADDSTRING( pak, "Server" );
           ADDBYTE( pak, 0 );
           ADDSTRING( pak, msg );
           ADDBYTE( pak, 0 );
           thisclient->client->SendPacket(&pak);
           return true;    
}

// from Paul_T
bool CWorldServer::SendGlobalMSG( CPlayer* thisclient, char msg[200] )
{
           BEGINPACKET( pak, 0x0784 );
           ADDSTRING( pak, thisclient->CharInfo->charname );
           ADDBYTE( pak, 0 );
           ADDSTRING( pak, msg );
           ADDBYTE( pak, 0 );
           SendToAll(&pak);
           return true;    
}

// Search Quest by ID
CQuest* CWorldServer::GetQuestByID( unsigned long int id )
{
	for(unsigned j=0; j<QuestList.size(); j++) 
    {
		CQuest* thisquest = (CQuest*)QuestList.at( j );
		if ( thisquest->id == id ) 
            return thisquest;
	}
	return 0;
}

// Search Quest by QuestID
CQuest* CWorldServer::GetQuestByQuestID( unsigned long int questid )
{
	for(unsigned j=0; j<QuestList.size(); j++) 
    {
		CQuest* thisquest = (CQuest*)QuestList.at( j );
		if ( thisquest->questid == questid ) 
            return thisquest;
	}
	return 0;
}

// Search Quest by FinalID
CQuest* CWorldServer::GetQuestByFinalID(  unsigned long int questid )
{
	for(unsigned j=0; j<QuestList.size(); j++) 
    {
		CQuest* thisquest = (CQuest*)QuestList.at( j );
		if ( thisquest->finalid == questid ) 
            return thisquest;
	}
	return 0;
}

bool CWorldServer::DoSkillScript( CCharacter* character, CSkills* thisskill )
{
    if(thisskill==0)
        return false;
    if(thisskill->script==0)
        return false;
    switch(thisskill->script)
    {
        case 1:
        {
            if(thisskill->svalue1==0)
                return false;                
            fPoint position = RandInCircle( character->Position->current, 5 );
            CMap* map = MapList.Index[character->Position->Map];
            CMonster* thismonster=map->AddMonster( thisskill->svalue1, position, character->clientid );
            if (thismonster!=NULL)
            {
               thismonster->skillid=thisskill->id;
               if (!thismonster->IsBonfire())
                  break;
               
               //LMA: Special for bonfire (and others) cases.
               switch (thisskill->value1[0])
               {
                      case 3:
                           {
                             thismonster->bonushp=1;
                             thismonster->bonusmp=1;
                           }
                           break;
                      case 2:
                           {
                             thismonster->bonushp=1;
                           }
                           break;
                      case 1:
                           {
                             thismonster->bonusmp=1;
                           }
                           break;
                      default:
                           {
                             //NA
                           }
                           break;
               }
                
                if (thismonster->bonushp==0&&thismonster->bonusmp==0)
                   break;
                thismonster->minvalue=thisskill->value1[1];
                thismonster->maxvalue=thisskill->value1[2];
                thismonster->range=thisskill->range;
            }
            
        }
        break;
    }
    return true;
}

// Build Drop
CDrop* CWorldServer::GetDrop( CMonster* thismon )
{
    CDrop* newdrop = new (nothrow) CDrop; 
    if(newdrop==NULL)
    {
        Log(MSG_WARNING, "Error allocing memory [getdrop]" );
        return NULL;
    }
    newdrop->clientid = GetNewClientID( );
    newdrop->posMap = thismon->Position->Map;
    newdrop->pos = RandInCircle( thismon->Position->current, 3 );
    newdrop->droptime = time(NULL);
    newdrop->owner = thismon->MonsterDrop->firsthit;
    newdrop->thisparty = thismon->thisparty;
    ClearItem(newdrop->item);
    int randv = RandNumber( 1, 100);
    if(thismon->MonsterDrop->mapdrop->level_max<thismon->MonsterDrop->firstlevel) randv = 100;
    if(randv<=30)//30% zuly [zulies will count as mapdrop]
    {
        if(thismon->MonsterDrop->mapdrop->level_max>=thismon->MonsterDrop->firstlevel)
        {
            newdrop->type = 1; //Drop Zuly
            newdrop->amount = thismon->thisnpc->level*5*Config.ZULY_RATE + RandNumber( 1, 10 );
            return  newdrop;
        }
        delete newdrop;
        return NULL;
    }
    CMDrops* thisdrops;
    newdrop->type = 2; //drop item drop
    switch( Config.DROP_TYPE )
    {
        case 0://only map
            thisdrops = thismon->MonsterDrop->mobdrop;
            if(thisdrops->level_max<thismon->MonsterDrop->firstlevel)
            {
                delete newdrop;
                return NULL;
            }            
        break;
        case 1://mob only
            thisdrops = thismon->MonsterDrop->mapdrop;
            if(thismon->thisnpc->level-thismon->MonsterDrop->firstlevel<-14)
            {
                delete newdrop;
                return NULL;
            }
        break;
        default://both
            randv = RandNumber(1,100);
            if(thismon->MonsterDrop->mapdrop!=NULL)
                if(thismon->MonsterDrop->mapdrop->level_max<thismon->MonsterDrop->firstlevel)
                    randv = 100;
            if(randv>60)//select wich drop will use (map or mob) //40 - 60%
            {
                thisdrops = thismon->MonsterDrop->mobdrop;
                if((int)(thismon->thisnpc->level-thismon->MonsterDrop->firstlevel) < -14)
                {
                    delete newdrop;
                    return NULL;
                }
            }
            else
            {
                thisdrops = thismon->MonsterDrop->mapdrop;
                if(thisdrops->level_max<thismon->MonsterDrop->firstlevel)
                {
                    delete newdrop;
                    return NULL;
                }
            }
        break;
    }
    if(thisdrops==NULL)
    {
        thisdrops = thismon->MonsterDrop->mobdrop;
        if(thisdrops==NULL)
        {
            thisdrops = thismon->MonsterDrop->mapdrop;
            if(thisdrops==NULL)
            {
                newdrop->type = 1; //Drop Zuly
                newdrop->amount = thismon->thisnpc->level*5*Config.ZULY_RATE - RandNumber( 1, 20 );
                return  newdrop;
            }
        }
    }
    randv = 0;
    randv = RandNumber( 1, thisdrops->probmax );
    DWORD prob = 1;
    for(UINT i=0;i<thisdrops->Drops.size();i++)
    {
        CDropInfo* dropinfo = thisdrops->Drops.at( i );
        prob += dropinfo->prob;
        if(randv<=prob)
        {
            newdrop->item.itemtype = dropinfo->type;
            newdrop->item.itemnum = dropinfo->item;
            break;
        }
    }
    if(newdrop->item.itemtype==0)
    {
        Log(MSG_WARNING, "Drop Probability Highter: %i", randv );
        delete newdrop;
        return NULL;
    }
    if(newdrop->item.itemtype>1 && newdrop->item.itemtype<10 && newdrop->item.itemtype!=JEWEL)
    {
        // Refine
        randv = RandNumber( 1, 100 );
        if( randv < 10 )        //10%
            newdrop->item.refine = 48;
        else if( randv < 25 )   //15%
            newdrop->item.refine = 32;
        else if( randv < 50 )   // 25%
            newdrop->item.refine = 16;
        else //50%
            newdrop->item.refine = 0;
        newdrop->item.lifespan = RandNumber( 30, 100 );
        newdrop->item.durability = RandNumber( 35, 70 );
        if( newdrop->item.itemtype==WEAPON || newdrop->item.itemtype==SUBWEAPON )
        {
            //socketed
            randv = RandNumber( 1, 100 );
            if( randv < 30 ) newdrop->item.socketed = true; // 30%
            else newdrop->item.socketed = false;            // 70%
        }
        else
        {
            newdrop->item.socketed = false;
        }
        randv = RandNumber( 1, 100 );
        if( randv < 30 )        // 30%
            newdrop->item.stats = rand()%300;
        newdrop->item.appraised = newdrop->item.stats==0?true:false;
    }
    else
    {
         newdrop->item.lifespan = 100;
         newdrop->item.durability = 40;    
         newdrop->item.socketed = false;     
         newdrop->item.stats = 0;
    }
    newdrop->item.count = 1;
    if( newdrop->item.itemtype == 10 || newdrop->item.itemtype == 12 )
    {
        newdrop->item.count = RandNumber( 1, 3 );
    }
    newdrop->item.gem = 0;
    return newdrop;
}

UINT CWorldServer::GetColorExp( UINT playerlevel,UINT moblevel, UINT exp )
{
    int dif = moblevel - playerlevel;        

    if(dif>15)//purple
        exp = (UINT)floor(exp*1.5);
    else if(dif>10)
        exp = (UINT)floor(exp*1.2);
    else if(dif>4)
        exp = (UINT)floor(exp*1.1);
    else if(dif>-4)
        exp = (UINT)floor(exp*1);
    else if(dif>-9)
        exp = (UINT)floor(exp*0.7);
    else if(dif>-15)
        exp = (UINT)floor(exp*0.1);        
    else if(dif>-25)
        exp = (UINT)floor(exp*0.005);    
    else
        exp = 0;
    return exp;
}
bool CWorldServer::SendSysMsg( CPlayer* thisclient, string message )
{
	BEGINPACKET( pak, 0x7df );
	ADDBYTE    ( pak, 0xf1 );
	ADDSTRING  ( pak, message.c_str( ));
	ADDBYTE    ( pak, 0x00 );
	thisclient->client->SendPacket( &pak );    
    return true;
}


// teleport to specificated map and position
bool CWorldServer::TeleportTo ( CPlayer* thisclient, int map, fPoint position )
{
    MapList.Index[map]->TeleportPlayer( thisclient, position, false );        
    return true;
}


// Learn Skill
bool CWorldServer::LearnSkill( CPlayer* thisclient, UINT skill )
{                 
/*
0 - ya aprendido 
1 - aprendio 
2 - no job
3 - falta otro skill 
4 - no status points (lvl)
5 - no se puede subir de lvl el skill
6 - numero incorrecto de skill
7 - no tienes suficientes sp
*/    
        int b=1;         
        CSkills* thisskill = GetSkillByID( skill );
        if( thisskill==NULL )
            return false;
        if( thisskill->clevel>thisclient->Stats->Level )
        {
            b=4;      
        }
        else if( thisclient->CharInfo->SkillPoints<thisskill->sp )
        {
                b=7;          
        }
        if(b==1)
        {
            thisclient->cskills[thisclient->p_skills].id = skill;
            thisclient->cskills[thisclient->p_skills].level=1;
            thisclient->cskills[thisclient->p_skills].thisskill = thisskill;            
            thisclient->CharInfo->SkillPoints -= 1;        
            thisclient->p_skills++;
        }
        BEGINPACKET( pak, 0x7b0 );                            
        ADDBYTE    ( pak, b);
        ADDWORD    ( pak, thisclient->p_skills-1);
        ADDWORD    ( pak, skill);
        ADDWORD    ( pak, thisclient->CharInfo->SkillPoints);
        thisclient->client->SendPacket( &pak);
        if(b==1)
        {
            thisclient->SetStats( );   
            return true;
        }
        else
        {
            return false;
        }
}

//add / remove a Fairy
void CWorldServer::DoFairyStuff(CPlayer* targetclient, int action)
{
    BEGINPACKET( pak, 0x7dc );
	ADDBYTE( pak, action );
	ADDWORD( pak, targetclient->clientid);
	SendToVisible( &pak, targetclient );
}

void CWorldServer::DoFairyFree( int fairy )
{
    GServer->FairyList.at(fairy)->assigned = false;
    GServer->FairyList.at(fairy)->ListIndex = 0;
    
    if(GServer->Config.FairyTestMode == 0)
         GServer->FairyList.at(fairy)->WaitTime = GServer->Config.FairyWait * (rand()% GServer->GetFairyRange(1)+ GServer->GetFairyRange(0));
    
    GServer->FairyList.at(fairy)->LastTime = clock();
}

void CWorldServer::RefreshFairy( )
{
        //fairy: The wait timer should be possibibly bigger when there is few people online. 
        if (Config.FairyMode == 1 && ClientList.size() > 1){   //if fairy mode on and someone online 
           for (int i=0; i<Config.FairyMax; i++)     // check all fairies
           {                           
                if ( FairyList.at(i)->LastTime <= ( clock() - (FairyList.at(i)->WaitTime*60*CLOCKS_PER_SEC) ) && !FairyList.at(i)->assigned )  // if fairy hour is now
	            { 
                    int value = rand()%(int)(ClientList.size()-1) + 1;  //choose random value in clientlist
		            CPlayer* targetclient = (CPlayer*) ClientList.at(value)->player;
		            
		            //LMA: Patch if fairy was attributed as the player is not actually yet IG...
		            //The Fairy is not an easy lady ^_^
		            if ((clock()-targetclient->firstlogin)>(60*CLOCKS_PER_SEC))
		            {
                        FairyList.at(i)->ListIndex = value;  
    			        FairyList.at(i)->LastTime = clock();
    			        FairyList.at(i)->assigned = true;
    			        targetclient->Fairy = true;
    			        targetclient->FairyListIndex = i;                 // FairyList index number of our actual fairy
    			        DoFairyStuff(targetclient, 1);           // spawn fairy to target
    			        targetclient->SetStats();                                                                 
                    }
			        
                }
			    if ( (FairyList.at(i)->LastTime + (Config.FairyStay*60*CLOCKS_PER_SEC)) <= clock() && FairyList.at(i)->assigned)  // id time for our fairy to go away
    			{   
                    CPlayer* oldclient  = (CPlayer*) ClientList.at(FairyList.at(i)->ListIndex)->player;
                    DoFairyFree(i);
                    oldclient->Fairy = false;
                    oldclient->FairyListIndex = 0;
                    DoFairyStuff(oldclient, 0);  // unspawn fairy
                    oldclient->SetStats();
                }
           }
        }
        // this close fairies after their time if GM de activate  fairy mode when some fairies are assigned.
        if (Config.FairyMode == 0 && ClientList.size() > 1){    // if serevr mode off and someone online
             for (int i=0; i<Config.FairyMax; i++)
             { 
                 if ( (FairyList.at(i)->LastTime + (Config.FairyStay*60*CLOCKS_PER_SEC)) <= clock() && FairyList.at(i)->assigned)
    			{   
                    CPlayer* oldclient  = (CPlayer*) ClientList.at(FairyList.at(i)->ListIndex)->player;
                    DoFairyFree(i);
                    oldclient->Fairy = 0;
                    oldclient->FairyListIndex = 0;
                    DoFairyStuff(oldclient, 0);
                    oldclient->SetStats();
                }
             }
         }      
}
