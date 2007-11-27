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
#include "player.h"
#include "worldserver.h"


//LMA function for logging quest info (IG)
//void CPlayer::LogQuest( char buffer2[200] )
void CPlayer::LogQuest( char *Format, ...)
{
	va_list ap;	      // For arguments
    va_start( ap, Format );
    
    if (this->questdebug)
     {
         char buffer2[200];
         vsprintf ( buffer2, Format, ap );
         GServer->SendPM(this, buffer2); 
         Log(MSG_INFO,buffer2);
     }
     
	va_end  ( ap );
}

bool CPlayer::AddQuest( unsigned long int questid )
{   
    LogQuest("Adding questid %lu ?", questid );
    
    if( GetQuestByQuestID( questid ) !=0 )
    {
           CQuest* thisquest = GServer->GetQuestByQuestID( questid );        
           LogQuest("Quest already in list %lu, quest %u",questid,thisquest->id);
           return false;        
    }
    
    // Check if is Start Quest ID        
    CQuest* thisquest = GServer->GetQuestByQuestID( questid ); 
    if(thisquest!=0)
    { 
        if( thisquest->script == 1 ) //Teleport Bypass 
        {
             LogQuest("questid %lu is Quest nb %i, teleport quest",questid,thisquest->id);
             return GServer->DoQuestScript( this, thisquest );
        }
        
        if (thisquest->script==9000)
        {
           //LMA: Used for Plastic surgery.
           if(PlasticSurgeon(thisquest))
           {
              //Database update.
              GServer->DB->QExecute("UPDATE characters SET sex=%i,hairStyle=%i,face=%i WHERE id=%i",CharInfo->Sex,CharInfo->Hair,CharInfo->Face,CharInfo->charid);
           }
           LogQuest("questid %lu is Quest nb %i, Plastic Surgeon Quest",questid,thisquest->id);
           return true;
        }
        
        //LMA begin
        LogQuest("questid %lu is Quest nb %i, NEW quest",questid,thisquest->id);
        //LMA end
        
        //LMA: main quest
        if(thisquest->id>=203&&thisquest->id<=205)
        {
            CQuest* oldquest = GServer->GetQuestByID( 202 );
            if( oldquest!=NULL )
            {
                QUESTS* myoldquest = GetQuestByQuestID( oldquest->questid ); 
                if( myoldquest!=NULL )
                {
                    Log(MSG_INFO,"deleting old quest id %u",202);
                    myoldquest->active = false;
                    ActiveQuest--;
                    SaveQuest(myoldquest);
                }
            }           
            
        }
        
        if( ActiveQuest >= 10 ) //Can't Take more quest
            return false;
            
        //LMA BEGIN
        //Clan Wars
        //we pay :)
        //and we check so that the other guy is happy...
        if (thisquest->id==2813)
        {
            if (CharInfo->Zulies>=10000000)
            {
                CharInfo->Zulies-=10000000;
                BEGINPACKET( pak, 0x71e );
                ADDQWORD   ( pak, CharInfo->Zulies );
                ADDBYTE    ( pak, 0x00 );
                client->SendPacket( &pak ); 
            }
            else
            {
                LogQuest("We got a hacker here :)");
                return false;
            }
        }
        
        QUESTS* myquest = new QUESTS;
        assert(myquest);
        myquest->questid = thisquest->questid;
        myquest->thisquest = thisquest;        
        myquest->active = true;
        for(int i=0;i<5;i++)
            myquest->items[i] = 0;  
            
        //LMA BEGIN
        //rev.70 org code (modified)
        for(int i=0;i<5;i++) // Look for start items
        {
            if( myquest->thisquest->startItems[i] != 0)
            {
                //myquest->items[i]++;
                myquest->items[i]=myquest->thisquest->startItems[i];
            }        
        } 
        
        MyQuest.push_back( myquest );
        ActiveQuest++;
        
         //LMA begin
         //20070621-211100
        //sometimes you want a script at the beginning of the quest
        if (thisquest->script>0&&thisquest->value3==2)
        {
            LogQuest("Time for script for new quest %lu !!",questid);
            GServer->DoQuestScript( this, thisquest );              
        }
        //LMA end
        
        //Patch for First Jobs
        //We got to disable Identification.
        if (thisquest->id==851||thisquest->id==901||thisquest->id==951||thisquest->id==1001)
        {
            CQuest* questtemp = GServer->GetQuestByID(801);
            QUESTS* myquesttemp = 0;
            
            if(questtemp!=NULL)
            {
                myquesttemp = GetQuestByQuestID( questtemp->questid);
                if (myquesttemp!=NULL)
                {
                    LogQuest("Patch for Junon ID.");
                    myquesttemp->active=false;
                    SaveQuest(myquesttemp);
                    ActiveQuest--;            
                }
                
            }

         }
        //LMA END
        
    }      
    
    // Check if is Finish Quest ID
    CQuest* finalquest = GServer->GetQuestByFinalID( questid );    
    QUESTS* myquest = 0;
    bool flag = false;
    if( finalquest != 0) 
    {                       
         //LMA begin
         LogQuest("Final Quest nb %i",finalquest->id);                    
         //LMA end
                         
        //Search if this user have the quest
        myquest = GetQuestByQuestID( finalquest->questid ); 
        if( myquest != 0 )
        {
            flag = true;            
            //LMA
            //20070622, 153000
            //patch for lost children
            if(myquest->thisquest->id>=9001&&myquest->thisquest->id<=9003)
            {
                CQuest* thisquestlc = GServer->GetQuestByID(215); 
                if(thisquestlc!=0)
                {                   
                  if( ActiveQuest < 10 )
                  {
                        QUESTS* myquestlc = new QUESTS;
                        assert(myquestlc);
                        myquestlc->questid = thisquestlc->questid;
                        myquestlc->thisquest = thisquestlc;        
                        myquestlc->active = true;
                        for(int i=0;i<5;i++)
                            myquestlc->items[i] = 0;  
                            myquestlc->items[0] = 1;
                        MyQuest.push_back( myquestlc );  
                        ActiveQuest++;
                        SaveQuest(myquestlc);
                        LogQuest("Special Activation for Lost children quest...");
                    }
                 }
            }
            //LMA END
        }
        
    }    
    else
    {
        // Search the quest who have this item
        myquest = GetQuestByItemID( questid );
        if( myquest!=0 )
        {
            //LMA begin
            LogQuest("questid %lu is Quest nb %i, item quest",questid,myquest->thisquest->id);
            //LMA end
            

            //LMA begin
            //20070621-211100
            //a peculiar item quest can launch the script
            //the script will be launched (if any) according to the place of the questitemid in the database in itemid field (item1|item2).
            //if in 0 position (so item1), the value 3 must be set to 3, if you want the script to be triggered when receiving item2, then 
            //as it is in offset 1, put 4 (always offset +3) 
            int itemquest_script=0;
            //LMA end
            
            for(int i=0;i<5;i++) // Search the Item Quest
            {
                if( myquest->thisquest->itemid[i] == questid )
                {
                    //LMA BEGIN
                    //20070621-211100--mod
                    //2do:compatibility with rev.70
                    //we calculate according to quest...
                    itemquest_script=i+3;
                    LogQuest("Adding itemquest %lu to quest, script level=%i, v3=%u",questid,itemquest_script,myquest->thisquest->value3);
                    
                    switch (myquest->thisquest->value3)
                    {
                           case 100:
                                {
                                    //useful for spero quest (2000)
                                    //Same for pomics quest (224)
                                    //11 monsters killed=1 page...
                                    myquest->items[i]++;
                                    if (myquest->items[i]==11)
                                    {
                                        myquest->items[2]++;
                                    }
                                    
                                }
                                break;
                           case 101:
                                  {
                                    //For pomics quest (224)                                    
                                    myquest->items[i]++;
                                    if (myquest->items[i]==11)
                                    {
                                         //we deactivate 224 and activate 225
                                         LogQuest("You should have the Red Paper, Special Pomics");
                                          myquest->active = false;
                                          ActiveQuest--;
                                          SaveQuest(myquest); 
                                        CQuest* thisquestlc = GServer->GetQuestByID(225); 
                                        if(thisquestlc!=0)
                                        {                   
                                          if( ActiveQuest < 10 )
                                          {
                                                QUESTS* myquestlc = new QUESTS;
                                                assert(myquestlc);
                                                myquestlc->questid = thisquestlc->questid;
                                                myquestlc->thisquest = thisquestlc;        
                                                myquestlc->active = true;
                                                for(int i=0;i<5;i++)
                                                    myquestlc->items[i] = 0;  
                                                    myquestlc->items[0] = 1;
                                                MyQuest.push_back( myquestlc );
                                                SaveQuest(myquestlc);  
                                                ActiveQuest++;
                                            }
                                         }                                                
                                          
                                    }
                                    
                                }
                                break;
                          case 102:
                                 {
                                    //For RoseMaryDoll quest
                                    myquest->items[i]++;
                                    if (myquest->items[i]==5)
                                    {
                                        myquest->items[1]++;
                                    }
                                    
                                }
                                break;
                          case 103:
                                 {
                                    //For Sellion's memorantum (813)
                                    myquest->items[i]++;
                                    if (myquest->thisquest->id==813&&myquest->items[0]==7)
                                    {
                                         //we deactivate 813 and activate 814
                                         LogQuest("You should have received the memorantum");
                                          myquest->active = false;
                                          ActiveQuest--;
                                          SaveQuest(myquest); 
                                        CQuest* thisquestlc = GServer->GetQuestByID(814); 
                                        if(thisquestlc!=0)
                                        {                   
                                          if( ActiveQuest < 10 )
                                          {
                                                QUESTS* myquestlc = new QUESTS;
                                                assert(myquestlc);
                                                myquestlc->questid = thisquestlc->questid;
                                                myquestlc->thisquest = thisquestlc;        
                                                myquestlc->active = true;
                                                for(int i=0;i<5;i++)
                                                    myquestlc->items[i] = 0;
                                                    myquestlc->items[0] = 1; 
                                                MyQuest.push_back( myquestlc );  
                                                ActiveQuest++;
                                                SaveQuest(myquestlc);
                                            }
                                         }                                                    
                                    }   
                                    
                                }
                                break;
                           case 104:
                                 {
                                      //standard issue for hunting pratices
                                      myquest->items[0]++;
                                      
                                      //Patch for the soldier 20 quest "Living as a True Soldier"
                                      if (myquest->thisquest->id==857&&i!=0)
                                      {
                                          switch (i)
                                          {
                                              case 1:
                                                  {
                                                    //Long Sword
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received an Long Sword !");
                                                    GServer->pakGMItemQuest(this,5,8,1,0,100,0,0,bufferquest);
                                                  }
                                                  break;
                                                case 2:
                                                  {
                                                       //Scimitar
                                                       char bufferquest[200];
                                                       sprintf ( bufferquest, "You received a Scimitar !");                                                       
                                                       GServer->pakGMItemQuest(this,164,8,1,0,100,0,0,bufferquest);
                                                  }
                                                  break;                                                                                           
                                                default:
                                                  {
                                                        //Battle Axe
                                                       char bufferquest[200];
                                                       sprintf ( bufferquest, "You received a Battle Axe !");                                                       
                                                       GServer->pakGMItemQuest(this,133,8,1,0,100,0,0,bufferquest);
                                                   }
                                                   break;                                              
                                         }
                                         
                                         myquest->active=false;
                                         ActiveQuest--;
                                         SaveQuest(myquest);
                                      }
                                      
                                      //Patch for the quest "Toddy stole the jewels"
                                      if (myquest->thisquest->id==956&&i!=0)
                                      {
                                          switch (i)
                                          {
                                              case 1:
                                                  {
                                                    //orc bow
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received an Orc Bow !");
                                                    GServer->pakGMItemQuest(this,204,8,1,0,100,0,0,bufferquest);
                                                  }
                                                  break;
                                                case 2:
                                                  {
                                                       //katar
                                                       char bufferquest[200];
                                                       sprintf ( bufferquest, "You received a Katar !");                                                       
                                                       GServer->pakGMItemQuest(this,403,8,1,0,100,0,0,bufferquest);
                                                  }
                                                  break;                                                                                           
                                                default:
                                                  {
                                                        //money
                                                        CharInfo->Zulies+=5000;
                                                        BEGINPACKET( pak, 0x71e );
                                                        ADDQWORD   ( pak, CharInfo->Zulies );
                                                        ADDBYTE    ( pak, 0x00 );
                                                        client->SendPacket( &pak );
                                                   }
                                                   break;                                              
                                         }
                                         
                                         myquest->active=false;
                                         ActiveQuest--;
                                         SaveQuest(myquest);
                                      }
                                      
                                   }
                                  break; 
                           case 105:
                                 {
                                      //for first job dealer.
                                      myquest->items[i]=5;                                      
                                  }                       
                                  break;                
                           case 3:
                                {
                                      if (myquest->thisquest->id==2010)
                                      {
                                        //for the vaccine spero quest, -- this time...
                                        myquest->items[i]--;                      
                                      }
                                      else
                                      {
                                          myquest->items[i]++;
                                      }

                                }
                                break;
                           case 106:
                                {
                                    //Patch for solitary Orias
                                    if (myquest->items[0]<5)
                                    {
                                       myquest->items[0]++;
                                    }
                                    else
                                    {
                                        if (myquest->items[1]<5)
                                        {
                                           myquest->items[1]++;                      
                                        }
                                        else
                                        {
                                            myquest->items[2]++;
                                        }
                                        
                                    }
                                    
                                }
                                break;
                           case 107:
                                {
                                      //Patch for Solitary Orias (reward)
                                      switch (i)
                                      {
                                          case 0:
                                              {
                                                //Animal Rod
                                               char bufferquest[200];
                                               sprintf ( bufferquest, "You received Animal Rod !");
                                                GServer->pakGMItemQuest(this,303,8,1,0,100,26,0,bufferquest);
                                              }
                                              break;
                                            case 1:
                                              {
                                                   //Sorcerer's wand
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received Sorcerer's Wand !");                                                       
                                                   GServer->pakGMItemQuest(this,333,8,1,0,100,26,0,bufferquest);
                                              }
                                              break;                                                                                           
                                            default:
                                              {
                                                    //money
                                                    CharInfo->Zulies+=1800;
                                                    BEGINPACKET( pak, 0x71e );
                                                    ADDQWORD   ( pak, CharInfo->Zulies );
                                                    ADDBYTE    ( pak, 0x00 );
                                                    client->SendPacket( &pak );
                                               }
                                               break;                                              
                                     }
                                     
                                     myquest->active=false;
                                     ActiveQuest--;
                                     SaveQuest(myquest);
                                }
                                break;
                           case 108:
                                {
                                      //Patch for lvl 30 class quest Hawker
                                      switch (i)
                                      {
                                          case 1:
                                              {
                                                   //White Wing Bow
                                                   myquest->items[4]=1;
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received White Wing Bow!");                                                       
                                                   GServer->pakGMItemQuest(this,205,8,1,0,100,41,0,bufferquest);
                                                   myquest->active = false;
                                                   ActiveQuest--;
                                                   SaveQuest(myquest);                                                 
                                              }
                                              break;
                                            case 2:
                                              {
                                                   //Rake Hand
                                                   myquest->items[4]=1;                                                   
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received Rake Hand!");                                                       
                                                   GServer->pakGMItemQuest(this,404,8,1,0,100,41,0,bufferquest);
                                                   myquest->active = false;
                                                   ActiveQuest--;
                                                   SaveQuest(myquest);
                                              }
                                              break;                                                                                           
                                            default:
                                              {
                                                  myquest->items[0]++;
                                               }
                                               break;                                              
                                     }
                                                                    
                                }
                                break;   
                           case 109:
                                {
                                      //Patch for lvl 30 class quest Muse
                                      switch (i)
                                      {
                                          case 0:
                                              {
                                                   //Mage's Rod
                                                   myquest->items[4]=1;
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received Mage's Rod!");                                                       
                                                   GServer->pakGMItemQuest(this,304,8,1,0,100,33,0,bufferquest);
                                                   myquest->active = false;
                                                   ActiveQuest--;
                                                   SaveQuest(myquest);                                                 
                                              }
                                              break;                                             
                                          case 1:
                                              {
                                                   //Elven Wand
                                                   myquest->items[4]=1;
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received Elven Wand!");                                                       
                                                   GServer->pakGMItemQuest(this,334,8,1,0,100,21,0,bufferquest);
                                                   myquest->active = false;
                                                   ActiveQuest--;
                                                   SaveQuest(myquest);
                                              }
                                              break;
                                            case 2:
                                              {
                                                   //Textual earing and Textual Necklace
                                                   myquest->items[4]=1;                                                   
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received Textual Earing!"); 
                                                   GServer->pakGMItemQuest(this,153,7,1,0,100,0,0,bufferquest);                                                   
                                                   sprintf ( bufferquest, "You received Textual Necklace!");                                                       
                                                   GServer->pakGMItemQuest(this,83,7,1,0,100,0,0,bufferquest);
                                                   myquest->active = false;
                                                   ActiveQuest--;
                                                   SaveQuest(myquest);
                                              }
                                              break;                                                                                           
                                            default:
                                              {
                                                  //Nothing
                                               }
                                               break;                                              
                                     }
                                                                    
                                }
                                break;
                           case 110:
                                {
                                      //Patch for lvl 30 class quest Dealer
                                      switch (i)
                                      {
                                          case 0:
                                              {
                                                    myquest->items[0]++;                                          
                                              }
                                              break;                                             
                                          case 1:
                                              {
                                                      myquest->items[0]++;                                              
                                              }
                                              break;
                                            case 3:
                                              {
                                                   //Iron Rifle
                                                   myquest->items[4]=1;                                                   
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received Iron Rifle!"); 
                                                   GServer->pakGMItemQuest(this,234,8,1,0,100,41,0,bufferquest);                                                   
                                                   myquest->active = false;
                                                   ActiveQuest--;
                                                   SaveQuest(myquest);
                                              }
                                              break; 
                                            case 4:
                                              {
                                                   //Basic launcher
                                                   myquest->items[4]=1;                                                   
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received Basic Launcher!"); 
                                                   GServer->pakGMItemQuest(this,262,8,1,0,100,41,0,bufferquest);                                                   
                                                   myquest->active = false;
                                                   ActiveQuest--;
                                                   SaveQuest(myquest);
                                              }
                                              break;                                                                                                                                          
                                            default:
                                              {
                                                  //Nothing
                                               }
                                               break;                                              
                                     }
                                                                    
                                }
                                break;    
                           case 111:
                                {
                                    //For Soldier quest lvl 30
                                    myquest->items[i]++;
                                    switch (i)
                                    {
                                           case 3:
                                             {
                                                   //Onion Mace
                                                   myquest->items[4]=1;                                                   
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received Onion Mace!"); 
                                                   GServer->pakGMItemQuest(this,35,8,1,0,100,41,0,bufferquest);                                                   
                                                   myquest->active = false;
                                                   ActiveQuest--;
                                                   SaveQuest(myquest);
                                             }
                                             break;                                                                                       
                                           case 4:
                                             {
                                                   //Battle Axe
                                                   myquest->items[4]=1;                                                   
                                                   char bufferquest[200];
                                                   sprintf ( bufferquest, "You received Battle Axe!"); 
                                                   GServer->pakGMItemQuest(this,133,8,1,0,100,53,0,bufferquest);                                                   
                                                   myquest->active = false;
                                                   ActiveQuest--;
                                                   SaveQuest(myquest);
                                             }
                                             break;                                           
                                           default:
                                             {
                                                   //nothing
                                             }
                                             break;
                                    }
                                    
                                }                                
                                break;
                           case 112:
                                {
                                      //Est quest (and various one too)
                                      myquest->items[1]++;
                                      if (myquest->items[1]>=myquest->thisquest->value2)
                                      {
                                        myquest->items[0]=1;
                                        myquest->items[1]=0;
                                      }

                                }
                                break;  
                           case 113:
                                {
                                      //Second job hawker
                                      myquest->items[2]++;
                                      if (myquest->items[2]==6)
                                      {
                                        if (myquest->items[0]==0)
                                        {
                                           myquest->items[0]=1;
                                        }
                                        else
                                        {
                                            myquest->items[1]=1;
                                        }
                                        
                                      }
                                      
                                      if (myquest->items[2]==7)
                                      {
                                        myquest->items[1]=1;
                                      }                                      

                                }
                                break; 
                          case 114:
                                 {
                                    //For main quest (broken staff)
                                    myquest->items[i]++;
                                    if (myquest->thisquest->id==236&&myquest->items[0]==1&&myquest->items[1]==1)
                                    {
                                         //we deactivate 236 and activate 237
                                          myquest->active = false;
                                          ActiveQuest--; 
                                          SaveQuest(myquest);
                                        CQuest* thisquestlc = GServer->GetQuestByID(237); 
                                        if(thisquestlc!=0)
                                        {                   
                                          if( ActiveQuest < 10 )
                                          {
                                                QUESTS* myquestlc = new QUESTS;
                                                assert(myquestlc);
                                                myquestlc->questid = thisquestlc->questid;
                                                myquestlc->thisquest = thisquestlc;        
                                                myquestlc->active = true;
                                                for(int i=0;i<5;i++)
                                                    myquestlc->items[i] = 0;
                                                    myquestlc->items[0] = 1;
                                                    myquestlc->items[1] = 1; 
                                                MyQuest.push_back( myquestlc );  
                                                ActiveQuest++;
                                                SaveQuest(myquestlc);
                                            }
                                         }                                                    
                                    }   
                                    
                                }
                                break;   
                          case 115:
                                 {
                                    //For main quest (sticky liquid)
                                    myquest->items[1]++;
                                    
                                    if (myquest->items[0]==0&&myquest->items[1]==0)
                                    {
                                       myquest->items[1]=1;
                                    }
                                    
                                    if (myquest->items[1]==4)
                                    {
                                       myquest->items[0]++;
                                       myquest->items[1]=0;
                                       LogQuest("One More...");
                                    }
                                    
                                    if (myquest->thisquest->id==239&&myquest->items[0]==10)
                                    {
                                         //we deactivate 239 and activate 240
                                          myquest->active = false;
                                          ActiveQuest--;
                                          SaveQuest(myquest); 
                                        CQuest* thisquestlc = GServer->GetQuestByID(240); 
                                        if(thisquestlc!=0)
                                        {                   
                                          if( ActiveQuest < 10 )
                                          {
                                                QUESTS* myquestlc = new QUESTS;
                                                assert(myquestlc);
                                                myquestlc->questid = thisquestlc->questid;
                                                myquestlc->thisquest = thisquestlc;        
                                                myquestlc->active = true;
                                                for(int i=0;i<5;i++)
                                                    myquestlc->items[i] = 0;
                                                    myquestlc->items[0] = 10;
                                                MyQuest.push_back( myquestlc );  
                                                ActiveQuest++;
                                                SaveQuest(myquestlc);
                                            }
                                         }                                                    
                                    }   
                                    
                                }
                                break;   
                          case 116:
                                 {
                                    //For main quest (sharp stingers and animal claws)
                                    if (i<2)
                                    {
                                       myquest->items[0]++;
                                    }
                                    else
                                    {
                                        myquest->items[1]++;
                                    }
                                                                                                            
                                    if (myquest->thisquest->id==262&&myquest->items[0]==20&&myquest->items[1]==20)
                                    {
                                         //we deactivate 262 and activate 263
                                          myquest->active = false;
                                          ActiveQuest--;
                                          SaveQuest(myquest); 
                                        CQuest* thisquestlc = GServer->GetQuestByID(263); 
                                        if(thisquestlc!=0)
                                        {                   
                                          if( ActiveQuest < 10 )
                                          {
                                                QUESTS* myquestlc = new QUESTS;
                                                assert(myquestlc);
                                                myquestlc->questid = thisquestlc->questid;
                                                myquestlc->thisquest = thisquestlc;        
                                                myquestlc->active = true;
                                                for(int i=0;i<5;i++)
                                                    myquestlc->items[i] = 0;
                                                    myquestlc->items[0] = 20;
                                                    myquestlc->items[1] = 20;
                                                MyQuest.push_back( myquestlc );  
                                                ActiveQuest++;
                                                SaveQuest(myquestlc);
                                            }
                                         }                                                    
                                    }   
                                    
                                }
                                break;  
                          case 117:
                                 {
                                    //For main quest (color scales)
                                    myquest->items[0]++;
                                                                                                        
                                    if (myquest->thisquest->id==267&&myquest->items[0]==10)
                                    {
                                         //we deactivate 267 and activate 268
                                          myquest->active = false;
                                          ActiveQuest--;
                                          SaveQuest(myquest); 
                                        CQuest* thisquestlc = GServer->GetQuestByID(268); 
                                        if(thisquestlc!=0)
                                        {                   
                                          if( ActiveQuest < 10 )
                                          {
                                                QUESTS* myquestlc = new QUESTS;
                                                assert(myquestlc);
                                                myquestlc->questid = thisquestlc->questid;
                                                myquestlc->thisquest = thisquestlc;        
                                                myquestlc->active = true;
                                                for(int i=0;i<5;i++)
                                                    myquestlc->items[i] = 0;
                                                    myquestlc->items[0] = 10;
                                                MyQuest.push_back( myquestlc );  
                                                ActiveQuest++;
                                                SaveQuest(myquestlc);
                                            }
                                         }                                                    
                                    }   
                                    
                                }
                                break;  
                          case 118:
                                 {
                                    //For main quest (zombie)
                                    if (i==1)
                                    {
                                       LogQuest("We force the Zombie...");
                                       itemquest_script=myquest->thisquest->value3;
                                    }
                                    
                                }
                                break;
                          case 119:
                                 {
                                    //For main quest (smoulies...)                                    
                                    myquest->items[i]++;
                                    if (i==1||i==3)
                                    {
                                       myquest->items[1]=1;
                                    } 
                                                                        
                                    if (i==2&&myquest->items[2]==1)
                                    {                                    
                                       LogQuest("We force the Smoulies...");
                                       itemquest_script=myquest->thisquest->value3;                                       
                                    }
                                    
                                    if (myquest->items[2]>1)
                                    {
                                       myquest->items[2]=2;
                                    }
                                    
                                    
                                    if (myquest->thisquest->id==274&&myquest->items[0]==10&&myquest->items[1]==1)
                                    {
                                          //we deactivate 274 and activate 275
                                          myquest->active = false;
                                          ActiveQuest--;
                                          SaveQuest(myquest); 
                                        CQuest* thisquestlc = GServer->GetQuestByID(275); 
                                        if(thisquestlc!=0)
                                        {                   
                                          if( ActiveQuest < 10 )
                                          {
                                                QUESTS* myquestlc = new QUESTS;
                                                assert(myquestlc);
                                                myquestlc->questid = thisquestlc->questid;
                                                myquestlc->thisquest = thisquestlc;        
                                                myquestlc->active = true;
                                                for(int i=0;i<5;i++)
                                                    myquestlc->items[i] = 0;  
                                                    myquestlc->items[0] = 10; 
                                                    myquestlc->items[1] = 1;                                                                                                         
                                                MyQuest.push_back( myquestlc );  
                                                ActiveQuest++;
                                                SaveQuest(myquestlc);
                                            }
                                         }                                       
                                    } 
                                    
                                }
                                break;                                                                                                                                                                                                                           
                           case 4:
                                {
                                    //For pomics quest (226)
                                    myquest->items[i]++;
                                    
                                    if (myquest->thisquest->id==226&&myquest->items[0]==6)
                                    {
                                         //we deactivate 226 and activate 227
                                         LogQuest("You should have finished the slaughter, Special Pomics");
                                          myquest->active = false;
                                          ActiveQuest--;
                                          SaveQuest(myquest); 
                                        CQuest* thisquestlc = GServer->GetQuestByID(227); 
                                        if(thisquestlc!=0)
                                        {                   
                                          if( ActiveQuest < 10 )
                                          {
                                                QUESTS* myquestlc = new QUESTS;
                                                assert(myquestlc);
                                                myquestlc->questid = thisquestlc->questid;
                                                myquestlc->thisquest = thisquestlc;        
                                                myquestlc->active = true;
                                                for(int i=0;i<5;i++)
                                                    myquestlc->items[i] = 0;  
                                                MyQuest.push_back( myquestlc );  
                                                ActiveQuest++;
                                                SaveQuest(myquestlc);
                                            }
                                         }                                                    
                                    }                                                                    
                                }
                                break;                               
                           default:
                                   {           
                                    //car quest...
                                    if (myquest->thisquest->id==805)
                                      {
                                            myquest->items[3]++;
                                            if (myquest->items[3]==1||myquest->items[3]==17||myquest->items[3]==33)
                                            {
                                                LogQuest("An item should have diseapear");
                                                myquest->items[4]--;
                                            }
                
                                      }
                                      else
                                      {
                                        //rev.70 org code...
                                        if (myquest->thisquest->numitems[i] <=myquest->items[i])
                                        {
                                           bool found= false;
                                           int j=i+1;
                                           for (int j=i+1; j<5;j++)
                                           {
                                                  found= (myquest->thisquest->numitems[j] > myquest->items[j]);
                                                  if (found)
                                                  {
                                                     myquest->items[j]++;
                                                     break;
                                                  }
                                           }
                                           break;
                                        }
                                        else
                                        {
                                            myquest->items[i]++;    
                                            break;    
                                        }
                                                                                  
                                      }                                       
           
                                   }
                                break;       
                    }
                    
                    break;
                    //LMA END                    
                }        
            }
            for(int i=0;i<5;i++) // look if Quest is Finished
            {
                if( myquest->thisquest->finalid!=0 )
                {
                    LogQuest("final id not 0, is %lu",myquest->thisquest->finalid);
                    break;
                }
                
                if( myquest->thisquest->numitems[i] != myquest->items[i] )
                {
                    //LMA begin
                    LogQuest("Quest not finished");
                    //LMA end
                    break; // Not Finished
                }
                
                if( i==4 )
                {
                    //LMA begin
                    LogQuest("Quest finished");
                    //LMA end
                    //Quest Finished
                    flag = true;
                  }
            }        
            
                 //LMA Begin
                 //20070621-211100
                 //sometimes if we receive an item quest event, we need to script...
                if (myquest->thisquest->script>0&&myquest->thisquest->value3!=0&&myquest->thisquest->value3==itemquest_script)
                {
                    LogQuest("Time for script for item quest %lu !!",questid);
                    GServer->DoQuestScript( this, myquest->thisquest );              
                }
                //LMA END
         }
        else
        {
            //LMA begin
            LogQuest("Questid %lu not found (by item).",questid); 
            //LMA end
        } 
           
    }
    
    if( flag ) // Check if finish the quest
    {  
        if( myquest!=0 )
        {
            if( myquest->active )
            {
                //LMA begin
                LogQuest("Quest reward time");
                //LMA end
                //Patch for Muse lvl 30 quest
                if (myquest->thisquest->id==9997)
                {
                   char bufferquest[200];
                   sprintf ( bufferquest, "You received Healing vials!");
                   GServer->pakGMItemQuest(this,2,10,2,0,100,0,0,bufferquest);
                }
                
                  GiveQuestReward( myquest->thisquest );
                  myquest->active = false;
                  ActiveQuest--;                           
            }
        }
    }
    
    //Last save.
    if(myquest!=0)
       SaveQuest(myquest);

    if(finalquest!=0)
    { 
        if(finalquest->script>0)
        {
            //LMA begin
            //20070621-211100
            //only if it's a "end of quest" script...
            LogQuest("Time for final script !!");
            if (finalquest->value3==0||finalquest->script==20)
            {
               return GServer->DoQuestScript( this, finalquest ); 
            }
            //LMA END
            
        }
        
       //LMA: We have to delete the reward quest on some cases...
       //[8000,8003] = Santa Rewards.
       if (finalquest->id>=8000&&finalquest->id<=8003)
       {
          LogQuest("Reward Santa, deleting Quest %i",finalquest->id);
          DelInactiveQuest(questid);
       }
        
    }
    
    
    return true;
}

//LMA: Let's change some face ;)
bool CPlayer::PlasticSurgeon(CQuest* thisquest)
{
     int questnb=thisquest->id;
     if (thisquest->value1==0||thisquest->value2==0||thisquest->value3==0||thisquest->ExpReward==0)
        return false;
     
     //Haircut (Girl Dumpling hair / pink dumpling), same for men.
     if(questnb==9500||questnb==9504)
     {
        if(!CheckItem(thisquest->value1,thisquest->value2,thisquest->ExpReward))
        {
           Log(MSG_HACK,"%s tried to hack quest %i",CharInfo->charname,questnb);
           return false;
        }

        CharInfo->Hair = (thisquest->value3*5);
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 9);
        ADDWORD(pak, CharInfo->Hair);
        ADDWORD(pak, 0);
        client->SendPacket(&pak);
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        client->SendPacket(&pak);
        LogQuest("Haircut changed to %i",CharInfo->Hair);
        GServer->SendPM(this, "Hair Changed!" );
       return true;
     }    
     
     //Face-Woman (cute and shame), same for men
     if(questnb==9501||questnb==9503)
     {
        if(!CheckItem(thisquest->value1,thisquest->value2,thisquest->ExpReward))
        {
           Log(MSG_HACK,"%s tried to hack quest %i",CharInfo->charname,questnb);
           return false;
        }

        CharInfo->Face = thisquest->value3;
        BEGINPACKET(pak, 0x721);
        ADDWORD(pak, 8);
        ADDWORD(pak, CharInfo->Face);
        ADDWORD(pak, 0);
        client->SendPacket(&pak);
        RESETPACKET(pak, 0x0730);
        ADDWORD(pak, 5);
        ADDWORD(pak, 0xa24d);
        ADDWORD(pak, 0x40b3);
        client->SendPacket(&pak); 
        LogQuest("Face changed to %i",CharInfo->Face);
        GServer->SendPM(this, "Face Changed!" );
       return true;
     }
          
     //Change sex
     if(questnb==9502)
     {
        if(!CheckItem(thisquest->value1,thisquest->value2,thisquest->ExpReward))
        {
           Log(MSG_HACK,"%s tried to hack quest %i",CharInfo->charname,questnb);
           return false;
        }

       if(CharInfo->Sex==0)
          CharInfo->Sex=1;
       else
          CharInfo->Sex=0;
       
        BEGINPACKET( pak, 0x720 );
        ADDWORD( pak, 2 );
        ADDWORD( pak, CharInfo->Sex );
        ADDWORD( pak, 0 );
        client->SendPacket( &pak );
        RESETPACKET( pak, 0x0730 );
        ADDWORD( pak, 5 );
        ADDWORD( pak, 0xa24d );
        ADDWORD( pak, 0x40b3 );
        client->SendPacket( &pak );
       GServer->SendPM(this, "Sex Changed!" );
       LogQuest("Sex changed to %i",CharInfo->Sex);
       return true;
     }
          
     LogQuest("Surgeon quest not handled %i",questnb);
     
     
     return false;
}

//LMA: checking if there is a skillbook in inventory
bool CPlayer::CheckItem(int itemnb,int familyid,int nb)
{
 	for(UINT i=0;i<MAX_INVENTORY;i++) 
    {
        if (items[i].itemnum==itemnb&&items[i].itemtype==familyid)
        {
            //Taking away one of them.
            /*Useless (done by client)
            BEGINPACKET( pak,0x7a3 );
            ADDWORD    ( pak, clientid );
            ADDWORD    ( pak, items[i].itemnum );
            ADDBYTE    ( pak, i );
            client->SendPacket( &pak );
            */
            if (items[i].count<nb)
            {
              Log(MSG_HACK,"Not enough items: %i/%i (%i:%i)",items[i].count,nb,familyid,itemnb);
              return false;
            }
            
            items[i].count -= nb;        
            if( items[i].count <= 0 )        
                ClearItem( items[i] );      
            
            LogQuest("We found a correct item! %i:%i",familyid,itemnb);
            return true;
        }           
                      
    }
     
     LogQuest("We did NOT found a correct item! %i:%i",familyid,itemnb);     
     return false;
}

//LMA: Saving a quest into database (Mysql 4.1+)
bool CPlayer::SaveQuest( QUESTS* myquest )
{
    char nqitem[200];	
	for( int j=0;j<5;j++ )
	{
        if( j==0 )
        {
	       sprintf(&nqitem[j], "%i",myquest->items[j]);
        }
        else
        {
	       sprintf(&nqitem[strlen(nqitem)], "|%i",myquest->items[j]);
        }
    }
    
    /*Mysql <4.1+
    GServer->DB->QExecute("INSERT INTO list_quest (owner,questid,nitems,active) VALUES (%i,%i,'%s',%i)",
    CharInfo->charid, myquest->thisquest->id, nqitem, myquest->active );    
     */     
     LogQuest("Saving quest %i",myquest->thisquest->id);
     
     //Mysql 4.1+
     GServer->DB->QExecute("INSERT INTO list_quest (owner,questid,nitems,active) VALUES (%i,%i,'%s',%i) ON DUPLICATE KEY UPDATE owner=VALUES(owner),questid=VALUES(questid),nitems=VALUES(nitems),active=VALUES(active)",
         CharInfo->charid, myquest->thisquest->id, nqitem, myquest->active ); 

     return true;
}

//LMA: delete inactive quest
bool CPlayer::DelInactiveQuest( unsigned long int questid )
{
     LogQuest("searching %lu",questid);
    QUESTS* myquest = GetQuestByQuestID( questid );
    if( myquest == 0 )
        return false;
        LogQuest("ok2");

    //Deleting also from MySQL        
    for(UINT i=0;i<MyQuest.size( );i++)
    {
        if(MyQuest.at(i)==myquest)
        {
            LogQuest("deleting Quest %i",myquest->thisquest->id);
            GServer->DB->QExecute( "DELETE FROM list_quest WHERE owner=%i AND questid=%i",CharInfo->charid,myquest->thisquest->id);
            MyQuest.erase( MyQuest.begin()+i );
            delete myquest;
            break;
        }
    }
    LogQuest("ok3");	
        
    
    return true;
}


bool CPlayer::DelQuest( unsigned long int questid )
{
    CQuest* thisquest = GServer->GetQuestByID( questid );
    if(thisquest==0)
        return true;       
    QUESTS* myquest = GetQuestByQuestID( thisquest->questid );
    if( myquest == 0 )
        return false;
    for(UINT i=0;i<MyQuest.size( );i++)
    {
        if(MyQuest.at(i)==myquest)
        {
            GServer->DB->QExecute( "DELETE FROM list_quest WHERE owner=%i AND questid=%i",CharInfo->charid,myquest->thisquest->id);
            MyQuest.erase( MyQuest.begin()+i );
            delete myquest;
            break;
        }
    }
	ActiveQuest--;	
    return true;
}


bool CPlayer::GiveQuestReward( CQuest* thisquest )
{
     
     //LMA: Check for direct quests rewards.
     if (thisquest->script==666&&thisquest->value1!=0)
     {
        //check if the parent quest exists.
        CQuest* thisquesttemp = GServer->GetQuestByID(thisquest->value1); 
        if(thisquesttemp==0)
        {
           Log(MSG_HACK, "[REWARD] player %s, Quest %i, Bogus Parent Quest %i", CharInfo->charname,thisquest->id, thisquest->value1 );
           return false;
        }
        
        //have we got this quest in list?
        QUESTS* myquest = GetQuestByQuestID( thisquesttemp->questid );
        if (myquest==0)
        {
           Log(MSG_HACK, "[REWARD] player %s, Quest %i, Parent Quest %i not in player list", CharInfo->charname, thisquest->id, thisquest->value1 );
           return false;
        }
        
        if (!myquest->active)
        {
           Log(MSG_HACK, "[REWARD] player %s, Quest %i, Parent Quest %i not active", CharInfo->charname, thisquest->id, thisquest->value1 );
           return false;
        }
                
        if (thisquest->value3!=0&&thisquest->value2<5)
        {
           //check if enough of an item quest to give the reward
           UINT lma_previous=0;
           lma_previous=myquest->items[thisquest->value2];
           if(myquest->items[thisquest->value2]<thisquest->value3)
           {
             Log(MSG_HACK, "[REWARD] player %s, Quest %i, Parent Quest %i hasn't enough items (%i<%i)", CharInfo->charname, thisquest->id, thisquest->value1, myquest->items[thisquest->value2], thisquest->value3 );
             return false;                                                                
           }
           
           //Taking items.
           myquest->items[thisquest->value2]-=thisquest->value3;
           LogQuest("[REWARD] player %s, Quest %i, Parent Quest %i we took %i, remains %i (was %i)", CharInfo->charname, thisquest->id, thisquest->value1, thisquest->value3,myquest->items[thisquest->value2],lma_previous);           
        }
        
     }
     //End of Checks.
     
     //Go on :)
    if( thisquest==0 )
        return false;
    if( thisquest->ExpReward>0 )//Give Exp
    {
        CharInfo->Exp += thisquest->ExpReward;
        BEGINPACKET( pak, 0x79b );
        ADDDWORD   ( pak, CharInfo->Exp );
        ADDWORD    ( pak, CharInfo->stamina );
		ADDWORD    ( pak, 0 );
        client->SendPacket( &pak );                        
    }   
    if( thisquest->ZulieReward>0 )//Give Zuly
    {
        CharInfo->Zulies += thisquest->ZulieReward;
        BEGINPACKET( pak, 0x71e );
        ADDQWORD   ( pak, CharInfo->Zulies );
        ADDBYTE    ( pak, 0x00 );
        client->SendPacket( &pak ); 
    } 
    for(int i=0;i<10;i++)
    {
        if( thisquest->Itemreward[i]==0 || thisquest->ItemType[i]==0 )
            break;
        CItem item;
        item.count = thisquest->CountItem[i];
        item.durability = 50;
        item.itemnum = thisquest->Itemreward[i];
        item.itemtype = thisquest->ItemType[i];
        item.lifespan = 100;
        item.refine = 0;
        item.stats = 0;
        item.socketed = 0;
        item.appraised = 0;    
        item.gem = 0;  
        unsigned int newslot = GetNewItemSlot( item );   
        if(newslot==0xffff)
            continue;
        if(items[newslot].count>0)
            items[newslot].count += item.count;
        else
            items[newslot] = item;                            
        BEGINPACKET( pak, 0x71f ); // Give Item
        ADDBYTE    ( pak, 0x01 );
        ADDBYTE    ( pak, newslot );
        ADDDWORD   ( pak, GServer->BuildItemHead( items[newslot] ) );
        ADDDWORD   ( pak, GServer->BuildItemData( items[newslot] ) );
        ADDDWORD( pak, 0x00000000 );
        ADDWORD ( pak, 0x0000 );
        ADDBYTE    ( pak, 0x00 );
        client->SendPacket( &pak );
    }      
       
    return true;
}

// Search Quest by QuestID
QUESTS* CPlayer::GetQuestByQuestID( unsigned long int questid )
{
    for(int i=0;i<MyQuest.size();i++)
    {
        QUESTS* myquest = MyQuest.at( i );
        if( myquest->questid == questid )
            return myquest;
    }
    return 0;
}

// Get MyQuest by ItemID
QUESTS* CPlayer::GetQuestByItemID( unsigned long int itemid )
{    
	for(int j=0; j<MyQuest.size( ); j++) 
    {
        QUESTS* myquest = MyQuest.at( j );
		for(int i=0;i<5;i++)
		{   // fixed by Drakia [If a quest had multiple monsters, but only one counter (Like the first hunting quest, kill 3 Jelly Bean's, or Mini-Jelly Bean's) you would need to kill 3 of the first creature in the list to finish the quest]
            if( itemid == myquest->thisquest->itemid[i] && myquest->active )
                return myquest;
        }
	}
	return 0;
}

// Get MyQuest by ItemID
QUESTS* CPlayer::GetQuestByMob( unsigned int mob )
{    
	for(int j=0; j<MyQuest.size( ); j++) 
    {
        QUESTS* myquest = MyQuest.at( j );
		for(int i=0;i<5;i++)
		{
            if( mob == myquest->thisquest->mobs[i] )
                return myquest;
        }
	}
	return 0;
}
