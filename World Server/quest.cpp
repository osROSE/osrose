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

bool CWorldServer::DoQuestScript( CPlayer* thisclient, CQuest* thisquest )
{
    switch( thisquest->script )
    {
        case 1: //Teleport
        {
            if(thisquest->value3>0)
            {
                thisclient->CharInfo->Zulies -= thisquest->value3;
               	BEGINPACKET( pak, 0x71e );
                ADDQWORD   ( pak, thisclient->CharInfo->Zulies );
                ADDBYTE    ( pak, 0x00 );
                thisclient->client->SendPacket( &pak );             
            }
            fPoint thispoint;
            thispoint.x = floor( thisquest->value2/10000 );
            thispoint.y = floor( thisquest->value2%10000 );
            TeleportTo ( thisclient, thisquest->value1, thispoint );       
            thisclient->DelQuest( thisquest->questid );
        }
        break;    
        case 2: // Change Job
        {
            if( thisclient->CharInfo->Job == 0 )
            {
                thisclient->CharInfo->Job = thisquest->value1;
                BEGINPACKET(pak, 0x721 );
                ADDWORD    (pak, 0x0004 );
                ADDWORD    (pak, thisclient->CharInfo->Job );
                ADDWORD    (pak, 0x0000 );
                thisclient->client->SendPacket(&pak);                                  
            }          
        }
        break;  
        case 20: // 2nd Change Job
        {   //Credits to tomiz
            if(thisquest->value3>0 && thisclient->CharInfo->Zulies>= thisquest->value3) // zuly need for second job
            {
                thisclient->CharInfo->Zulies-= thisquest->value3;
                BEGINPACKET( pak, 0x71e );
                ADDQWORD   ( pak, thisclient->CharInfo->Zulies );
                ADDBYTE    ( pak, 0x00 );
                thisclient->client->SendPacket( &pak );
            }
            unsigned short tmp = thisquest->value1-thisclient->CharInfo->Job;
            if( tmp==11 || tmp==10  )
            {
                thisclient->CharInfo->Job = thisquest->value1;
                BEGINPACKET(pak, 0x721 );
                ADDWORD    (pak, 0x0004 );
                ADDWORD    (pak, thisclient->CharInfo->Job );
                ADDWORD    (pak, 0x0000 );
                thisclient->client->SendPacket(&pak);
            }
            if(thisquest->value2>0) //which quest to delete
            {
                CQuest* athisquest = GetQuestByID( thisquest->value2 );
                QUESTS* myquest = thisclient->GetQuestByQuestID( athisquest->questid ); 
                if( myquest!=0 )
                {
                    Log(MSG_INFO,"deleting quest id %u",myquest->thisquest->id);
                    myquest->active = false;
                    thisclient->ActiveQuest--;
                }
                
            }
        }
        break;          
        case 3: //Give Stats/Skill points
        {
            switch( thisquest->value1 )
            {
                case 1: // Give Stats points
                    thisclient->CharInfo->StatPoints += thisquest->value2;                
                    BEGINPACKET( pak, 0x720 );
                    ADDWORD    ( pak, 0x0020 );
                    ADDDWORD   ( pak, thisquest->value2 );
                    thisclient->client->SendPacket( &pak );                
                break;
                case 2: // Give Skill points
                    thisclient->CharInfo->SkillPoints += thisquest->value2;
                break;
            }
        }
        break;
        case 4: //Restart Stats/Skills points
            switch( thisquest->value1 )
            {
                case 1: // Stats points [fixed by Geobot]
                    thisclient->CharInfo->StatPoints = 0;
                    for(UINT i=2;i<=thisclient->Stats->Level;i++)
                        thisclient->CharInfo->StatPoints += 10 + (i/2);
                    thisclient->Attr->Str = 15;
                    thisclient->Attr->Dex = 15;
                    thisclient->Attr->Int = 15;
                    thisclient->Attr->Con = 15;
                    thisclient->Attr->Cha = 10;
                    thisclient->Attr->Sen = 10;                    
                break;
                case 2: // Skill points

                break;
            }         
        break;
        case 5: //Do spawn a monster
             GServer->pakGMMon(thisclient, thisquest->value1, thisquest->value2 );
        break;
       case 10: //Delete a quest
             {
                if( thisquest->value1==0 )
                    return true;    
                CQuest* athisquest = GetQuestByID( thisquest->value1 );
                QUESTS* myquest = thisclient->GetQuestByQuestID( athisquest->questid ); 
                if( myquest!=0 )
                    myquest->active = false;               
            }
        break;
        case 30:
             {
                 //LMA begin
                 //20070621-211100
                 //quest modifications and ONLY for the children !!!
                 //value1=code of the "normal" kid
                 //value2=code of the "changed" kid
                 //value3=when to execute the script, 0 = end of quest, 1=for item quest, 2=beginning of the quest
                 if (thisquest->value1==0||thisquest->value2==0)
                 {
                    break;
                 }
                 
                 CMap* map = MapList.Index[thisclient->Position->Map];                 
                 UINT montype = (UINT) thisquest->value2;
                 fPoint position_cf = GServer->RandInCircle( thisclient->Position->current,10 );
                 CMonster* monster2=map->AddMonster( montype, position_cf, 0, NULL, NULL, 0, true );
                 monster2->StartAction( (CCharacter*)thisclient, NORMAL_ATTACK, 0 );
                 break;
            }
        break;
        case 31:
             {
                 //LMA BEGIN
                 //20070621-211100
                 //let's make a monster come at some events :)
                 //mostly for purified rakkies and infected pomics too.
                 if (thisquest->value1==0||thisquest->value2==0)
                 {
                    break;
                 }
                 
                 CMap* map = MapList.Index[thisclient->Position->Map];
                  int montype = (int) thisquest->value1;                 
                  BYTE monaction= (BYTE) thisquest->value2;
                 int nb_monsters=1;
                 if (thisquest->value3==119)
                 {
                    nb_monsters=10;
                 }
                 
                 for (int k=0;k<nb_monsters;k++)
                 {
                     Log(MSG_INFO,"a monster is summoned, %i, nb: %i / %i",montype,k,nb_monsters);
                     fPoint position_cf = GServer->RandInCircle( thisclient->Position->current,10 );
                     CMonster* monster2=map->AddMonster( montype, position_cf, 0, NULL, NULL, 0, true );
                     //the monster does something (see define, 9=nothing)...
                     monster2->StartAction( (CCharacter*)thisclient, monaction, 0 );
                 }
                 
             }
        break;
          case 32:
             {   
                 //LMA BEGIN
                 //learning a skill automatically, for cart skill mainly :)
                 //20070621-211100
                   if (thisquest->value1==0)
                 {
                    break;
                 }
                 
                 UINT skillID=UINT (thisquest->value1);
                 //Let's add the player an additionnal sp, just in case.
                 thisclient->CharInfo->SkillPoints++;
                 GServer->LearnSkill(thisclient,skillID);
             }   
        break;
          case 33:
             {   
                  //LMA BEGIN
                 //teleport, but this time at the end of the quest
                 //20070622-214100                
                if(thisquest->value3>0)
                {
                    thisclient->CharInfo->Zulies -= 0;
                   	BEGINPACKET( pak, 0x71e );
                    ADDQWORD   ( pak, thisclient->CharInfo->Zulies );
                    ADDBYTE    ( pak, 0x00 );
                    thisclient->client->SendPacket( &pak );             
                }
                fPoint thispoint;
                thispoint.x = floor( thisquest->value2/10000 );
                thispoint.y = floor( thisquest->value2%10000 );
                TeleportTo ( thisclient, thisquest->value1, thispoint );
             }   
        break;
        //LMA end
        case 666:
             {
              //Don't ever touch this one.
             }
             break;
    }    
    return true;
}
