BEGIN TRANSACTION;

INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (1, 'Forgotten Realms');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (2, 'Ravenloft');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (3, 'Dark Sun');
INSERT INTO CAMPAIGN_SETTINGS ([ID], [DESCRIPTION]) VALUES (4, 'Greyhawk');

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

