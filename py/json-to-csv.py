import sys
import json
import getopt
from sys import argv

import common

################################
def generate_csv(csvFile, jsonFile):
    jsonInput = open(jsonFile)
    monstersData = json.load(jsonInput)
    fieldNames = ['Diet','Frequency','Hit Dice','Intelligence','Magic Resistance',
                  'Morale','Movement','No. Appearing','No. of Attacks','Organization','Size',
                  'Special Attacks','Special Defenses','THAC0','Treasure','XP Value',
                  "Alignment" , "Armor Class", "Climate/Terrain", "Damage/Attack"]
               
    separator = '|'
    useApices = False
    writeCaption = True
    
    with open(csvFile, 'w') as outFile:
        for md in monstersData:
            monster = md['monster_data']
            mainTitle = md['title']
            monsterKey = md["monster_key"]

            tsrIssues = monster['TSR']
            setting = monster['setting']

            if 'statblock' not in monster.keys():
                continue

            monsterInfo = monster['statblock']
            if monsterInfo == None:
                continue
            title = monster['title']

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

                if writeCaption == True:
                    writeCaption = False
                    common.write_csv_caption(outFile, fields, useApices, separator)
                
                common.write_csv_row(outFile, fields, useApices, separator)
    
    jsonInput.close()

def main():
    if len(sys.argv) < 2:
        common.print_help(sys.argv[0])
        sys.exit()

    jsonConfig = ""
    csvFile = ""
    
    opts, args = getopt.getopt(sys.argv[1:],"h:j:o:",["help","json=","output="])
    for opt, arg in opts:
        if opt == '-h':
            common.print_help(sys.argv[0])
            sys.exit()
        elif opt in ("-j", "--json"):
            jsonConfig = arg
        elif opt in ("-o", "--output"):
            csvFile = arg

    generate_csv(csvFile, jsonConfig)

###############################################
if __name__ == "__main__":    
    main()