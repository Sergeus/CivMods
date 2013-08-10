CREATE TABLE NotificationIconTypes
(
	ID integer PRIMARY KEY AUTOINCREMENT,
	Type text NOT null
);

INSERT INTO NotificationIconTypes (ID, Type)
VALUES (0, 'ICON_TYPE_NONE');

INSERT INTO NotificationIconTypes (Type)
VALUES ('ICON_TYPE_UNIT'); 
INSERT INTO NotificationIconTypes (Type)
VALUES ('ICON_TYPE_BUILDING');
INSERT INTO NotificationIconTypes (Type)
VALUES ('ICON_TYPE_PROJECT');
INSERT INTO NotificationIconTypes (Type)
VALUES ('ICON_TYPE_TECH');

CREATE TABLE TempNotifications
(
	ID integer,
	Type text,
	Welcomeness integer,
	Urgent boolean
);

INSERT INTO TempNotifications (ID, Type, Welcomeness, Urgent)
SELECT ID, NotificationType, Welcomeness, Urgent FROM Notifications;

DROP TABLE Notifications;

CREATE TABLE Notifications
(
	ID integer PRIMARY KEY AUTOINCREMENT,
	Type text NOT null,
	Welcomeness integer DEFAULT 0,
	Urgent boolean DEFAULT false,
	Message text DEFAULT null REFERENCES Language_en_US(Tag),
	Summary text DEFAULT null REFERENCES Language_en_US(Tag),
	IconIndex integer DEFAULT -1,
	IconAtlas text DEFAULT null REFERENCES IconTextureAtlases(Atlas),
	IconSize integer DEFAULT 80,
	BlocksEndTurn boolean DEFAULT false,
	BlockMessage text DEFAULT null REFERENCES Language_en_US(Tag),
	BlockToolTip text DEFAULT null REFERENCES Language_en_US(Tag),
	LargeButton boolean DEFAULT false,
	MiniCivIcon boolean DEFAULT false,
	ChecksKnown boolean DEFAULT false,
	UnknownMessage text DEFAULT null REFERENCES Language_en_US(Tag),
	UsesRuntimeIndex boolean DEFAULT false,
	ExistingIconType text DEFAULT null REFERENCES NotificationIconTypes(Type),
	DoubleCivIcon boolean DEFAULT false,
	Civ1Anchor text DEFAULT "L,B",
	Civ2Anchor text DEFAULT "R,T",
	UIContext text DEFAULT null,
	ExpiresAtTurnEnd boolean DEFAULT true,
	PlaysFXOnPlot boolean DEFAULT true
);

INSERT INTO Notifications (ID, Type, Welcomeness, Urgent)
SELECT ID, Type, Welcomeness, Urgent FROM TempNotifications;

--INSERT INTO Notifications (Type) NOTIFICATION_REQUEST_RESOURCE;
INSERT INTO Notifications (Type) 
VALUES ('NOTIFICATION_LIBERATED_MAJOR_CITY');
INSERT INTO Notifications (Type) 
VALUES ('NOTIFICATION_RESURRECTED_MAJOR_CIV');
INSERT INTO Notifications (Type)
VALUES ('NOTIFICATION_PLACEHOLDER_1');
INSERT INTO Notifications (Type)
VALUES ('NOTIFICATION_PLACEHOLDER_1');