-- ---------------------------
-- Table structure for storage
-- ---------------------------
CREATE TABLE IF NOT EXISTS  `storage` (
	`owner` int(11) NOT NULL,
	`itemnum` int(11) NOT NULL,
	`itemtype` int(11) NOT NULL,
	`refine` int(11) NOT NULL,
	`durability` int(11) NOT NULL default '40',
	`lifespan` int(11) NOT NULL default '100',
	`slotnum` int(11) NOT NULL,
	`count` int(11) NOT NULL default '1',
	`stats` int(11) NOT NULL default '0',
	`socketed` int(11) NOT NULL default '0',
	`appraised` int(11) NOT NULL default '0',
	`gem` int(11) NOT NULL default '0'
	) ENGINE=InnoDB DEFAULT CHARSET=latin1;
