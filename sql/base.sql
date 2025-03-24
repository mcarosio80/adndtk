BEGIN TRANSACTION;

INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (1, 'Forgotten Realms');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (2, 'Ravenloft');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (3, 'Dark Sun');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (4, 'Greyhawk');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (5, 'Spelljammer');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (6, 'Planescape');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (7, 'Mystara');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (8, 'Al-Qadim (Forgotten Realms)');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (9, 'Maztica (Forgotten Realms)');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (10, 'Dragonlance');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (11, 'Red Steel (Mystara)');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (12, 'Lankhmar');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (13, 'Birthright');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (14, 'Kara-Tur (Forgotten Realms)');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (15, 'Masque of the Red Death (Ravenloft)');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (16, 'The Horde (Forgotten Realms)');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (17, 'Alternity');

COMMIT TRANSACTION;

-------------------------------------------------------------------------------------------------------------

BEGIN TRANSACTION;

INSERT INTO [EDITION] ([ACRONYM], [DESCRIPTION]) VALUES ('Adnd2.0', 'Advanced Dungeons & Dragons 2nd Edition');

COMMIT TRANSACTION;

-------------------------------------------------------------------------------------------------------------

BEGIN TRANSACTION;

INSERT INTO [DIE] ([ID], [ACRONYM], [DESCRIPTION]) VALUES (4, 'd4', 'Four-faced die');
INSERT INTO [DIE] ([ID], [ACRONYM], [DESCRIPTION]) VALUES (6, 'd6', 'Six-faced die');
INSERT INTO [DIE] ([ID], [ACRONYM], [DESCRIPTION]) VALUES (8, 'd8', 'Eight-faced die');
INSERT INTO [DIE] ([ID], [ACRONYM], [DESCRIPTION]) VALUES (10, 'd10', 'Ten-faced die');
INSERT INTO [DIE] ([ID], [ACRONYM], [DESCRIPTION]) VALUES (12, 'd12', 'Twelve-faced die');
INSERT INTO [DIE] ([ID], [ACRONYM], [DESCRIPTION]) VALUES (20, 'd20', 'Twenty-faced die');
INSERT INTO [DIE] ([ID], [ACRONYM], [DESCRIPTION]) VALUES (100, 'd100', 'One hundred-faced die');

COMMIT TRANSACTION;

-------------------------------------------------------------------------------------------------------------

