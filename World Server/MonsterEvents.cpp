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
#include "worldmonster.h"

// called when a monster is attacked  [attack/use atk skill/use buff/run/summon]
bool CMonster::OnBeAttacked( CCharacter* Enemy )
{
    if(!IsOnBattle( ))
    {
        //Some monsters do not attack and stay still (mc)
        if(!stay_still)
        {
            StartAction( Enemy, NORMAL_ATTACK, 0 ); 
        }
        else
        {
            StartAction( Enemy, STAY_STILL_ATTACK, 0 ); 
        }
        
    }
    
    return true;        
}

// called when a monster die [give exp/give extra drop]
bool CMonster::OnDie( )
{
    CMap* map = GServer->MapList.Index[Position->Map];
    if(map->ghost!=0)
    {
     if((map->IsNight( ) || map->ghost==2) && !IsGhost( ) && !IsGhostSeed( ) && !IsSummon( ))
       {
        UINT gs = GServer->RandNumber( 0, 100 );
        if(gs<30) // 30% / 70%
            {   // spawn a ghost seed [for now will be 701 [level 1 ghost seed] ]
                map->AddMonster( 701, Position->current, 0, NULL, NULL, Position->respawn, true );
            }
        }
    }
    
    //LMA begin
    //CF mode 1
    //20070621-211100
    UINT special_exp=0;
    UINT special_lvl=0;
    
    if (map->is_cf==1)
    {
       //what monster is dead?
       if(this->montype==map->id_def_mon)
       {
           //oh my god, they killed a j&b !! (or the monster with exp)
           special_exp=(UINT) map->mon_lvl;
           special_lvl= (UINT) map->mon_exp;
       }
       else
       {
           if (this->montype!=map->id_temp_mon)
           {
                UINT gs = GServer->RandNumber( 0, 100 );
                if(gs<map->percent)
                {
                  //we use the temporary monster as a decoy.
                  fPoint position_cf = GServer->RandInCircle( Position->current, 50 );
                  CMonster* monster2 = map->AddMonster( map->id_temp_mon, position_cf, 0, NULL, NULL, 0, true );           
                }
                
           }

       }
       
    }
    
    GServer->GiveExp( this , special_lvl, special_exp );
    //LMA End
    

    return true; 
}
// called when we create a new monster [attack players?]
bool CMonster::OnSpawn( bool Attack )
{
     lastSighCheck = clock();
    if(IsGhost( ) && Attack )
    {
        CPlayer* player = GetNearPlayer( );
        if(player==NULL) // no players or too far
            return true;
        StartAction( (CCharacter*)player, NORMAL_ATTACK, 0 ); 
    }
    return true;    
}

// [not]called when a monster is almost dead [run/use only skills/call a healer? :S xD]
bool CMonster::OnAlmostDie( )
{
    return true;    
}

// called when a enemy is on sight 
bool CMonster::OnEnemyOnSight( CPlayer* Enemy )
{
    clock_t etime = clock() - lastSighCheck;
    if(etime<5000) return true;
    if(!IsOnBattle( ))
    {
        if(thisnpc->aggresive>1)
        {
            UINT aggro = GServer->RandNumber(2,15);
            if(thisnpc->aggresive>=aggro && !IsGhostSeed( ))
            {
                Enemy->ClearObject( this->clientid );
				SpawnMonster(Enemy, this );
                StartAction( (CCharacter*) Enemy, NORMAL_ATTACK, 0 );
            }
            else
            if(IsGhostSeed( ) || thisnpc->aggresive>5)
                MoveTo( Enemy->Position->current, true );
        }
        lastSighCheck = clock();
    }
    return true;    
}

// called when enemy die
bool CMonster::OnEnemyDie( CCharacter* Enemy )
{    
    Position->destiny = Position->source; //ON MOB DIE
    ClearBattle( Battle );
    MoveTo( Position->source );
    return true;        
}

bool CMonster::OnFar( )
{
    Position->destiny = Position->source; //ON TARGET LOST
    ClearBattle( Battle );
    MoveTo( Position->source );
    return true;
}


//LMA: Arnold code for guardiantree
bool CMonster::Guardiantree(CMonster* monster,CMap* map)
{
     if(Stats->HP>9000)monster->hitcount=0;
     switch (monster->hitcount)
     {
         case 0:
              if(Stats->HP<9000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                  }
                  monster->hitcount=1;
              }
              break;
         case 1:
              if(Stats->HP<8000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                  }
                  monster->hitcount=2;
              }
              break;
         case 2:
              if(Stats->HP<7000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                  }
                  monster->hitcount=3;
              }
              break;
         case 3:
              if(Stats->HP<6000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                  }
                  monster->hitcount=4;
              }
              break;
         case 4:
              if(Stats->HP<5000)
              {
                  for(unsigned char i=0;i<5;i++)
                  {
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                  }
                  monster->hitcount=5;
              }
              break;
         case 5:
              if(Stats->HP<4000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                  }
                  monster->hitcount=6;
              }
              break;
         case 6:
              if(Stats->HP<3000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                  }
                  monster->hitcount=7;
              }
              break;
         case 7:
              if(Stats->HP<2000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                  }
                  monster->hitcount=8;
              }
              break;
         default:
                 monster->hitcount=9;
                 
     }
}

//LMA: Handling Moonchild siblings
bool CMonster::MoonChild(CMonster* monster,CMap* map)
{
     if (monster->hitcount>=monster->maxhitcount)
        return true;
     
     if(Stats->HP>(Stats->MaxHP*0.98))
         monster->hitcount=0;
         
     CPlayer* player = NULL;
     
         
     switch (monster->hitcount)
     {
         case 0:
              if(Stats->HP<(Stats->MaxHP*0.98))
              {
                  Log(MSG_INFO,"case 0 for the MC");
                  for(unsigned char i=0;i<2;i++)
                  {
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* monster2=map->AddMonster( 657, position, 0, NULL, NULL, 0, true );

                      if(i==0)
                         player = monster2->GetNearPlayer(20);    //getting near player.

                     if(player!=NULL)
                         monster2->StartAction( (CCharacter*)player, NORMAL_ATTACK, 0 );
                     
                  }
                  for(unsigned char i=0;i<3;i++)
                  {
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* monster2=map->AddMonster( 658, position, 0, NULL, NULL, 0, true );

                     if(player!=NULL)
                         monster2->StartAction( (CCharacter*)player, NORMAL_ATTACK, 0 );                      
                     
                  }
                  
                  monster->hitcount=1;
              }
              break;
         case 1:
              if(Stats->HP<(Stats->MaxHP*0.80))
              {
                  Log(MSG_INFO,"case 1 for the MC");                                               
                  for(unsigned char i=0;i<3;i++)
                  {
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* monster2=map->AddMonster( 657, position, 0, NULL, NULL, 0, true );

                      if(i==0)
                         player = monster2->GetNearPlayer(20);    //getting near player.

                     if(player!=NULL)
                         monster2->StartAction( (CCharacter*)player, NORMAL_ATTACK, 0 );
                  }
                  for(unsigned char i=0;i<4;i++)
                  {
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* monster2=map->AddMonster( 658, position, 0, NULL, NULL, 0, true );

                     if(player!=NULL)
                         monster2->StartAction( (CCharacter*)player, NORMAL_ATTACK, 0 );
                     
                  }
                  monster->hitcount=2;
              }
              break;
         case 2:
              if(Stats->HP<(Stats->MaxHP*0.70))
              {
                  Log(MSG_INFO,"case 2 for the MC");                                               
                  for(unsigned char i=0;i<10;i++)
                  {
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* monster2=map->AddMonster( 657, position, 0, NULL, NULL, 0, true );

                      if(i==0)
                         player = monster2->GetNearPlayer(20);    //getting near player.

                     if(player!=NULL)
                         monster2->StartAction( (CCharacter*)player, NORMAL_ATTACK, 0 );
                     
                  }
                  for(unsigned char i=0;i<5;i++)
                  {
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* monster2=map->AddMonster( 658, position, 0, NULL, NULL, 0, true );

                     if(player!=NULL)
                         monster2->StartAction( (CCharacter*)player, NORMAL_ATTACK, 0 );                      
                     
                  }
                  monster->hitcount=monster->maxhitcount;
              }
              break;

         default:
                 monster->hitcount=monster->maxhitcount;
                 
     }
     
     
     return true;
}

//LMA: Handling Worm Dragon :)
bool CMonster::WormDragon(CMonster* monster,CMap* map)
{
     if (monster->hitcount>=monster->maxhitcount)
        return true;
     
     if(Stats->HP>(Stats->MaxHP*0.50))
         return true;
         
     CPlayer* player = NULL;
     int nb_worms =0;
     nb_worms=GServer->RandNumber(2,4);
     
      Log(MSG_INFO,"case 0 for the Worm Dragon (%i)",nb_worms);
      for(unsigned char i=0;i<nb_worms;i++)
      {
          fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
          CMonster* monster2=map->AddMonster( 202, position, 0, NULL, NULL, 0, true );

          if(i==0)
             player = monster2->GetNearPlayer(20);    //getting near player.

         if(player!=NULL)
             monster2->StartAction( (CCharacter*)player, NORMAL_ATTACK, 0 );         
      }
      
      monster->hitcount=monster->maxhitcount;
     
     
     return true;
}
