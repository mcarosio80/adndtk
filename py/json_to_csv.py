import sys
import json
import getopt
import os
import re
from sys import argv

import common

fieldNames = ['Diet','Frequency','Hit Dice','Intelligence','Magic Resistance',
                'Morale','Movement','No. Appearing','No. of Attacks','Organization','Size',
                'Special Attacks','Special Defenses','THAC0','Treasure','XP Value',
                "Alignment" , "Armor Class", "Climate/Terrain", "Damage/Attack"]


################################
def get_monsters_list(jsonFile, source = "", limit = 0):
    with open(jsonFile) as jsonInput:
        monstersData = json.load(jsonInput)

        monstersList = []
        for md in monstersData:
            monster = md['monster_data']

            if 'statblock' not in monster.keys():
                continue

            monsterInfo = monster['statblock']
            if monsterInfo == None:
                continue

            tsrIssues = monster['TSR']
            if source != "" and source not in tsrIssues:
                continue

            title = monster['title']
            mainTitle = md['title']
            monsterKey = md["monster_key"]
            setting = monster['setting']

            monsterVariants = monsterInfo.keys()
            
            for variantName in monsterVariants:
                statBlock = monsterInfo[variantName]

                fields = dict()
                fields['TSR'] = tsrIssues
                fields['monsterKey'] = monsterKey
                fields['setting'] = setting
                fields['mainTitle'] = mainTitle
                fields['title'] = title
                fields['name'] = variantName
                
                for fieldName in fieldNames:
                    fields[fieldName] = common.read_dictionary_field(statBlock, fieldName, '')

                monstersList.append(fields)

            if limit > 0 and len(monstersList) >= limit:
                return monstersList
    
    return monstersList


################################
def generate_csv(csvFile, monsters, separator = '|', useApices = False):
    firstOrNone = monsters[0] if len(monsters) > 0 else None

    with open(csvFile, 'w') as outFile:
        common.write_csv_caption(outFile, firstOrNone, useApices, separator)
        for monster in monsters:
            common.write_csv_row(outFile, monster, useApices, separator)


################################
def parse_treasures(treasure):
    print(f"treasure : {treasure}")

    if treasure[0] in [chr(i) for i in range(ord('A'),ord('Z')+1)]:
        return treasure[0]


################################
def match_neutral(align):
    if align == 'neutral':
        return ['true neutral']
    return None


################################
def match_any_wildcard(align, moralOrder, moralView):
    match = re.match(r"Any (.*)", align)
    aligns = list()
    if match:
        if match.group(1) in moralOrder.keys():
            for view in moralView.keys():
                aligns.append(f"{match.group(1)} {view}".lower())
        elif match.group(1) in moralView.keys():
            for order in moralOrder.keys():
                aligns.append(f"{order} {match.group(1)}".lower())
    else:
        return None

    return aligns


################################
def match_parenthesys(align, moralOrder, moralView):
    match = re.match(r"(.*) \((.*)\)", align)
    aligns = list()
    if match:
        primary = match_neutral(match.group(1))
        aligns.append(primary[0])
        secondary = match_neutral(match.group(2))
        if secondary is None:
            secondary = match.group(2)
        aligns.append(match.group(2))
    else:
        return None
    
    return aligns


################################
def generate_alignment_features(sqlDatabase, sql_out_dir, monsters, separator = '|', useApices = False):
    csvFile = f"{sql_out_dir}/features_alignment.csv"
    feature = {
        'key' : '',
        'monster-name' : '',
        'alignment-id' : '',
    }
    dbConn = common.connect_db(sqlDatabase)
    if dbConn is None:
        return
    
    aligns = dict()
    moralOrder = dict()
    moralView = dict()
    for k, v in common.fetch_values(dbConn, 'MORAL_ALIGNMENT', 'ID', 'NAME').items():
        aligns[k.lower()] = v
    for k, v in common.fetch_values(dbConn, 'MORAL_ORDER', 'ID', 'NAME').items():
        moralOrder[k.lower()] = v
    for k, v in common.fetch_values(dbConn, 'MORAL_VIEW', 'ID', 'NAME').items():
        moralView[k.lower()] = v

    with open(csvFile, 'w') as outFile:
        common.write_csv_caption(outFile, feature, useApices, separator)
        for monster in monsters:
            feature['key'] = monster['monsterKey']
            feature['monster-name'] = monster['name']
            align = monster['Alignment'].lower()
            alignList = None

            if align in aligns.keys():
                alignList = [align]

            if alignList is None:
                alignList = match_neutral(align)

            if alignList is None:
                alignList = match_any_wildcard(align, moralOrder, moralView)

            if alignList is None:
                alignList = match_parenthesys(align, moralOrder, moralView)

            if alignList is not None:
                for a in alignList:
                    feature['alignment-id'] = aligns[a]
                    common.write_csv_row(outFile, feature, useApices, separator)


################################
# def generate_treasure_features(sql_out_dir, monsters, separator = '|', useApices = False):
#     csvFile = f"{sql_out_dir}/treasure_features.csv"
#     treasureFeature = {
#         'key' : '',
#         'monster-name' : '',
#         'treasure-class' : '',
#         'multiplier' : '',
#         'treasure-component' : '',
#     }

#     with open(csvFile, 'w') as outFile:
#         common.write_csv_caption(outFile, treasureFeature, useApices, separator)
#         for monster in monsters:
#             if monster['Treasure'] == 'Nil' or monster['Treasure'] == 'Special':
#                 continue

#             treasureFeature['key'] = monster['monsterKey']
#             treasureFeature['monster-name'] = monster['title']
#             treasureClasses = monster['Treasure']

#             classes = parse_treasures(treasureClasses)

#             treasureFeature['treasure-class'] = treasureClasses
#             treasureFeature['multiplier'] = None
#             treasureFeature['treasure-component'] = None
#             common.write_csv_row(outFile, treasureFeature, useApices, separator)


################################
def main():
    if len(sys.argv) < 2:
        common.print_help(sys.argv[0])
        sys.exit()

    jsonConfig = ""
    csvFile = ""
    source = ""
    sqlOutDir = ""
    limit = 0
    dbPath = ""
    
    opts, args = getopt.getopt(sys.argv[1:],"h:j:o:s:d:l:b:",["help","json=","output=","source=","sql_out_directory=","limit=","database="])
    for opt, arg in opts:
        if opt == '-h':
            common.print_help(sys.argv[0])
            sys.exit()
        elif opt in ("-j", "--json"):
            jsonConfig = arg
        elif opt in ("-o", "--output"):
            csvFile = arg
        elif opt in ("-s", "--source"):
            source = arg
        elif opt in ("-d", "--sql_out_directory"):
            sqlOutDir = arg
        elif opt in ("-l", "--limit"):
            limit = int(arg)
        elif opt in ("-b", "--database"):
            dbPath = arg

    separator = '|'
    useApices = False
    sqlOutDir = os.path.dirname(csvFile) if sqlOutDir == "" else sqlOutDir

    monsters = get_monsters_list(jsonConfig, source, limit)
    generate_csv(csvFile, monsters, separator, useApices)

    #generate_treasure_features(sqlOutDir, monsters, separator, useApices)
    generate_alignment_features(dbPath, sqlOutDir, monsters, separator, useApices)


###############################################
if __name__ == "__main__":    
    main()