import sys
import json
import getopt
from sys import argv

import common


###############################
def print_enum(data, enumName, outFile, indentationLevel):
    outFile.write(f"{common.indent(indentationLevel)}enum class {enumName}\n")
    outFile.write(f"{common.indent(indentationLevel)}{{\n")
    for d in data:
        queryId = d['id']
        outFile.write(f"{common.indent(indentationLevel+1)}{queryId},\n")

    outFile.write(f"{common.indent(indentationLevel)}}};\n\n")

################################
def generate_queries(headerFile, namespaces, version, jsonConfig):
    queries = open(jsonConfig)
    data = json.load(queries)
    
    with open(headerFile, 'w') as outFile:
        common.write_heading(outFile, headerFile, version)

        outFile.write(f'#include <string>\n')
        outFile.write(f'#include <map>\n\n')
    
        indentation = common.open_namespace(outFile, namespaces)

        print_enum(data, "Query", outFile, indentation)
        
        outFile.write(f"{common.indent(indentation)}inline static std::map<Query, std::string> Queries =\n")
        outFile.write(f"{common.indent(indentation)}{{\n")

        for d in data:
            queryId = d['id']
            fieldsSelect = d['select']
            useDistinct = d['distinct'] if 'distinct' in d.keys() else ""
            fieldsAliases = d['aliases'] if 'aliases' in d.keys() else []
            fromClauses = d['from']
            whereClauses = d['where'] if 'where' in d.keys() else []
            orderlauses = d['order'] if 'order' in d.keys() else []
            groupClauses = d['group'] if 'group' in d.keys() else []
            limitNumber = d['limit'] if 'limit' in d.keys() else ""
    
            hasAliases = len(fieldsAliases)>0
            stmt = "select "
            stmt += "distinct " if useDistinct == "yes" else ""
            for idx, fs in enumerate(fieldsSelect):
                stmt += f"'{fieldsAliases[idx]}'" if hasAliases else f"'{fs}'"
                stmt += f", {fs}"
                stmt += "" if idx==len(fieldsSelect)-1 else ", "

            stmt += " from"
            for fc in fromClauses:
                stmt += f" {fc}"

            countWhere = len(whereClauses)
            if countWhere>0:
                stmt += " where "
                for idx, wc in enumerate(whereClauses):
                    stmt += f"{wc}"
                    stmt += "" if idx==countWhere-1 else " and "

            groupOrder = len(groupClauses)
            if groupOrder>0:
                stmt += " group by "
                for idx, gc in enumerate(groupClauses):
                    stmt += f"{gc}"
                    stmt += "" if idx==groupOrder-1 else ", "

            countOrder = len(orderlauses)
            if countOrder>0:
                stmt += " order by "
                for idx, oc in enumerate(orderlauses):
                    stmt += f"{oc}"
                    stmt += "" if idx==countOrder-1 else ", "

            if limitNumber != "":
                stmt += f" limit {limitNumber}"
            
            outFile.write(f"{common.indent(indentation+1)}{{Query::{queryId}, \"{stmt}\"}},\n")

        outFile.write(f"{common.indent(indentation)}}};\n")

        common.close_namespace(outFile, namespaces, indentation)

        common.write_footer(outFile, headerFile)
    
    queries.close()

def main():
    if len(sys.argv) < 2:
        common.print_help(sys.argv[0])
        sys.exit()

    # dbPath = ""
    jsonConfig = ""
    headerFile = ""
    namespaces = list()
    version = ""
    
    # opts, args = getopt.getopt(sys.argv[1:],"h:d:j:o:n:v:",["help","database=","json=","output=","namespace=","version="])
    opts, args = getopt.getopt(sys.argv[1:],"h:j:o:n:v:",["help","json=","output=","namespace=","version="])
    for opt, arg in opts:
        if opt == '-h':
            common.print_help(sys.argv[0])
            sys.exit()
        # elif opt in ("-d", "--database"):
        #     dbPath = arg
        elif opt in ("-j", "--json"):
            jsonConfig = arg
        elif opt in ("-o", "--output"):
            headerFile = arg
        elif opt in ("-n", "--namespace"):
            namespaces = arg.split(':')
        elif opt in ("-v", "--version"):
            version = arg

    generate_queries(headerFile, namespaces, version, jsonConfig)

###############################################
if __name__ == "__main__":    
    main()