DROP TABLE IF EXISTS `user_table`;
CREATE TABLE IF NOT EXISTS `user_table` (
	`id`	INTEGER PRIMARY KEY,
	`passw` TEXT,
	`name`	TEXT,
	`icon`	TEXT
);

DROP TABLE IF EXISTS `invite_code_table`;
CREATE TABLE IF NOT EXISTS `invite_code_table` (
	`id`	INTEGER PRIMARY KEY,
	`code`	TEXT,
	`times`	INTEGER
);
INSERT INTO `invite_code_table` VALUES (1,'522522',0);