-- ----------------------------------
-- Table structure for list_telegates
-- ----------------------------------
DROP TABLE IF EXISTS `list_telegates`;
CREATE TABLE `list_telegates` (
	`id` int(11) NOT NULL,
	`name` varchar(255) NOT NULL default '',
	`map` int(11) NOT NULL,
	`x` float NOT NULL,
	`y` float NOT NULL,
	  PRIMARY KEY  (`id`)
	) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- -------------------------------
-- Records list_telegates / rl2171
-- -------------------------------
INSERT INTO `list_telegates` VALUES ('17', 'Adventure Plains (NW) > Valley of Luxem Tower (SW)', '21', '5098', '5051');
INSERT INTO `list_telegates` VALUES ('12', 'Valley of Luxem Tower (SW) > Adventure Plains (NW)', '22', '5846', '5539');
INSERT INTO `list_telegates` VALUES ('19', 'Adventure Plains (NE) > Valley of Luxem Tower (SE)', '21', '5295', '5058');
INSERT INTO `list_telegates` VALUES ('14', 'Valley of Luxem Tower (SE) > Adventure Plains (NE)', '22', '5972', '5551');
INSERT INTO `list_telegates` VALUES ('18', 'Adventure Plains > Goblin Cave B1', '31', '5516', '5437');
INSERT INTO `list_telegates` VALUES ('61', 'Goblin Cave B1 > Adventure Plains', '22', '5414', '5051');
INSERT INTO `list_telegates` VALUES ('65', 'Goblin Cave B1 (End) > B2', '31', '5384', '5228');
INSERT INTO `list_telegates` VALUES ('63', 'Goblin Cave B2 > B1 (End)', '32', '5524', '5377');
INSERT INTO `list_telegates` VALUES ('62', 'Goblin Cave B1 > B2', '32', '5435', '5258');
INSERT INTO `list_telegates` VALUES ('64', 'Goblin Cave B2 > B1', '31', '5294', '5339');
INSERT INTO `list_telegates` VALUES ('67', 'Goblin Cave B2 > B3', '33', '5155', '5206');
INSERT INTO `list_telegates` VALUES ('69', 'Goblin Cave B3 > B2', '32', '5119', '5064');
INSERT INTO `list_telegates` VALUES ('66', 'Goblin Cave B2 > B3', '33', '5605', '5488');
INSERT INTO `list_telegates` VALUES ('68', 'Goblin Cave B3 > B2', '32', '5434', '5200');
INSERT INTO `list_telegates` VALUES ('13', 'Valley of Luxem Tower > Breezy Hills', '23', '5049', '4964');
INSERT INTO `list_telegates` VALUES ('26', 'Breezy Hills > Valley of Luxem Tower', '21', '5510', '5334');
INSERT INTO `list_telegates` VALUES ('11', 'Valley of Luxem Tower > Canyon City of Zant', '1', '5249', '5025');
INSERT INTO `list_telegates` VALUES ('21', 'Canyon City of Zant > Valley of Luxem Tower', '21', '5144', '5505');
INSERT INTO `list_telegates` VALUES ('23', 'Canyon City of Zant > Breezy Hills', '23', '5039', '5207');
INSERT INTO `list_telegates` VALUES ('28', 'Breezy Hills > Canyon City of Zant', '1', '5510', '5177');
INSERT INTO `list_telegates` VALUES ('22', 'Canyon City of Zant > El Verloon Desert', '24', '5162', '4871');
INSERT INTO `list_telegates` VALUES ('31', 'El Verloon Desert > Canyon City of Zant', '1', '5284', '5509');
INSERT INTO `list_telegates` VALUES ('32', 'El Verloon Desert > Breezy Hills', '23', '5065', '5538');
INSERT INTO `list_telegates` VALUES ('27', 'Breezy Hills > El Verloon Desert', '24', '5386', '4891');
INSERT INTO `list_telegates` VALUES ('33', 'El Verloon Desert > Anima Lake', '25', '5021', '5424');
INSERT INTO `list_telegates` VALUES ('36', 'Anima Lake > El Verloon Desert', '24', '5709', '5497');
INSERT INTO `list_telegates` VALUES ('38', 'Anima Lake > Junon Polis', '2', '5614', '5542');
INSERT INTO `list_telegates` VALUES ('56', 'Junon Polis > Anima Lake', '25', '5403', '4958');
INSERT INTO `list_telegates` VALUES ('40', 'Anima Lake > Desert of the Dead', '29', '5081', '5010');
INSERT INTO `list_telegates` VALUES ('43', 'Desert of the Dead > Anima Lake', '25', '5508', '5537');
INSERT INTO `list_telegates` VALUES ('39', 'Anima Lake > Forest of Wisdom', '26', '5050', '5504');
INSERT INTO `list_telegates` VALUES ('41', 'Forest of Wisdom > Anima Lake', '25', '5650', '5238');
INSERT INTO `list_telegates` VALUES ('42', 'Forest of Wisdom > Kenji Beach', '27', '5155', '5489');
INSERT INTO `list_telegates` VALUES ('46', 'Kenji Beach > Forest of Wisdom', '26', '5282', '4884');
INSERT INTO `list_telegates` VALUES ('47', 'Kenji Beach > Gorge of Silence', '28', '5474', '4765');
INSERT INTO `list_telegates` VALUES ('51', 'Gorge of Silence > Kenji Beach', '27', '5768', '5485');
INSERT INTO `list_telegates` VALUES ('70', 'Sunshine Coast > Canyon City of Zant', '1', '5242', '5193');
INSERT INTO `list_telegates` VALUES ('71', 'Grand Ballroom > Junon Polis', '2', '5548', '5184');
INSERT INTO `list_telegates` VALUES ('104', 'Magic City of Eucar > Freezing Plateau', '55', '5522', '4479');
INSERT INTO `list_telegates` VALUES ('115', 'Freezing Plateau > Magic City of Eucar', '51', '5088', '5351');
INSERT INTO `list_telegates` VALUES ('103', 'Magic City of Eucar > Crystal Snowfields', '54', '5058', '4423');
INSERT INTO `list_telegates` VALUES ('112', 'Crystal Snowfields > Magic City of Eucar', '51', '5672', '5235');
INSERT INTO `list_telegates` VALUES ('113', 'Crystal Snowfields > Freezing Plateau', '55', '5807', '4685');
INSERT INTO `list_telegates` VALUES ('116', 'Freezing Plateau > Crystal Snowfields', '54', '5098', '4823');
INSERT INTO `list_telegates` VALUES ('86', 'Crystal Snowfields > Temple B1', '56', '5035', '4272');
INSERT INTO `list_telegates` VALUES ('120', 'Forgotten Temple B1 > Crystal Snowfields', '54', '5092', '4245');
INSERT INTO `list_telegates` VALUES ('87', 'Forgotten Temple B1L > B1North', '56', '5664', '4925');
INSERT INTO `list_telegates` VALUES ('88', 'Forgotten Temple B1North > B1l', '56', '5693', '4384');
INSERT INTO `list_telegates` VALUES ('92', 'Forgotten Temple B1Sister > B1FarLeft', '56', '5034', '4536');
INSERT INTO `list_telegates` VALUES ('91', 'Forgotten Temple B1FarLeft > B1Sister', '56', '5500', '4564');
INSERT INTO `list_telegates` VALUES ('89', 'Forgotten Temple B1Middle > B1TopLeft', '56', '5125', '5302');
INSERT INTO `list_telegates` VALUES ('90', 'Forgotten Temple B1TopLeft > B1Middle', '56', '5215', '4630');
INSERT INTO `list_telegates` VALUES ('95', 'Forgotten Temple B1 > B2', '57', '5539', '5155');
INSERT INTO `list_telegates` VALUES ('96', 'Forgotten Temple B2 > B1', '56', '5034', '5229');
INSERT INTO `list_telegates` VALUES ('93', 'Forgotten Temple B2Large > B2Small', '57', '5439', '5354');
INSERT INTO `list_telegates` VALUES ('94', 'Forgotten Temple B2Small > B2Large', '57', '5765', '5183');
INSERT INTO `list_telegates` VALUES ('101', 'Magic City of Eucar > Mana Snowfields', '52', '6152', '5209');
INSERT INTO `list_telegates` VALUES ('106', 'Mana Snowfields > M.C. Eucar', '51', '5040', '4849');
INSERT INTO `list_telegates` VALUES ('107', 'Mana Snowfields > Arumic Valley', '53', '5041', '5203');
INSERT INTO `list_telegates` VALUES ('110', 'Arumic Valley > Mana Snowfields', '52', '5698', '4692');
INSERT INTO `list_telegates` VALUES ('102', 'Magic City of Eucar > Arumic Valley', '53', '6161', '5373');
INSERT INTO `list_telegates` VALUES ('109', 'Arumic Valley > M.C. Eucar', '51', '5446', '4716');
INSERT INTO `list_telegates` VALUES ('123', 'Luna Clan Field > Crystal Snowfields', '54', '5094', '4254');
INSERT INTO `list_telegates` VALUES ('117', 'Mount Eruca > Magic City of Eucar', '51', '5004', '5278');
INSERT INTO `list_telegates` VALUES ('105', 'Magic City of Eucar > Mount Eruca', '58', '5680', '4287');
INSERT INTO `list_telegates` VALUES ('126', 'Xita Refuge (NW) > Shady Jungle (SW)', '62', '5683', '4479');
INSERT INTO `list_telegates` VALUES ('128', 'Shady Jungle (SW) > Xita Refuge (NW)', '61', '5218', '5381');
INSERT INTO `list_telegates` VALUES ('127', 'Xita Refuge (NE) > Shady Jungle (SE)', '62', '6326', '4429');
INSERT INTO `list_telegates` VALUES ('129', 'Shady Jungle (SE) > Xita Refuge (NE)', '61', '5799', '5383');
INSERT INTO `list_telegates` VALUES ('130', 'Shady Jungle > Forest of Wandering', '63', '6088', '5201');
INSERT INTO `list_telegates` VALUES ('131', 'Forest of Wandering > Shady Jungle', '62', '5086', '5268');
INSERT INTO `list_telegates` VALUES ('132', 'Shady Jungle > Marsh of Ghosts', '64', '5039', '5326');
INSERT INTO `list_telegates` VALUES ('133', 'Marsh of Ghosts > Shady Jungle', '62', '6543', '5247');
INSERT INTO `list_telegates` VALUES ('134', 'Forest of Wandering > Sikuku Underground Prison', '65', '5511', '5395');
INSERT INTO `list_telegates` VALUES ('135', 'Sikuku Underground Prison > Forest of Wandering', '63', '5403', '4437');
