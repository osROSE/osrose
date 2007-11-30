/*
MySQL Data Transfer
Source Host: 192.168.1.100
Source Database: osrose
Target Host: 192.168.1.100
Target Database: osrose
Date: 11/29/2007 4:05:23 PM
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for list_npcs
-- ----------------------------
CREATE TABLE `list_npcs` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(255) NOT NULL,
  `type` int(11) NOT NULL,
  `map` int(11) NOT NULL,
  `dir` float NOT NULL,
  `x` float NOT NULL,
  `y` float NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=158 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `list_npcs` VALUES ('1', '[Village Chief] Cornell', '1001', '1', '180', '5216', '5112');
INSERT INTO `list_npcs` VALUES ('2', '[Akram Kingdom Minister] Warren', '1002', '1', '180', '5208', '5112');
INSERT INTO `list_npcs` VALUES ('3', '[Co-Founder of the Junon Order] Francis', '1003', '1', '135', '5080', '5065');
INSERT INTO `list_npcs` VALUES ('4', '[Ferrell Guild Staff] Crow', '1004', '1', '0', '5094', '5259');
INSERT INTO `list_npcs` VALUES ('5', '[Righteous Crusader] Leonard', '1005', '1', '45', '5261', '5166');
INSERT INTO `list_npcs` VALUES ('6', '[Arumic Merchant] Tryteh', '1006', '1', '270', '5312', '5152');
INSERT INTO `list_npcs` VALUES ('7', '[Gypsy Jewel Seller] Mina', '1007', '1', '225', '5292', '5127');
INSERT INTO `list_npcs` VALUES ('8', '[Weapon Seller] Raffle', '1008', '1', '315', '5303', '5231');
INSERT INTO `list_npcs` VALUES ('9', '[Armor Seller] Carrion', '1009', '1', '270', '5314', '5198');
INSERT INTO `list_npcs` VALUES ('10', '[Designer] Keenu', '1010', '1', '0', '5273', '5257');
INSERT INTO `list_npcs` VALUES ('11', '[Eccentric Inventor] Spero', '1011', '1', '45', '5213', '5255');
INSERT INTO `list_npcs` VALUES ('12', '[Ferrell Guild Staff] Ulysses', '1012', '1', '90', '5071', '5207');
INSERT INTO `list_npcs` VALUES ('13', '[Tavern Owner] Sharlin', '1013', '1', '315', '5131', '5218');
INSERT INTO `list_npcs` VALUES ('14', '[Guide] Lena', '1014', '1', '135', '5240', '5124');
INSERT INTO `list_npcs` VALUES ('15', '[Resident] Luth', '1015', '1', '270', '5130', '5153');
INSERT INTO `list_npcs` VALUES ('16', '[Livestock Farmer] Lampa', '1016', '1', '190', '5110', '5363');
INSERT INTO `list_npcs` VALUES ('17', '[Event Info] Judy', '1201', '1', '135', '5262', '5212');
INSERT INTO `list_npcs` VALUES ('18', '[Teleporter] Selladin', '1017', '1', '180', '5244', '5222');
INSERT INTO `list_npcs` VALUES ('19', '[Cornell\'s Grandson] Cheney', '1018', '1', '45', '5441', '5493');
INSERT INTO `list_npcs` VALUES ('20', '[Lena\'s Sister] Shirley', '1019', '1', '225', '5512', '5291');
INSERT INTO `list_npcs` VALUES ('21', '[Keenu\'s Daughter] Arisa', '1020', '1', '225', '5431', '5088');
INSERT INTO `list_npcs` VALUES ('22', '[Historian] Jones', '1104', '2', '160', '5578', '4890');
INSERT INTO `list_npcs` VALUES ('23', '[Ferrell Guild Staff] Itz', '1100', '2', '270', '5603', '5034');
INSERT INTO `list_npcs` VALUES ('24', '[Ferrell Guild Staff] Hayen', '1099', '2', '270', '5603', '5046');
INSERT INTO `list_npcs` VALUES ('25', '[Tavern Owner] Harin', '1097', '2', '90', '5681', '5131');
INSERT INTO `list_npcs` VALUES ('26', '[Gypsy Jewel Seller] Bellia', '1092', '2', '270', '5726', '5202');
INSERT INTO `list_npcs` VALUES ('27', '[Ferrell Guild Merchant] Mildun', '1096', '2', '180', '5672', '5193');
INSERT INTO `list_npcs` VALUES ('28', '[Weapon Merchant] Crune', '1093', '2', '300', '5729', '5225');
INSERT INTO `list_npcs` VALUES ('29', '[Armor Merchant] Saki', '1094', '2', '270', '5722', '5290');
INSERT INTO `list_npcs` VALUES ('30', '[Manager of Ferrell] Arothel', '1089', '2', '270', '5713', '5360');
INSERT INTO `list_npcs` VALUES ('31', '[Ferrell Guild Staff] Charrs', '1110', '2', '270', '5713', '5368');
INSERT INTO `list_npcs` VALUES ('32', '[Guide] Eva', '1082', '2', '135', '5506', '5346');
INSERT INTO `list_npcs` VALUES ('33', '[Soldier] Winters', '1117', '2', '0', '5523', '5396');
INSERT INTO `list_npcs` VALUES ('34', '[Soldier] Odelo', '1116', '2', '0', '5507', '5396');
INSERT INTO `list_npcs` VALUES ('35', '[Righteous Crusader] Gawain', '1090', '2', '90', '5335', '5354');
INSERT INTO `list_npcs` VALUES ('36', '[Clan Owner] Burtland', '1115', '2', '100', '5288', '5264');
INSERT INTO `list_npcs` VALUES ('147', '[1st Arena Staff] Regina', '1751', '2', '100', '5288', '5243');
INSERT INTO `list_npcs` VALUES ('148', '[Vicious Captain] Ruven', '1125', '2', '270', '5597', '4918');
INSERT INTO `list_npcs` VALUES ('155', '[Clan Merchant] Aliche Patt', '1752', '2', '100', '5288', '5260');
INSERT INTO `list_npcs` VALUES ('37', '[Righteous Crusader] Huffe', '1111', '2', '90', '5335', '5346');
INSERT INTO `list_npcs` VALUES ('38', '[Elder of Junon Order] Gorthein', '1109', '2', '180', '5325', '5095');
INSERT INTO `list_npcs` VALUES ('39', '[Founder of Junon Order] Raw', '1088', '2', '180', '5319', '5095');
INSERT INTO `list_npcs` VALUES ('40', '[Guide] Alphonso', '1118', '2', '180', '5520', '5064');
INSERT INTO `list_npcs` VALUES ('41', '[Livestock Farmer] Sicru', '1107', '2', '190', '5818', '5487');
INSERT INTO `list_npcs` VALUES ('42', '[Ferrell Guild Staff] Kiroth', '1098', '2', '270', '5603', '5055');
INSERT INTO `list_npcs` VALUES ('43', '[Designer] Lisa', '1095', '2', '270', '5729', '5136');
INSERT INTO `list_npcs` VALUES ('44', '[Arumic Researcher] Carasia', '1112', '2', '190', '5730', '5093');
INSERT INTO `list_npcs` VALUES ('45', '[Event Info] Felice Fete', '1202', '2', '220', '5562', '5164');
INSERT INTO `list_npcs` VALUES ('46', '[Mayor] Darren', '1081', '2', '180', '5514', '5237');
INSERT INTO `list_npcs` VALUES ('47', '[Arumic Merchant] Chester', '1091', '2', '220', '5724', '5087');
INSERT INTO `list_npcs` VALUES ('48', '[Teleporter] Idiosel', '1119', '2', '270', '5347', '5177');
INSERT INTO `list_npcs` VALUES ('49', '[Ferrell Guild Staff] Rooen', '1513', '3', '45', '5105', '5015');
INSERT INTO `list_npcs` VALUES ('50', '[Akram Minister] Mel', '1087', '5', '270', '5250', '5279');
INSERT INTO `list_npcs` VALUES ('51', '[Akram Minister] Rodath', '1086', '5', '90', '5149', '5279');
INSERT INTO `list_npcs` VALUES ('52', '[Akram Minister] Gamp', '1084', '6', '270', '5250', '5279');
INSERT INTO `list_npcs` VALUES ('137', '[Referee] Leum', '1113', '9', '0', '5200', '5385');
INSERT INTO `list_npcs` VALUES ('138', '[Referee] Pirre', '1114', '9', '180', '5199', '4775');
INSERT INTO `list_npcs` VALUES ('53', '[Akram Minister] Nell', '1085', '11', '45', '4982', '5481');
INSERT INTO `list_npcs` VALUES ('54', '[Akram Minister] Nell', '1085', '11', '135', '4953', '4996');
INSERT INTO `list_npcs` VALUES ('55', '[Akram Minister] Nell', '1085', '11', '270', '5522', '5020');
INSERT INTO `list_npcs` VALUES ('56', '[Akram Minister] Nell', '1085', '11', '320', '5454', '5480');
INSERT INTO `list_npcs` VALUES ('139', '[Akram Minister] Nell', '1085', '12', '45', '4982', '5481');
INSERT INTO `list_npcs` VALUES ('140', '[Akram Minister] Nell', '1085', '12', '135', '4953', '4996');
INSERT INTO `list_npcs` VALUES ('141', '[Akram Minister] Nell', '1085', '12', '270', '5522', '5020');
INSERT INTO `list_npcs` VALUES ('142', '[Akram Minister] Nell', '1085', '12', '320', '5454', '5480');
INSERT INTO `list_npcs` VALUES ('143', '[Akram Minister] Nell', '1085', '13', '45', '4982', '5481');
INSERT INTO `list_npcs` VALUES ('144', '[Akram Minister] Nell', '1085', '13', '135', '4953', '4996');
INSERT INTO `list_npcs` VALUES ('145', '[Akram Minister] Nell', '1085', '13', '270', '5522', '5020');
INSERT INTO `list_npcs` VALUES ('146', '[Akram Minister] Nell', '1085', '13', '320', '5454', '5480');
INSERT INTO `list_npcs` VALUES ('152', '[Clan House Manager] Kushard', '1161', '15', '0', '5200', '5219');
INSERT INTO `list_npcs` VALUES ('153', '[Clan House Soldier] Jason', '1162', '15', '180', '5204', '5128');
INSERT INTO `list_npcs` VALUES ('154', '[Clan Clerk] Regina', '1120', '15', '180', '5196', '5128');
INSERT INTO `list_npcs` VALUES ('57', '[Arumic Resercher] Lutis', '1051', '21', '135', '5118', '5374');
INSERT INTO `list_npcs` VALUES ('58', '[Cleric] Karitte', '1053', '21', '135', '5115', '5384');
INSERT INTO `list_npcs` VALUES ('59', '[Mountain Guide] Shannon', '1052', '21', '45', '5358', '5278');
INSERT INTO `list_npcs` VALUES ('60', '[Village Chief] Gray', '1038', '22', '135', '5081', '5283');
INSERT INTO `list_npcs` VALUES ('61', '[Smith] Ronk', '1034', '22', '225', '5139', '5299');
INSERT INTO `list_npcs` VALUES ('62', '[Akram Minister] Mairard', '1032', '22', '270', '5155', '5335');
INSERT INTO `list_npcs` VALUES ('63', '[Visitor Guide] Ronway', '1031', '22', '270', '5146', '5350');
INSERT INTO `list_npcs` VALUES ('64', '[Novice Designer] Cassirin', '1040', '22', '0', '5106', '5377');
INSERT INTO `list_npcs` VALUES ('65', '[Old Man] Myad', '1041', '22', '45', '5080', '5374');
INSERT INTO `list_npcs` VALUES ('66', '[Fruit Store] Reene', '1039', '22', '45', '5061', '5347');
INSERT INTO `list_npcs` VALUES ('67', '[Visitor Guide] Arua\'s Fairy', '1030', '22', '90', '5060', '5522');
INSERT INTO `list_npcs` VALUES ('68', '[Little Street Vendor] Pony', '1035', '22', '0', '5861', '5432');
INSERT INTO `list_npcs` VALUES ('69', '[Ferrell Guild Staff] Seyon', '1036', '22', '270', '5883', '5424');
INSERT INTO `list_npcs` VALUES ('70', '[Ferrell Guild Staff] Peron', '1511', '22', '0', '5899', '5307');
INSERT INTO `list_npcs` VALUES ('149', 'Melendino', '1473', '22', '90', '5107', '5013');
INSERT INTO `list_npcs` VALUES ('150', '[Boy in the Hut] Huey', '1021', '22', '315', '5650', '5407');
INSERT INTO `list_npcs` VALUES ('151', '[Ferrell Guild Storage Keeper] Fabrizio', '1042', '22', '180', '5113', '5261');
INSERT INTO `list_npcs` VALUES ('71', '[Smith] Punwell', '1062', '23', '45', '5341', '5071');
INSERT INTO `list_npcs` VALUES ('72', '[Ferrell Guild Staff] Belz', '1061', '23', '270', '5359', '5059');
INSERT INTO `list_npcs` VALUES ('73', '[Little Street Vendor] Mile', '1063', '23', '270', '5356', '5043');
INSERT INTO `list_npcs` VALUES ('74', '[Weapon Craftsman] Mairath', '1064', '23', '180', '5349', '5034');
INSERT INTO `list_npcs` VALUES ('75', '[Ikaness Staff] Orias', '1073', '24', '125', '5518', '4924');
INSERT INTO `list_npcs` VALUES ('76', '[Gypsy Merchant] Methio', '1072', '24', '200', '5556', '4894');
INSERT INTO `list_npcs` VALUES ('77', '[Ferrell Guild Merchant] Lina', '1071', '24', '270', '5566', '4926');
INSERT INTO `list_npcs` VALUES ('78', '[Gypsy Seller] Edone', '1123', '25', '185', '5448', '4976');
INSERT INTO `list_npcs` VALUES ('79', '[Ikaness Staff] Shroon', '1121', '25', '45', '5375', '5188');
INSERT INTO `list_npcs` VALUES ('80', '[Guide of Plain] Pein', '1122', '25', '0', '5503', '5350');
INSERT INTO `list_npcs` VALUES ('81', '[Mountain Guide] Kay', '1131', '26', '310', '5671', '5107');
INSERT INTO `list_npcs` VALUES ('82', '[Ferrell Guild Staff] Robin', '1512', '27', '250', '5239', '5040');
INSERT INTO `list_npcs` VALUES ('83', '[Residents] Hotch', '1144', '27', '180', '5384', '5179');
INSERT INTO `list_npcs` VALUES ('84', '[Righteous Crusader] Gallahad', '1141', '27', '180', '5345', '5047');
INSERT INTO `list_npcs` VALUES ('85', '[Ferrell Guild Merchant ] Bith', '1143', '27', '200', '5260', '5147');
INSERT INTO `list_npcs` VALUES ('86', '[Akram Minister] Luce', '1142', '27', '90', '5319', '5081');
INSERT INTO `list_npcs` VALUES ('87', '[Ferrell Guild Merchant] Med', '1151', '28', '110', '5587', '4734');
INSERT INTO `list_npcs` VALUES ('88', '[Dead Priest] Lantore', '1154', '29', '265', '5115', '5131');
INSERT INTO `list_npcs` VALUES ('89', '[Ghost] Harry', '1157', '29', '180', '5051', '5119');
INSERT INTO `list_npcs` VALUES ('90', '[Town Girl] Lithia', '1156', '29', '45', '5091', '5182');
INSERT INTO `list_npcs` VALUES ('91', '[Ranger] Paul', '1155', '29', '330', '5157', '5176');
INSERT INTO `list_npcs` VALUES ('92', '[Event Info] Judith', '1204', '37', '0', '5112', '5322');
INSERT INTO `list_npcs` VALUES ('93', '[Event Info] Lucielle Fete', '1203', '40', '270', '5257', '5211');
INSERT INTO `list_npcs` VALUES ('94', '[Magic Goods Seller] Pabel', '1185', '51', '0', '5298', '5105');
INSERT INTO `list_npcs` VALUES ('95', '[Tavern Owner] Anzhelika', '1186', '51', '90', '5264', '5083');
INSERT INTO `list_npcs` VALUES ('96', '[Arumic Prophet] Olleck Basilasi', '1173', '51', '180', '5358', '5086');
INSERT INTO `list_npcs` VALUES ('97', '[Ferrell Guild Banker] Andre', '1180', '51', '0', '5414', '5113');
INSERT INTO `list_npcs` VALUES ('98', '[Smith] Pavrick', '1181', '51', '320', '5440', '5099');
INSERT INTO `list_npcs` VALUES ('99', '[Ferrell Trader] Sergei', '1184', '51', '270', '5447', '5080');
INSERT INTO `list_npcs` VALUES ('100', '[Eucar Judge] Ishtal', '1171', '51', '225', '5432', '4940');
INSERT INTO `list_npcs` VALUES ('101', '[Akram Ambassador] Eliot', '1172', '51', '160', '5286', '4941');
INSERT INTO `list_npcs` VALUES ('102', '[Station Guide] Illiya', '1188', '51', '90', '5104', '5000');
INSERT INTO `list_npcs` VALUES ('103', '[Shamanist] Est', '1191', '54', '90', '5088', '4257');
INSERT INTO `list_npcs` VALUES ('104', '[Event Info] Loelsch', '1206', '38', '45', '5059', '5353');
INSERT INTO `list_npcs` VALUES ('105', '[Event] Santa Claus', '1502', '38', '0', '5073', '5362');
INSERT INTO `list_npcs` VALUES ('106', '[Raknu Resident] Jerrita', '1236', '61', '270', '5659', '4457');
INSERT INTO `list_npcs` VALUES ('107', '[Chef] Peppie', '1224', '61', '180', '5628', '4436');
INSERT INTO `list_npcs` VALUES ('108', '[Station Guide] Chacha', '1219', '61', '45', '5226', '4454');
INSERT INTO `list_npcs` VALUES ('109', '[Elder of Junon Order] Oscar Patrick', '1214', '61', '120', '5368', '4544');
INSERT INTO `list_npcs` VALUES ('110', '[Ferrell Guild Staff] Gilbert', '1213', '61', '260', '5473', '4514');
INSERT INTO `list_npcs` VALUES ('111', '[Raknu Chief] Darka Khan', '1220', '61', '310', '5647', '4462');
INSERT INTO `list_npcs` VALUES ('112', '[Patrol Dog] Stephen', '1244', '61', '180', '5426', '4485');
INSERT INTO `list_npcs` VALUES ('113', '[Akram Ambassador] Jacklyn Cooper', '1215', '61', '180', '5429', '4485');
INSERT INTO `list_npcs` VALUES ('114', '[Patrol Dog] Max', '1243', '61', '210', '5489', '4550');
INSERT INTO `list_npcs` VALUES ('115', '[Storage Keeper] Dustin Leta', '1222', '61', '210', '5491', '4552');
INSERT INTO `list_npcs` VALUES ('116', '[Raknu Resident] Netty', '1237', '61', '315', '5496', '4597');
INSERT INTO `list_npcs` VALUES ('117', '[Arumic Researcher] Catherine Clara', '1212', '61', '315', '5475', '4619');
INSERT INTO `list_npcs` VALUES ('118', '[Raknu Warrior] Jeffrey Lloyd', '1221', '61', '180', '5436', '4593');
INSERT INTO `list_npcs` VALUES ('119', '[Smith] Nel Eldora', '1223', '61', '45', '5395', '4617');
INSERT INTO `list_npcs` VALUES ('120', '[Righteous Crusader] Harold Evan', '1211', '61', '90', '5367', '4577');
INSERT INTO `list_npcs` VALUES ('121', '[Raknu Warrior] Guanu', '1230', '61', '125', '5421', '4673');
INSERT INTO `list_npcs` VALUES ('122', '[Raknu Warrior] Toanu', '1229', '61', '210', '5438', '4674');
INSERT INTO `list_npcs` VALUES ('123', '[Sikuku Resident] Martie', '1258', '62', '45', '5638', '5127');
INSERT INTO `list_npcs` VALUES ('124', '[Sikuku Warrior] Kilie', '1257', '62', '315', '5647', '5126');
INSERT INTO `list_npcs` VALUES ('125', '[Sikuku Resident] Carl', '1259', '62', '100', '5714', '5122');
INSERT INTO `list_npcs` VALUES ('126', '[Sikuku Resident] Parah', '1260', '62', '225', '5721', '5118');
INSERT INTO `list_npcs` VALUES ('127', '[Sikuku Warrior] Ruduck', '1254', '62', '45', '5778', '5040');
INSERT INTO `list_npcs` VALUES ('128', '[Sikuku Warrior] Yak', '1255', '62', '45', '5789', '5051');
INSERT INTO `list_npcs` VALUES ('129', '[Sikuku Chief] Namiel Char', '1252', '62', '225', '5896', '5129');
INSERT INTO `list_npcs` VALUES ('130', '[Bird] Hawker', '1253', '62', '230', '5901', '5130');
INSERT INTO `list_npcs` VALUES ('131', '[Sikuku Warrior] Seka', '1256', '62', '180', '5817', '5173');
INSERT INTO `list_npcs` VALUES ('132', '[Critically Wounded] Nukie', '1266', '62', '220', '5858', '5208');
INSERT INTO `list_npcs` VALUES ('133', '[Cleric] Jude', '1251', '62', '315', '5851', '5209');
INSERT INTO `list_npcs` VALUES ('134', '[Sikuku Resident] Shilma', '1261', '62', '315', '5854', '5224');
INSERT INTO `list_npcs` VALUES ('135', '[Sikuku Warrior] Wounded Soldier', '1262', '62', '230', '5865', '5211');
INSERT INTO `list_npcs` VALUES ('156', '[Sikuku Warrior] Wounded Soldier', '1263', '62', '315', '5862', '5218');
