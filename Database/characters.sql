/*
MySQL Data Transfer
Source Host: 192.168.1.100
Source Database: osrose
Target Host: 192.168.1.100
Target Database: osrose
Date: 11/15/2007 7:54:02 PM
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for characters
-- ----------------------------
CREATE TABLE `characters` (
  `account_name` varchar(64) NOT NULL,
  `id` int(11) NOT NULL auto_increment,
  `char_name` varchar(64) NOT NULL,
  `zuly` decimal(11,0) NOT NULL default '10000',
  `level` decimal(11,0) NOT NULL default '1',
  `curHp` decimal(18,0) NOT NULL default '50',
  `curMp` decimal(18,0) NOT NULL default '18',
  `str` decimal(11,0) NOT NULL default '15',
  `con` decimal(11,0) NOT NULL default '15',
  `dex` decimal(11,0) NOT NULL default '15',
  `_int` decimal(11,0) NOT NULL default '15',
  `cha` decimal(11,0) NOT NULL default '10',
  `sen` decimal(11,0) NOT NULL default '10',
  `face` decimal(11,0) NOT NULL default '1',
  `hairStyle` decimal(11,0) NOT NULL default '1',
  `sex` decimal(11,0) NOT NULL default '0',
  `exp` decimal(11,0) NOT NULL default '0',
  `skillp` decimal(11,0) NOT NULL default '0',
  `statp` decimal(11,0) NOT NULL default '0',
  `clanid` decimal(11,0) NOT NULL default '0',
  `classid` decimal(11,0) NOT NULL default '0',
  `deletetime` decimal(20,0) NOT NULL default '0',
  `clan_rank` int(11) NOT NULL default '0',
  `stamina` int(11) NOT NULL default '5000',
  `quickbar` varchar(255) NOT NULL default '0',
  `basic_skills` varchar(255) NOT NULL default '11,12,13,14,15,16,17,18,19,20,21,22,25,5000,5001,101,102,103,104',
  `class_skills` varchar(300) NOT NULL default '0',
  `class_skills_level` varchar(300) NOT NULL default '1',
  `respawnid` int(11) NOT NULL default '56',
  `townid` int(11) NOT NULL default '6',
  `rewardpoints` int(11) NOT NULL default '0',
  `unionid` int(11) NOT NULL default '0',
  `unionfame` int(11) NOT NULL default '0',
  `union01` int(11) NOT NULL default '0',
  `union02` int(11) NOT NULL default '0',
  `union03` int(11) NOT NULL default '0',
  `union04` int(11) NOT NULL default '0',
  `union05` int(11) NOT NULL default '0',
  `bonusxp` int(11) NOT NULL default '0',
  `timerxp` int(11) NOT NULL default '0',
  `shoptype` int(11) NOT NULL default '0',
  `timershop` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
