BEGIN TRANSACTION;

INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Skeleton', 0);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('HD1', 0);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Zombie', 1);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Ghoul', 2);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('HD2', 2);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Shadow', 3);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('HD3', 3);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('HD4', 3);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Wight', 4);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('HD5', 4);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Ghast', 5);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Wraith', 6);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('HD6', 6);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Mummy', 7);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('HD7', 7);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Spectre', 8);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('HD8', 8);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Vampire', 9);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('HD9', 9);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Ghost', 10);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('HD10', 10);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Lich', 11);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('HD11+', 11);
INSERT INTO TURNABLE ([NAME], [SCORE]) VALUES ('Special', 12);

COMMIT TRANSACTION;

-------------------------------------------------------------------------------------------------------------

BEGIN TRANSACTION;

INSERT INTO TURN_EFFECT ([ID], [NAME], [ACRONYM]) VALUES (0, 'Percentage', NULL);
INSERT INTO TURN_EFFECT ([ID], [NAME], [ACRONYM]) VALUES (1, 'Turn', 'T');
INSERT INTO TURN_EFFECT ([ID], [NAME], [ACRONYM]) VALUES (2, 'Destroy', 'D');
INSERT INTO TURN_EFFECT ([ID], [NAME], [ACRONYM]) VALUES (3, 'Destroy more', 'D*');
INSERT INTO TURN_EFFECT ([ID], [NAME], [ACRONYM]) VALUES (4, 'No effect', '-');

COMMIT TRANSACTION;

-------------------------------------------------------------------------------------------------------------

BEGIN TRANSACTION;

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  1, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  1, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  1, 0, 16);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  1, 0, 19);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  1, 0, 20);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  1, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  1, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  1, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  1, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  1, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 1, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 1, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 1, 4, NULL);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  2, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  2, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  2, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  2, 0, 16);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  2, 0, 19);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  2, 0, 20);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  2, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  2, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  2, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  2, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 2, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 2, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 2, 4, NULL);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  3, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  3, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  3, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  3, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  3, 0, 16);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  3, 0, 19);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  3, 0, 20);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  3, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  3, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  3, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 3, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 3, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 3, 4, NULL);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  4, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  4, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  4, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  4, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  4, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  4, 0, 16);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  4, 0, 19);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  4, 0, 20);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  4, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  4, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 4, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 4, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 4, 4, NULL);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  5, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  5, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  5, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  5, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  5, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  5, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  5, 0, 16);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  5, 0, 19);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  5, 0, 20);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  5, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 5, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 5, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 5, 4, NULL);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  6, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  6, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  6, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  6, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  6, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  6, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  6, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  6, 0, 16);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  6, 0, 19);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  6, 0, 20);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 6, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 6, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 6, 4, NULL);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  7, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  7, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  7, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  7, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  7, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  7, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  7, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  7, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  7, 0, 16);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  7, 0, 19);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 7, 0, 20);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 7, 4, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 7, 4, NULL);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  8, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  8, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  8, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  8, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  8, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  8, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  8, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  8, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  8, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  8, 0, 16);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 8, 0, 19);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 8, 0, 20);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 8, 4, NULL);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  9, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  9, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  9, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  9, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  9, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  9, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  9, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  9, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  9, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  9, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 9, 0, 16);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 9, 0, 19);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 9, 0, 20);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  10, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  10, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  10, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  10, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  10, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  10, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  10, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  10, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  10, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  10, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 10, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 10, 0, 16);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 10, 0, 19);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  11, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  11, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  11, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  11, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  11, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  11, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  11, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  11, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  11, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  11, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 11, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 11, 0, 16);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 11, 0, 19);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  12, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  12, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  12, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  12, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  12, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  12, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  12, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  12, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  12, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  12, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 12, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 12, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 12, 0, 16);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  13, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  13, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  13, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  13, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  13, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  13, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  13, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  13, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  13, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  13, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 13, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 13, 0, 13);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 13, 0, 16);

INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (0,  14, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (1,  14, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (2,  14, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (3,  14, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (4,  14, 3, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (5,  14, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (6,  14, 2, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (7,  14, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (8,  14, 1, NULL);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (9,  14, 0, 4);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (10, 14, 0, 7);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (11, 14, 0, 10);
INSERT INTO TURN_UNDEAD ([TURNABLE_CODE], [LEVEL], [EFFECT], [VALUE]) VALUES (12, 14, 0, 13);

COMMIT TRANSACTION;

-------------------------------------------------------------------------------------------------------------

