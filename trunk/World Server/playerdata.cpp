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


// Load this characters userdata from the database
bool CPlayer::loaddata( )
{
	MYSQL_ROW row;
	MYSQL_RES *result = GServer->DB->QStore("SELECT level,face,hairStyle,sex,classid,zuly,str,dex,_int, con,cha,sen,curHp,curMp,id,statp,skillp,exp,stamina,quickbar,basic_skills, class_skills,class_skills_level,respawnid,clanid,clan_rank,townid FROM characters WHERE char_name='%s'", CharInfo->charname);
	if(result==NULL) return false;
	if(mysql_num_rows(result)!=1)
	{
        Log( MSG_WARNING, "Number of user with charname '%s' is %i", CharInfo->charname,mysql_num_rows(result));
        return false;
    }
	row = mysql_fetch_row(result);
	Stats->Level = atoi(row[0]);
	CharInfo->Face = atoi(row[1]);
	CharInfo->Hair = atoi(row[2]);
	CharInfo->Sex = atoi(row[3]);
	CharInfo->Job = atoi(row[4]);
	CharInfo->Zulies = atol(row[5]);
	Attr->Str = atoi(row[6]);
	Attr->Dex = atoi(row[7]);
	Attr->Int = atoi(row[8]);
	Attr->Con = atoi(row[9]);
	Attr->Cha = atoi(row[10]);
	Attr->Sen = atoi(row[11]);
	Stats->HP = atoi(row[12]);
	Stats->MP = atoi(row[13]);
	CharInfo->charid = atoi(row[14]);
	CharInfo->StatPoints = atoi(row[15]);
	CharInfo->SkillPoints = atoi(row[16]);
	CharInfo->Exp = atoi(row[17]);
	CharInfo->stamina = atoi(row[18]);
    Position->respawn = atoi(row[23]);	
    Clan->clanid = atoi(row[24]);
    Clan->clanrank = atoi(row[25]);
    Position->saved = atoi(row[26]);
	p_skills = 0;
	for(BYTE i=0;i<48;i++) 
    { 
        char* tmp=strtok((i==0?row[19]:NULL), ","); 
        if (tmp!=NULL) 
            quickbar[i]=atoi(tmp); 
        else  
            quickbar[i]=0;         
    }
	for(UINT i=0;i<42;i++) 
    { 
        char* tmp=strtok((i==0?row[20]:NULL), ","); 
        if (tmp!=NULL) 
            bskills[i]=atoi(tmp); 
        else    
            bskills[i]=0; 
    }	
	for(UINT i=0;i<60;i++) 
	{
        char* tmp=strtok((i==0?row[21]:NULL), ","); 
        if (tmp!=NULL)          
        {
            if(atoi(tmp)!=0)
                p_skills++;
            cskills[i].id=atoi(tmp);                     
        }
        else
            cskills[i].id=0;                    
    }
	for(UINT i=0;i<60;i++) 
    { 
        char* tmp=strtok((i==0?row[22]:NULL), ","); 
        if (tmp!=NULL)
            cskills[i].level=atoi(tmp); 
        else    
            cskills[i].level=1; 
    }
    for(int i=0;i<60;i++)
    {     
        if(cskills[i].id==0)
        {
            cskills[i].thisskill = NULL;
        }
        else
        {
            cskills[i].thisskill = GServer->GetSkillByID( cskills[i].id+cskills[i].level-1 );                
        }
    }
	GServer->DB->QFree( );	
	result = GServer->DB->QStore("SELECT itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem FROM items WHERE owner=%i", CharInfo->charid);
    if(result==NULL) return false;
	while(row = mysql_fetch_row(result)) 
    {
        if(!GServer->IsValidItem( atoi(row[1]), atoi(row[0]) ) || atoi(row[6])==0)
        {
            Log(MSG_WARNING, "char %s have a invalid or empty item in inventory: %i-%i [%i], this item will be deleted", CharInfo->charname, atoi(row[1]), atoi(row[0]), atoi(row[6]) );
            continue;
        }
		UINT itemnum = atoi(row[5]);        
		items[itemnum].itemnum = atoi(row[0]);
		items[itemnum].itemtype = atoi(row[1]);
		items[itemnum].refine = atoi(row[2]);
		items[itemnum].durability = atoi(row[3]);
		items[itemnum].lifespan = atoi(row[4]);
		items[itemnum].count = atoi(row[6]);
		items[itemnum].stats = atoi(row[7]);
		items[itemnum].socketed = (atoi(row[8])==1)?true:false;
		items[itemnum].appraised = (atoi(row[9])==1)?true:false;
		items[itemnum].gem = atoi(row[10])>3999?0:atoi(row[10]);
	}
	GServer->DB->QFree( );
	result = GServer->DB->QStore("SELECT itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem FROM storage WHERE owner=%i", Session->userid);
	if(result==NULL) return false;
	nstorageitems = mysql_num_rows(result);
	while(row = mysql_fetch_row(result)) 
    {
        if(!GServer->IsValidItem( atoi(row[1]), atoi(row[0]) ) || atoi(row[6])==0)
        {
            Log(MSG_WARNING, "char %s have a invalid or empty item in storage: %i%i [%i], this item will be deleted", CharInfo->charname, atoi(row[1]), atoi(row[0]), atoi(row[6]) );
            continue;
        }        
		UINT itemnum = atoi(row[5]);	
		storageitems[itemnum].itemnum = atoi(row[0]);
		storageitems[itemnum].itemtype = atoi(row[1]);
		storageitems[itemnum].refine = atoi(row[2]);
		storageitems[itemnum].durability = atoi(row[3]);
		storageitems[itemnum].lifespan = atoi(row[4]);
		storageitems[itemnum].count = atoi(row[6]);
		storageitems[itemnum].stats = atoi(row[7]);
		storageitems[itemnum].socketed = (atoi(row[8])==1)?true:false;
		storageitems[itemnum].appraised = (atoi(row[9])==1)?true:false;
		storageitems[itemnum].gem = atoi(row[10]);
	}
	GServer->DB->QFree( );
    if(Clan->clanid!=0)
    {	
    	result = GServer->DB->QStore("SELECT logo,back,name,grade FROM list_clan where id=%i", Clan->clanid);
        if(result==NULL) return false;    	
    	if(mysql_num_rows(result)!=1)
    	{
            Log(MSG_WARNING, "There are %i clan(s) with id %i", mysql_num_rows(result), Clan->clanid );
    	    Clan->clanid=0;
        }
    	else
    	{
    	    row = mysql_fetch_row(result);
    	    Clan->logo = atoi(row[0]);
    	    Clan->back = atoi(row[1]);    	   
    	    strcpy(Clan->clanname,row[2]);
    	    Clan->grade = atoi(row[3]);
        }
    	GServer->DB->QFree( );   
    }
    
    for (int i=0;i<25;i++)
    {
       QuestVariables[i]=0x00; 
    }
    
     //LMA begin
     //20070621-211100
    //Patch for soul crystal quest... 
     result = GServer->DB->QStore("SELECT questid, active from list_quest where owner=%i and questid>208 and questid<218",CharInfo->charid );
     if(result!=NULL)
     {    
           bool tryteh_done=false;
           bool spero_time=false;
           
           while(row = mysql_fetch_row(result))
           {
                     switch (atoi(row[0]))
                     {
                            case 209:
                                 {
                                     QuestVariables[1]=0x06;
                                 }
                                break;                                  
                            case 210:
                                 {
                                     QuestVariables[1]=0x07;                                     
                                 }
                                break;                                
                            case 211:
                                 {
                                     QuestVariables[1]=0x08;                                     
                                 }
                                break;                                  
                            case 215:
                                 {
                                      spero_time=true;
                                  }
                                 break;                                                            
                            case 217:
                                 {
                                      tryteh_done=true;
                                  }
                                 break;
                            default:
                                    break;
                     }
                                          
           }
           
           GServer->DB->QFree( );
           if (tryteh_done||(!spero_time))
           {
              QuestVariables[1]=0x00;
           }

     }
     
    //LMA END   
    
    //LMA BEGIN
    //20060623, 022000
    //Patch for Lunar Quest
    bool is_lunar=false;
    //LMA END
    
    result = GServer->DB->QStore("SELECT questid,nitems,active from list_quest where owner=%i order by active DESC",CharInfo->charid );
    if(result==NULL) return false;    
	while(row = mysql_fetch_row(result)) 
    {
        int questid = atoi(row[0]);        
        CQuest* thisquest = GServer->GetQuestByID( questid );
        if(thisquest==0)
            continue;
            
         //LMA BEGIN
        //20060623, 022000
        //Patch for Lunar Quest
        //Log( MSG_INFO, "Reading quest %u",thisquest->id);
        if (thisquest->id==808&&atoi(row[2]))
        {
           Log( MSG_INFO, "Patch Lunar part 1");
           is_lunar=true;
           continue;
        }        
        //LMA END

  
        QUESTS* myquest = new QUESTS;
        assert(myquest);
        myquest->questid = thisquest->questid;
        myquest->thisquest = thisquest;
        for(int i=0;i<5;i++)
        {
            char* tmp = strtok((i==0?row[1]:NULL),"|");
            if(tmp!=NULL)
                myquest->items[i] = atoi(tmp);
            else
                myquest->items[i] = 0;
        }
        
        //LMA BEGIN
        //20070621-215100
        //Various patches for the quests...
        //Patch for Spero quest
        if (thisquest->id==2000)
        {
           myquest->items[0]=0;
           myquest->items[1]=0;
           if (myquest->items[2]==1)
           {
               myquest->items[0]=1;     
           }
           
            if (myquest->items[2]==2)
           {
               myquest->items[0]=1; 
               myquest->items[1]=1;     
           }
        }
        
        //Patch for hunt for pomics
        if (thisquest->id==224||thisquest->id==226)
        {
           myquest->items[0]=0;
           myquest->items[1]=0;                                
        }
        
        //Patch for Ikaness
        if (thisquest->id==232)
        {
           myquest->items[0]=1;                             
        }        
 
        //patch for Rosemary Doll:
        if (thisquest->id==806)
        {
           myquest->items[0]=0;
           if(myquest->items[1]>0)
           {
               myquest->items[0]=1;                        
           }
           
        }   
 
         //patch for Sellion's memorantum
        if (thisquest->id==813)
        {
           myquest->items[0]=0;
        } 
                      
        //patch for car quest:
        if (thisquest->id==805)
        {
           myquest->items[0]=0;
           myquest->items[1]=0;
           myquest->items[2]=0;
           myquest->items[3]=0;
           UINT nb_papers= (UINT) myquest->items[4];
           
           if(nb_papers==3)
           {
               myquest->items[0]=1;
               myquest->items[1]=1;
               myquest->items[2]=1;                          
           }
           
           if (nb_papers==2)
           {
               myquest->items[0]=1;
               myquest->items[1]=1;       
           }
           
           if (nb_papers==1)
           {
              myquest->items[0]=1;
           }
           
        }   
        //LMA END

        myquest->active = atoi( row[2] );
        if( myquest->active )
            ActiveQuest++;
            
        //Patch soldier quest lvl 20:
        if (thisquest->id==856||thisquest->id==855)
        {
            myquest->items[0]=0;
            myquest->items[0]=0;
        }
        
        if (thisquest->id==855&&myquest->active&&myquest->items[0]==40)
        {
           myquest->active=false;
           ActiveQuest--;
           Log(MSG_INFO,"Patch for quest soldier lvl 20 startup");
        }

        //lvl 30 soldier quest
        if (myquest->thisquest->id==858)
        {
           int nb_ok=0;
           
           if (myquest->items[0]>0)
           {
             nb_ok++;
           }
           
           if (myquest->items[1]>0)
           {
             nb_ok++;
           }

           if (myquest->items[2]>0)
           {
             nb_ok++;
           }

           if (myquest->items[2]>=22)
           {
             nb_ok++;
           }

           if (myquest->items[1]>=12)
           {
             nb_ok++;
           }
           
           for (int k=0;k<5;k++)
           {
               myquest->items[k]=0;
           }
           
           for (int k=0;k<nb_ok;k++)
           {
               myquest->items[k]=1;
           }

        }

         //LMA BEGIN
        //20060623, 022000
        //Patch for Lunar Quest
        if (myquest->thisquest->id==807&&is_lunar)
        {
           Log( MSG_INFO, "Patch Lunar part 2");
           myquest->active = 1;
           ActiveQuest++;
        }        
        //LMA END
        
        //LMA BEGIN
        //20060622, 211500
        //no shadow quests...
        if(myquest->thisquest->id==63&&myquest->active)
        {
           myquest->active=!myquest->active;
           ActiveQuest--;
        }
        //LMA END
        
        //LMA: Quest of Eucar (Est)
        if (myquest->thisquest->id==2058)
        {
                if (QuestVariables[10]<0x02)
               {
                  QuestVariables[10]=0x01;
               }          
        }
        
        if(myquest->thisquest->id>=2060&&myquest->thisquest->id<=2086)
        {
           myquest->items[1]=0;
        }
        
        if (myquest->thisquest->id==1055||myquest->thisquest->id==1059||myquest->thisquest->id==1062||myquest->thisquest->id==1064||myquest->thisquest->id==1074||myquest->thisquest->id==239||myquest->thisquest->id==269)
        {
           myquest->items[1]=0;   
        }
        
        if (myquest->thisquest->id==1068)
        {
           myquest->items[2]=0;           
        }        

        if (myquest->thisquest->id==1057||myquest->thisquest->id==1070)
        {
           int nb_pieces=0;
           nb_pieces=myquest->items[0]+myquest->items[1]+myquest->items[2]+myquest->items[3]+myquest->items[4];
           
           for (int k=0;k<5;k++)
           {
              if (k<nb_pieces)
              {
                  myquest->items[k]=1;
              }
              else
              {
                  myquest->items[k]=0;
              }
              
           }
           
        }   
                
        if (myquest->thisquest->id==2066)
        {
                if (QuestVariables[10]<0x03)
               {
                  QuestVariables[10]=0x02;
               }          
        }

        if (myquest->thisquest->id==2071)
        {
                if (QuestVariables[10]<0x04)
               {
                  QuestVariables[10]=0x03;
               }          
        }
        
        if (myquest->thisquest->id==2076)
        {
                if (QuestVariables[10]<0x05)
               {
                  QuestVariables[10]=0x04;
               }          
        }
        
        //LMA: Patch for Flame of Eucar Quest:
        if (myquest->thisquest->id==2079)
        {
                if (QuestVariables[11]<0x02)
               {
                  QuestVariables[11]=0x01;
               }          
        }
           
        if (myquest->thisquest->id==2084)
        {
                if (QuestVariables[11]<0x03)
               {
                  QuestVariables[11]=0x02;
               }          
        }
        
        if (myquest->thisquest->id==2088)
        {
                if (QuestVariables[11]<0x04)
               {
                  QuestVariables[11]=0x03;
               }          
        }                  

        if (myquest->thisquest->id==2092)
        {
                if (QuestVariables[11]<0x05)
               {
                  QuestVariables[11]=0x04;
               }          
        } 
            
        //Patch for second jobs quests
        if (myquest->thisquest->id==1051||myquest->thisquest->id==1052||myquest->thisquest->id==1053||myquest->thisquest->id==1054)
        {
                if (QuestVariables[6]<0x02)
               {
                  QuestVariables[6]=0x01;
               }          
        } 

        //LMA: Main quest
        if (myquest->thisquest->id==201)
        {
           if (myquest->active)
           {
               if (QuestVariables[1]<0x02)
               {
                  QuestVariables[1]=0x01;
               }  
                               
           }
           else
           {
               if (QuestVariables[1]<0x03)
               {
                  QuestVariables[1]=0x02;
               }  
               
           }

        }  
        
        /*
         if (myquest->thisquest->id==234&&QuestVariables[1]<0x11)
        {
          QuestVariables[1]=0x10;
        }                  
         if (myquest->thisquest->id==235&&QuestVariables[1]<0x12)
        {
          QuestVariables[1]=0x11;
        } 
         if (myquest->thisquest->id==236&&QuestVariables[1]<0x13)
        {
          QuestVariables[1]=0x12;
        } 
         if (myquest->thisquest->id==237&&QuestVariables[1]<0x14)
        {
          QuestVariables[1]=0x13;
        }         
         if (myquest->thisquest->id==238&&QuestVariables[1]<0x15)
        {
          QuestVariables[1]=0x14;
        }  
         if (myquest->thisquest->id==239&&QuestVariables[1]<0x16)
        {
          QuestVariables[1]=0x15;
        }                       
         if (myquest->thisquest->id==240&&QuestVariables[1]<0x17)
        {
          QuestVariables[1]=0x16;
        }     
         if (myquest->thisquest->id==241&&QuestVariables[1]<0x18)
        {
          QuestVariables[1]=0x17;
        } 
         
         if (myquest->thisquest->id==242&&QuestVariables[1]<0x19)
        {
          QuestVariables[1]=0x18;
        } 
        */
         if (myquest->thisquest->id>=234&&myquest->thisquest->id<=246)
        {
          UINT lma_value=0;
          lma_value=16+(myquest->thisquest->id-234);
          if (QuestVariables[1]<(lma_value+1))
          {
           QuestVariables[1]=lma_value;
          }
          
        }       
  
        //TEST
        if (myquest->thisquest->id==247&&QuestVariables[1]<31)
        {
          QuestVariables[1]=30;
        }
         if (myquest->thisquest->id==248&&QuestVariables[1]<32)
        {
          QuestVariables[1]=31;
        }     
        
        //2do: other values
         if (myquest->thisquest->id==254&&QuestVariables[1]<40)
        {
          QuestVariables[1]=40;
        }     
          //2do: other values
         if (myquest->thisquest->id==261&&QuestVariables[1]<51)
        {
          QuestVariables[1]=50;
        }            
         if (myquest->thisquest->id==262&&QuestVariables[1]<52)
        {
          QuestVariables[1]=51;
        } 
        //2Do: other values      
         if (myquest->thisquest->id==264&&QuestVariables[1]<54)
        {
          QuestVariables[1]=53;
        }           
        //2do: other values
          if (myquest->thisquest->id==266&&QuestVariables[1]<54)
        {
          QuestVariables[1]=55;
        }       
        //2do: other values
          if (myquest->thisquest->id==269&&QuestVariables[1]<59)
        {
          QuestVariables[1]=58;
        }         
         //2do: other values
          if (myquest->thisquest->id==273&&QuestVariables[1]<65)
        {
          QuestVariables[1]=64;
        }   
          //2do: other values
          if (myquest->thisquest->id==274&&QuestVariables[1]<66)
        {
          QuestVariables[1]=65;
        }        
         
        
        //TEST 282 (ok)
        /*
         if (myquest->thisquest->id==282&&QuestVariables[1]<75)
        {
          QuestVariables[1]=74;
        } 
        */        
         
        //test on unactive quests.                                
        if( ! myquest->active )
        {    
            //Main quest
            if (myquest->thisquest->id==212&&QuestVariables[1]<0x04)
            {
              QuestVariables[1]=0x03;
            }
            if (myquest->thisquest->id==213&&QuestVariables[1]<0x05)
            {
              QuestVariables[1]=0x04;
            }
            if (myquest->thisquest->id==214&&QuestVariables[1]<0x06)
            {
              QuestVariables[1]=0x05;
            }
            if (myquest->thisquest->id>=218&&myquest->thisquest->id<=220&&QuestVariables[1]<0x0A)
            {
              QuestVariables[1]=0x09;
            }       
             if (myquest->thisquest->id==101&&QuestVariables[1]<0x0B)
            {
              QuestVariables[1]=0x0A;
            }   
             if (myquest->thisquest->id==104&&QuestVariables[1]<0x0C)
            {
              QuestVariables[1]=0x0B;
            }                    
             if (myquest->thisquest->id==107&&QuestVariables[1]<0x0D)
            {
              QuestVariables[1]=0x0C;
            }   
             if (myquest->thisquest->id==223&&QuestVariables[1]<0x0E)
            {
              QuestVariables[1]=0x0D;
            }        
             if (myquest->thisquest->id==231&&QuestVariables[1]<0x0F)
            {
              QuestVariables[1]=0x0E;
            }    
             if (myquest->thisquest->id==233&&QuestVariables[1]<0x10)
            {
              QuestVariables[1]=0x0F;
            }   
             if (myquest->thisquest->id==234&&QuestVariables[1]<0x11)
            {
              QuestVariables[1]=0x10;
            }  
                                                                                 
            //second level job (Soldier/muse/hawker/dealer)
            if (myquest->thisquest->id==1061||myquest->thisquest->id==1067||myquest->thisquest->id==1073||myquest->thisquest->id==1079)
            {
                QuestVariables[6]=0x02;          
            }   

            
            if (myquest->thisquest->id==2092)
            {
                QuestVariables[11]=0x05;          
            }   
                  
            if (myquest->thisquest->id==2077)
            {
                QuestVariables[10]=0x05;
            }             
            
            //LMA: Patch for lvl 30 class quests.
            if (myquest->thisquest->id==857|myquest->thisquest->id==907||myquest->thisquest->id==956||myquest->thisquest->id==1006||myquest->thisquest->id==1007)
            {
               if (QuestVariables[5]<0x04)
               {
                  QuestVariables[5]=0x03;
               }
               
            }
            
            //lvl 30 quest are done.
            if (myquest->thisquest->id==912)
            {
               if (QuestVariables[5]<0x07)
               {
                  QuestVariables[5]=0x06;
               }
            }

            if (myquest->thisquest->id==9997||myquest->thisquest->id==858)
            {
               QuestVariables[5]=0x07;
            }
            
            if (myquest->thisquest->id==1008)
            {
               QuestVariables[5]=0x07;
            }
            
            //For Hawkers
            if (((myquest->thisquest->id==957||myquest->thisquest->id==958)&&myquest->items[4]==1)||myquest->thisquest->id==959)
            {
               QuestVariables[5]=0x07;
            }
                                    
            //Patch to end Spero Quest.
            if (myquest->thisquest->id==2011)
            {
               QuestVariables[8]=0x07;
            }            
            
            //Log(MSG_INFO, "BLAAAAAAAAA!"); test non-sense
            switch (myquest->thisquest->id)
            {
                   case 814: Log(MSG_INFO, "Player: %s can use flying vessel", CharInfo->charname); canUseFlyingVessel = true; break;
                   case 9999: Log(MSG_INFO, "Player: %s can speak Luna lang", CharInfo->charname); speaksLuna = true; break;
            }
        }
        MyQuest.push_back( myquest );             
    }   	
   	GServer->DB->QFree( );  
	Session->isLoggedIn = true;	
    GServer->DB->QExecute("UPDATE accounts SET online=true where id=%u", Session->userid );
    
    // Calculate how many fairies are available online, according to amoutn of players
	int oldFairyMax = GServer->Config.FairyMax;
    GServer->Config.FairyMax = (int)ceil((float)GServer->ClientList.size() / 50.0); //(1 fairy more every 50 player)
	if( oldFairyMax < GServer->Config.FairyMax ){
        CFairy* thisfairy = new (nothrow) CFairy;
        thisfairy->LastTime = clock();
        thisfairy->assigned = 0;
        thisfairy->ListIndex = 0;
        thisfairy->WaitTime = GServer->Config.FairyWait;
        GServer->FairyList.push_back( thisfairy );
    }
	return true;
}

// This is a REALLY bad way of saving the character data, but it works ^^
void CPlayer::savedata( )
{
    lastSaveTime = clock();
    if(Session->userid!=0)
    {	       
        CMap* map = GServer->MapList.Index[Position->Map];
    	CRespawnPoint* thisrespawn = map->GetNearRespawn( this );
    	if(thisrespawn == NULL)
    	   Position->respawn = 4;
    	else
    	   Position->respawn = thisrespawn->id;	           
	    char quick[1024];
	    char basic[1024];
	    char sclass[1024];	
 	    char slevel[1024];	
    	for(UINT i=0;i<MAX_SKILL;i++)
    	{
            if(i==0)
            {
    	       sprintf(&sclass[i], "%i",cskills[i].id);
    	       sprintf(&slevel[i], "%i",cskills[i].level);                
            }
            else
            {
    	       sprintf(&sclass[strlen(sclass)], ",%i",cskills[i].id);
    	       sprintf(&slevel[strlen(slevel)], ",%i",cskills[i].level);                            
            }
        }	   
    	for(UINT i=0;i<42;i++)
    	{
            if(i==0)
            	sprintf(&basic[i], "%i",bskills[i]);
            else
                sprintf(&basic[strlen(basic)], ",%i",bskills[i]);
        }	
        for(UINT i=0;i<48;i++)
        {
            if(i==0)
            	sprintf(&quick[i], "%i",quickbar[i]);
            else
                sprintf(&quick[strlen(quick)], ",%i",quickbar[i]);        
        }
        long int hp = Stats->HP;
    	if(hp<=0)
    	   hp=Stats->MaxHP * 10 / 100;
	   if(Stats->MP<0)
	       Stats->MP=0;	   
        GServer->DB->QExecute("UPDATE characters SET classid=%i,level=%i,zuly=%i,curHp=%i,curMp=%i,str=%i,con=%i,dex=%i,_int=%i,cha=%i,sen=%i,exp=%i,skillp=%i,statp=%i, stamina=%i,quickbar='%s',class_skills='%s',class_skills_level='%s',basic_skills='%s',respawnid=%i,clanid=%i,clan_rank=%i, townid=%i WHERE id=%i", 
                    CharInfo->Job,Stats->Level, CharInfo->Zulies, hp, Stats->MP, 
                    Attr->Str,Attr->Con,Attr->Dex,Attr->Int,Attr->Cha,Attr->Sen,
                    CharInfo->Exp,CharInfo->SkillPoints,CharInfo->StatPoints,CharInfo->stamina, 
                    quick, sclass,slevel,basic,Position->respawn,Clan->clanid,Clan->clanrank,Position->saved,CharInfo->charid );
    	if(!GServer->DB->QExecute("DELETE FROM items WHERE owner=%i", CharInfo->charid)) return;
    	for(UINT i=0;i<MAX_INVENTORY;i++) 
        {
    		if (items[i].count > 0) 
            {
    			GServer->DB->QExecute("INSERT INTO items (owner,itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem) VALUES(%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i)",
    								CharInfo->charid, items[i].itemnum, items[i].itemtype,items[i].refine, items[i].durability,
    								items[i].lifespan, i, items[i].count, items[i].stats, (items[i].socketed?1:0),
    								(items[i].appraised?1:0),items[i].gem );
    		}
    	}    	
    	if(!GServer->DB->QExecute("DELETE FROM storage WHERE owner=%i", Session->userid)) return;
    	for(UINT i=0;i<160;i++) 
        {
    		if (storageitems[i].count > 0) 
            {
    			GServer->DB->QExecute("INSERT INTO storage (owner,itemnum,itemtype,refine,durability,lifespan,slotnum,count,stats,socketed,appraised,gem) VALUES(%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i)",
    								Session->userid, storageitems[i].itemnum, storageitems[i].itemtype,storageitems[i].refine, storageitems[i].durability, 
    								storageitems[i].lifespan, i, storageitems[i].count, storageitems[i].stats, (storageitems[i].socketed?1:0), 
                                    (storageitems[i].appraised?1:0), storageitems[i].gem );//Gem Fix by Irulagain
    		}
    	}  
    	if(!GServer->DB->QExecute( "DELETE FROM list_quest WHERE owner=%i",CharInfo->charid )) return;
    	for(int i=0;i<MyQuest.size();i++)
    	{
            QUESTS* myquest = MyQuest.at( i );
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
            GServer->DB->QExecute("INSERT INTO list_quest (owner,questid,nitems,active) VALUES (%i,%i,'%s',%i)",
            CharInfo->charid, myquest->thisquest->id, nqitem, myquest->active );
        }
		GServer->DB->QExecute("update accounts set zulystorage = %i where id = %i", CharInfo->Storage_Zulies, Session->userid);
		Log(MSG_INFO, "Data Saved for char '%s' ", CharInfo->charname );
    }
}
