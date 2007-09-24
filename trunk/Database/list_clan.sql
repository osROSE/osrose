-- -----------------------------
-- Table structure for list_clan
-- -----------------------------
CREATE TABLE IF NOT EXISTS list_clan (
	id int(11) NOT NULL auto_increment,
	logo int(11) default NULL,
	back int(11) default NULL,
	name varchar(50) default NULL,
	cp int(11) default NULL,
	grade int(11) default NULL,
	slogan varchar(100) default NULL,
	news varchar(200) default NULL,
	  PRIMARY KEY  (id)
	) ENGINE=InnoDB AUTO_INCREMENT=74 DEFAULT CHARSET=latin1;
