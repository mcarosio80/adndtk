import sys
import json
import getopt
from sys import argv

import common

################################
def print_enum(lines, enumName, outFile, indentationLevel, isClass = True):
    numLines = len(lines)
    countEnums = 1
    if len(lines) > 0:
        outFile.write(f"{common.indent(indentationLevel)}")
        if isClass:
            outFile.write(f"enum class {enumName}\n")
        else:
            outFile.write(f"enum {enumName}\n")
            
        outFile.write(f"{common.indent(indentationLevel)}")
        outFile.write("{\n")

        for line in lines:
            outFile.write(f"{common.indent(indentationLevel+1)}")
            if countEnums < numLines:
                outFile.write(f"{line},\n")
            else:
                outFile.write(f"{line}\n")
            countEnums += 1

        outFile.write(f"{common.indent(indentationLevel)}")
        outFile.write("};\n\n")

        outFile.write(f"{common.indent(indentationLevel)}")
        outFile.write(f"inline std::ostream& operator<<(std::ostream& out, const {enumName}& data)\n")
        outFile.write(f"{common.indent(indentationLevel)}{{\n")
        outFile.write(f"{common.indent(indentationLevel+1)}out << static_cast<std::underlying_type<{enumName}>::type>(data);\n")
        outFile.write(f"{common.indent(indentationLevel+1)}return out;\n")
        outFile.write(f"{common.indent(indentationLevel)}}}\n")

################################
def generate_enum(dbPath, headerFile, namespaces, version, jsonConfig):
    cfg = open(jsonConfig)
    data = json.load(cfg)
     
    #open database connection
    conn = common.connect_db(dbPath)
    if conn is None:
        return 1
    
    with open(headerFile, 'w') as outFile:
        common.write_heading(outFile, headerFile, version)

        outFile.write(f'#include <iostream>\n')
        outFile.write(f'#include <type_traits>\n\n')
    
        indentation = common.open_namespace(outFile, namespaces)

        firstElement = True
        for d in data:
            tableName = d['table']
            keyColumn = d['key']
            valueColumn = d['value']
            enumName = common.clean_string(tableName)
    
            if firstElement:
                firstElement = False
            else:
                outFile.write("\n")

            lines = common.fetch_key_value_pairs(conn, keyColumn, valueColumn, tableName)
            print_enum(lines, enumName, outFile, indentation)

        common.close_namespace(outFile, namespaces, indentation)

        common.write_footer(outFile, headerFile)
    
    cfg.close()

def main():
    if len(sys.argv) < 2:
        common.print_help(sys.argv[0])
        sys.exit()

    dbPath = ""
    jsonConfig = ""
    headerFile = ""
    namespaces = list()
    version = ""
    
    opts, args = getopt.getopt(sys.argv[1:],"h:d:j:o:n:v:",["help","database=","json=","output=","namespace=","version="])
    for opt, arg in opts:
        if opt == '-h':
            common.print_help(sys.argv[0])
            sys.exit()
        elif opt in ("-d", "--database"):
            dbPath = arg
        elif opt in ("-j", "--json"):
            jsonConfig = arg
        elif opt in ("-o", "--output"):
            headerFile = arg
        elif opt in ("-n", "--namespace"):
            namespaces = arg.split(':')
        elif opt in ("-v", "--version"):
            version = arg

    generate_enum(dbPath, headerFile, namespaces, version, jsonConfig)

###############################################
if __name__ == "__main__":    
    main()