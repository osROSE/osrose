/*
MySQL Data Transfer
Source Host: 192.168.1.100
Source Database: osrose
Target Host: 192.168.1.100
Target Database: osrose
Date: 9/23/2007 8:55:57 PM
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for chest_data
-- ----------------------------
CREATE TABLE `chest_data` (
  `id` int(11) NOT NULL auto_increment,
  `chestid` int(11) NOT NULL,
  `reward` varchar(255) NOT NULL,
  `rewardtype` varchar(255) NOT NULL,
  `prob` varchar(255) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `chest_data` VALUES ('1', '1001', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('2', '1002', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('3', '1003', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('4', '1004', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('5', '1005', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('6', '1006', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('7', '1007', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('8', '1008', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('9', '1009', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('10', '1010', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('11', '1011', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('12', '1012', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('13', '1013', '109|125', '10|10', '1000|500');
INSERT INTO `chest_data` VALUES ('15', '1014', '72|250', '2|8', '1000|500');
INSERT INTO `chest_data` VALUES ('16', '1015', '103|480', '3|4', '1000|500');
INSERT INTO `chest_data` VALUES ('17', '1016', '133|151', '4|8', '1000|500');
INSERT INTO `chest_data` VALUES ('18', '1017', '43|680', '5|5', '1000|500');
INSERT INTO `chest_data` VALUES ('19', '1018', '580|87', '2|12', '1000|500');
INSERT INTO `chest_data` VALUES ('20', '1019', '103|481', '3|4', '1000|500');
INSERT INTO `chest_data` VALUES ('21', '1020', '133|681', '4|5', '1000|500');
INSERT INTO `chest_data` VALUES ('22', '1021', '73|133', '5|5', '1000|500');
INSERT INTO `chest_data` VALUES ('23', '1022', '780|781', '2|2', '1000|500');
INSERT INTO `chest_data` VALUES ('24', '1023', '581|680', '3|3', '1000|500');
INSERT INTO `chest_data` VALUES ('25', '1024', '73|581', '4|4', '1000|500');
INSERT INTO `chest_data` VALUES ('26', '1025', '133|43', '5|5', '1000|500');
INSERT INTO `chest_data` VALUES ('27', '1026', '77|250', '12|8', '1000|500');
INSERT INTO `chest_data` VALUES ('28', '1027', '87|77', '12|12', '1000|500');
INSERT INTO `chest_data` VALUES ('29', '1028', '87|77', '12|12', '1000|500');
