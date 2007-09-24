-- ------------------------------
-- Table structure for list_quest
-- ------------------------------
CREATE TABLE IF NOT EXISTS  `list_quest` (
	`owner` int(11) NOT NULL,
	`questid` int(11) NOT NULL,
	`nitems` varchar(255) NOT NULL,
	`active` tinyint(1) NOT NULL
	) ENGINE=InnoDB DEFAULT CHARSET=latin1;
