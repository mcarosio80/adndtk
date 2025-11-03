import sys
import json
import getopt
from sys import argv

import common

################################
def print_dictionary(tableName, idField, descField, data, outFile, indentationLevel):
    numLines = len(data)
    enumName = common.clean_string(tableName)
    enumTypeName = f"Defs::{enumName}"

    if len(data) > 0:
        outFile.write(f"""{common.indent(indentationLevel)}static std::string to_string(const {enumTypeName}& value)
        {{
            auto data = Cyclopedia::get_instance().exec_prepared_statement(Query::select_all_{enumName}).to_map<{enumTypeName}, std::string>("{idField}", "{descField}");
            return data[value];
        }}\n""")

################################
def generate_dictionary(dbPath, headerFile, namespaces, version, jsonConfig):
    cfg = open(jsonConfig)
    data = json.load(cfg)
     
    #open database connection
    conn = common.connect_db(dbPath)
    if conn is None:
        return 1
    
    with open(headerFile, 'w') as outFile:
        common.write_heading(outFile, headerFile, version)

        outFile.write(f'#include <string>\n')
        outFile.write(f'#include <map>\n')
        outFile.write(f'#include <defs.h>\n')
        outFile.write(f'#include <cyclopedia.h>\n')
        outFile.write(f'#include <query_result.h>\n')
        outFile.write(f'#include <queries.h>\n')
        outFile.write(f'\n')
    
        indentation = common.open_namespace(outFile, namespaces)

        firstElement = True
        for d in data:
            tableName = d['table']
            keyColumn = d['key']
            valueColumn = d['value']
    
            if firstElement:
                firstElement = False
            else:
                outFile.write("\n")

            values = common.fetch_values(conn, tableName, keyColumn, valueColumn)
            print_dictionary(tableName, valueColumn, keyColumn, values, outFile, indentation)

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

    generate_dictionary(dbPath, headerFile, namespaces, version, jsonConfig)

###############################################
if __name__ == "__main__":    
    main()