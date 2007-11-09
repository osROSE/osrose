/*
MySQL Data Transfer
Source Host: localhost
Source Database: osrose
Target Host: localhost
Target Database: osrose
Date: 11/8/2007 10:30:43 AM
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for list_respawnzones
-- ----------------------------
CREATE TABLE `list_respawnzones` (
  `id` int(11) NOT NULL auto_increment,
  `map` int(11) NOT NULL,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `radius` float NOT NULL default '10',
  `type` int(11) NOT NULL default '2',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=91 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `list_respawnzones` VALUES ('1', '22', '5098', '5322', '10', '1');
INSERT INTO `list_respawnzones` VALUES ('2', '1', '5239', '5192', '10', '1');
INSERT INTO `list_respawnzones` VALUES ('3', '24', '5537', '4966', '10', '1');
INSERT INTO `list_respawnzones` VALUES ('4', '2', '5653', '5214', '10', '1');
INSERT INTO `list_respawnzones` VALUES ('5', '51', '5357', '5013', '10', '1');
INSERT INTO `list_respawnzones` VALUES ('6', '22', '5026', '5522', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('7', '22', '5232', '5246', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('8', '22', '5073', '5019', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('9', '22', '5645', '5394', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('10', '22', '5851', '5422', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('11', '21', '5083', '5157', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('12', '21', '5142', '5506', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('13', '21', '5343', '5281', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('14', '23', '5345', '5056', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('15', '23', '5338', '5386', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('19', '24', '5027', '5090', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('20', '24', '5265', '5521', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('21', '24', '5523', '5378', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('22', '25', '5128', '5413', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('23', '25', '5489', '5357', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('24', '25', '5242', '5092', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('25', '25', '5555', '4970', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('27', '2', '5514', '5453', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('28', '2', '5048', '5451', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('29', '29', '5159', '5165', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('30', '29', '5067', '5357', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('31', '29', '5568', '5160', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('32', '29', '5823', '5098', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('33', '29', '5664', '4893', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('34', '26', '5146', '5248', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('35', '26', '5127', '5477', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('36', '26', '5429', '5272', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('37', '26', '5681', '5104', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('38', '26', '5288', '4890', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('39', '27', '5219', '5328', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('40', '27', '5588', '5462', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('41', '28', '5189', '5036', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('42', '28', '5639', '4761', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('43', '28', '5568', '5121', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('44', '28', '5541', '5364', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('45', '28', '5232', '5344', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('46', '51', '5357', '5013', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('47', '51', '5638', '5032', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('48', '51', '5324', '5343', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('49', '51', '5321', '4707', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('50', '52', '5132', '4603', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('51', '52', '5181', '4891', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('52', '52', '5698', '5295', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('53', '52', '6050', '5020', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('54', '52', '5622', '4866', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('55', '52', '5533', '4555', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('56', '22', '5025', '5522', '10', '1');
INSERT INTO `list_respawnzones` VALUES ('57', '6', '5200', '5056', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('58', '58', '5700', '4330', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('59', '31', '5430', '5508', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('60', '32', '5253', '5415', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('61', '33', '5065', '5000', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('62', '33', '5695', '5269', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('63', '53', '5040', '5200', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('64', '54', '5080', '4420', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('65', '55', '5520', '4480', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('66', '56', '5035', '4275', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('67', '57', '5430', '5355', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('68', '61', '5435', '4595', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('69', '62', '5750', '5100', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('70', '63', '6075', '5185', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('71', '64', '5035', '5325', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('72', '65', '5485', '5285', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('73', '11', '5000', '5047', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('74', '12', '5000', '5047', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('75', '13', '5000', '5047', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('76', '59', '5102', '5132', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('77', '9', '5201', '4861', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('78', '8', '5160', '5078', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('79', '4', '5197', '5042', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('80', '37', '5117', '5303', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('81', '38', '5072', '5345', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('82', '3', '5198', '5010', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('83', '5', '5201', '5277', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('84', '15', '5200', '5200', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('85', '40', '5184', '5211', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('86', '41', '5050', '5250', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('87', '101', '5284', '5038', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('88', '74', '5161', '5302', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('89', '70', '5301', '5039', '10', '2');
INSERT INTO `list_respawnzones` VALUES ('90', '6', '5200', '5501', '10', '2');
