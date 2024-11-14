import sys
import sqlite3
from sqlite3 import Error
import json
import getopt
from sys import argv
from datetime import datetime
import os
import re

################################
def connect_db(db_file):
    conn = None
    try:
        conn = sqlite3.connect(db_file)
    except Error as e:
        print(e)

    return conn

################################
def clean_string(token):
    t = token.lower()
    return t.replace(' ', '_').replace('\'', '').replace('-', '_').replace('+', '_or_more').replace(',', '').replace(')', '').replace('(', '').replace('.', '').replace('/', '_')

################################
def fetch_enum_values(conn, key, value, table):
    cur = conn.cursor()
    cur.execute(f"SELECT {key}, {value} FROM {table};")

    rows = cur.fetchall()

    lines = list()
    for row in rows:
        lines.append(f"{clean_string(row[0])} = {row[1]}")
    return lines

################################
def print_enum(lines, enumName, outFile, indentationLevel, isClass = True):
    numLines = len(lines)
    countEnums = 1
    if len(lines) > 0:
        outFile.write(f"{indent(indentationLevel)}")
        if isClass:
            outFile.write(f"enum class {enumName}\n")
        else:
            outFile.write(f"enum {enumName}\n")
            
        outFile.write(f"{indent(indentationLevel)}")
        outFile.write("{\n")

        for line in lines:
            outFile.write(f"{indent(indentationLevel+1)}")
            if countEnums < numLines:
                outFile.write(f"{line},\n")
            else:
                outFile.write(f"{line}\n")
            countEnums += 1

        outFile.write(f"{indent(indentationLevel)}")
        outFile.write("};\n\n")

        outFile.write(f"{indent(indentationLevel)}")
        outFile.write(f"inline std::ostream& operator<<(std::ostream& out, const {enumName}& data)\n")
        outFile.write(f"{indent(indentationLevel)}{{\n")
        outFile.write(f"{indent(indentationLevel+1)}out << static_cast<std::underlying_type<{enumName}>::type>(data);\n")
        outFile.write(f"{indent(indentationLevel+1)}return out;\n")
        outFile.write(f"{indent(indentationLevel)}}}\n")

################################
def print_help(exeName):
    print (f'Usage: {exeName} -d <db_file> -j <json_config> -o <output_header> -n <lib-namespace:module-namespace>')

################################
def indent(indentationLevel):
    if indentationLevel == 0:
        return ""
    return '\t' * indentationLevel

################################
def fetch_tables(conn, tableList):
    cur = conn.cursor()
    cur.execute(f"select lower(name) as name from sqlite_master where type = 'table';")
    rows = cur.fetchall()

    tables = list()
    for row in rows:
        if row[0] in tableList:
            tables.append(row[0])
    return tables

################################
def to_ctype(varName, sqlType):
    cType = sqlType
    if sqlType == "INTEGER":
        cType = "int"
    elif sqlType == "INT":
        cType = "int"
    elif sqlType == "FLOAT":
        cType = "double"
    else:
        res = re.search(r"CHAR\((\d+)\)", sqlType)
        if res:
            if int(res.group(1)) == 1:
                cType = "char"
            else:
                cType = "std::string"
        # if res:
        #     cType = "char"
        #     if int(res.group(1)) > 1:
        #         varName = f"{varName}[{res.group(1)}]"
    
    return cType, varName

################################
def fetch_table_info(conn, tableName):
    cur = conn.cursor()
    cur.execute(f"pragma table_info({tableName});")
    rows = cur.fetchall()

    info = list()
    for row in rows:
        dataType, identifier = to_ctype(row[1].lower(), row[2])
        t = (tableName, row[0], identifier, dataType, row[3], row[4], row[5])
        info.append(t)
    return info

################################
def print_free_helpers(outFile, indentationLevel):
    outFile.write(f"""{indent(indentationLevel)}namespace Helpers\n""")
    outFile.write(f"""{indent(indentationLevel)}{{""")

    # Generate function to_vector
    outFile.write(f"""
            /// Returns data from a given column and specified query
            template<typename _T>
            static std::vector<_T> to_vector(const std::string& fieldName, const Query& queryId)
            {{
                auto results = Cyclopedia::get_instance().exec_prepared_statement<>(queryId);
                return results.to_vector<_T>(fieldName);
            }}\n""")

    # Generate function to_set
    outFile.write(f"""
            /// Returns data from a given column and specified query as a typed set
            template<typename _Type>
			static std::set<_Type> to_set(const std::string& fieldName, const Query& queryId)
            {{
                std::set<_Type> data{{}};
                auto results = Cyclopedia::get_instance().exec_prepared_statement<>(queryId);
                for (auto& r : results)
                {{
                    auto value = r.as<_Type>(fieldName);
                    data.emplace(value);
                }}
                return data;
			}}\n""")

    outFile.write(f"""{indent(indentationLevel)}}} // namespace Helpers\n\n""")

################################
def print_struct_methods(tableName, fields, outFile, indentationLevel):
    # Generate function to_vector
    outFile.write(f"""
            /// Returns data from a given column in table '{tableName}' as a typed vector
            template<typename _T>
			static std::vector<_T> to_vector(const std::string& fieldName)
            {{
                return Helpers::to_vector<_T>(fieldName, Query::select_all_{tableName});
			}}\n""")

    # Generate function to_map
    outFile.write(f"""
            /// Returns all records from table '{tableName}' indexed on 'fieldName'
            template<typename _KeyType>
			static std::map<_KeyType, {tableName}> to_map(const std::string& fieldName)
            {{
                std::map<_KeyType, {tableName}> data{{}};
                auto results = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_all_{tableName});
                for (auto& r : results)
                {{
                    auto key = r.as<_KeyType>(fieldName);
                    data[key] = convert(r);
                }}
                return data;
			}}\n""")

    # Generate function to_set
    outFile.write(f"""
            /// Returns data from a given column in table '{tableName}' as a typed set
            template<typename _Type>
			static std::set<_Type> to_set(const std::string& fieldName)
            {{
                return Helpers::to_set<_Type>(fieldName, Query::select_all_{tableName});
			}}\n""")

    # Generate function fetch_all
    outFile.write(f"""
            /// Returns all records from table '{tableName}'
            static std::vector<{tableName}> fetch_all()
            {{
                std::vector<{tableName}> vec{{}};
                auto results = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_all_{tableName});
                for (auto& r : results)
                {{
                    {tableName} stats = convert(r);
                    vec.push_back(stats);
                }}
                return vec;
            }}\n""")

    # Generate function select
    outFile.write(f"""
            /// Returns all records from table '{tableName}' where fieldName matches fieldValue
            template<typename _FieldType>
            static std::vector<{tableName}> select(const std::string& fieldName, const _FieldType& fieldValue)
            {{
                std::vector<{tableName}> vec{{}};
                auto results = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_all_{tableName});
                for (auto& r : results)
                {{
                    if (r.as<_FieldType>(fieldName) == fieldValue)
                    {{
                        {tableName} stats = convert(r);
                        vec.push_back(stats);
                    }}
                }}
                return vec;
            }}\n""")

    # Generate function select_one
    outFile.write(f"""
            /// Returns the first record from table '{tableName}' where fieldName matches fieldValue
            template<typename _FieldType>
            static std::optional<{tableName}> select_one(const std::string& fieldName, const _FieldType& fieldValue)
            {{
                const auto data = select(fieldName, fieldValue);
                return (data.empty()) ? std::nullopt : std::make_optional(data.at(0));
            }}\n""")

    # Begins private block
    outFile.write(f"""{indent(indentationLevel)}private:""")

    # Generate function convert
    outFile.write(f"""
            /// Converts a record from table '{tableName}' to plain data object
            static {tableName} convert(const QueryResult& r)
            {{
                {tableName} stats{{}};\n""")
    
    indentationLevel += 2
    for t in fields:
        # field-type, field-name, is-nullable
        fieldName = t[0]
        fieldType = t[1]
        isNullable = t[2]

        outFile.write(f"{indent(indentationLevel)}")
        if isNullable == 0:
            if fieldType == "std::string":
                outFile.write(f"stats.{fieldName} = r[\"{fieldName}\"];")
            else:
                outFile.write(f"stats.{fieldName} = r.try_as<{fieldType}>(\"{fieldName}\");")
        else:
            if fieldType == "std::string":
                outFile.write(f"stats.{fieldName} = r[\"{fieldName}\"].value_or(\"\");")
            else:
                outFile.write(f"stats.{fieldName} = r.try_as<{fieldType}>(\"{fieldName}\").value();")
        outFile.write("\n")
            
    outFile.write(f"""
                return stats;
            }}\n""")

################################
def print_struct(tableName, fields, outFile, indentationLevel):
     if len(fields) > 0:
        outFile.write(f"{indent(indentationLevel)}")
        outFile.write(f"struct {tableName}\n")
            
        outFile.write(f"{indent(indentationLevel)}")
        outFile.write("{\n")

        for field in fields:
            outFile.write(f"{indent(indentationLevel+1)}")
            if field[2] == 1:
                outFile.write(f"{field[1]} {field[0]}{{}};\n")
            else:
                outFile.write(f"std::optional<{field[1]}> {field[0]}{{}};\n")
        
        print_struct_methods(tableName, fields, outFile, indentationLevel)

        outFile.write(f"{indent(indentationLevel)}")
        outFile.write("};\n")

################################
def write_heading(outFile, headerFile, version, addIncludes):
    now = datetime.now()
    dateTime = now.strftime("%Y-%m-%d, %H:%M:%S")
    headerName = os.path.basename(headerFile)
    
    #outFile.write(f'#pragma once\n\n')
    includeGuard = headerName.replace('.', '_').upper()
    outFile.write(f'#ifndef {includeGuard}\n')
    outFile.write(f'#define {includeGuard}\n\n')

    outFile.write(f'// AD&D Toolkit version {version}\n')
    outFile.write('// Autogenerated header file, do not modify!!\n')
    outFile.write(f'// Header file: {headerName}\n')
    outFile.write(f'// Last modified: {dateTime}\n\n')

    if addIncludes:
        outFile.write(f'#include <optional>\n')
        outFile.write(f'#include <string>\n')
        outFile.write(f'#include <vector>\n')
        outFile.write(f'#include <set>\n')
        outFile.write(f'#include <algorithm>\n')
        outFile.write(f'#include <defs.h>\n')
        outFile.write(f'#include <common_types.h>\n')
        outFile.write(f'#include <cyclopedia.h>\n')
        outFile.write(f'#include <query_result.h>\n\n')


def write_footer(outFile, headerFile):
    headerName = os.path.basename(headerFile)
    includeGuard = headerName.replace('.', '_').upper()
    outFile.write(f'#endif // {includeGuard}\n')
    
################################
def open_namespace(outFile, namespaces):
    indentLevel = 0
    for ns in namespaces:
        outFile.write(f'{indent(indentLevel)}namespace {ns}\n')
        outFile.write(f"{indent(indentLevel)}")
        outFile.write("{\n")
        indentLevel += 1
    return indentLevel

################################
def close_namespace(outFile, namespaces, indentLevel):
    for ns in namespaces:
        indentLevel -= 1
        outFile.write(f"{indent(indentLevel)}")
        outFile.write("}\n")

################################
def generate_struct(dbPath, headerFile, namespaces, version, jsonConfig):
    cfg = open(jsonConfig)
    data = json.load(cfg)

    #open database connection
    conn = connect_db(dbPath)
    if conn is None:
        return 1
    
    tables = fetch_tables(conn, data)
    
    with open(headerFile, 'w') as outFile:
        write_heading(outFile, headerFile, version, True)
    
        indentation = open_namespace(outFile, namespaces)

        print_free_helpers(outFile, indentation)

        firstElement = True
        fields = list()
        for table in tables:
            info = fetch_table_info(conn, table)
            tableName = info[0][0]

            for i in info:
                #cid = info[1]
                # field-type, field-name, is-nullable
                fieldName = i[2]
                fieldType = i[3]
                for idx in range(len(data[table])):
                    if 'field' in data[table][idx] and 'type' in data[table][idx] and data[table][idx]['field'] == fieldName:
                        fieldType = data[table][idx]['type']

                fields.append( (fieldName, fieldType, i[4]) )
                #isNullable = info[4]
                #defaultValue = info[5]
                #isPrimaryKey = info[6]

            if firstElement:
                firstElement = False
            else:
                outFile.write("\n")

            print_struct(tableName, fields, outFile, indentation)
            fields.clear()

        close_namespace(outFile, namespaces, indentation)

        write_footer(outFile, headerFile)

################################
def generate_enum(dbPath, headerFile, namespaces, version, jsonConfig):
    cfg = open(jsonConfig)
    data = json.load(cfg)
     
    #open database connection
    conn = connect_db(dbPath)
    if conn is None:
        return 1
    
    with open(headerFile, 'w') as outFile:
        write_heading(outFile, headerFile, version, False)

        outFile.write(f'#include <iostream>\n')
        outFile.write(f'#include <type_traits>\n\n')
    
        indentation = open_namespace(outFile, namespaces)

        firstElement = True
        for d in data:
            tableName = d['table']
            keyColumn = d['key']
            valueColumn = d['value']
            enumName = clean_string(tableName)
    
            if firstElement:
                firstElement = False
            else:
                outFile.write("\n")

            lines = fetch_enum_values(conn, keyColumn, valueColumn, tableName)
            print_enum(lines, enumName, outFile, indentation)

        close_namespace(outFile, namespaces, indentation)

        write_footer(outFile, headerFile)
    
    cfg.close()

def main():
    if len(sys.argv) < 2:
        print_help(sys.argv[0])
        sys.exit()

    dbPath = ""
    jsonConfig = ""
    headerFile = ""
    namespaces = list()
    version = ""
    target = ""
    
    opts, args = getopt.getopt(sys.argv[1:],"h:d:j:o:n:v:t:",["help","database=","json=","output=","namespace=","version=","target="])
    for opt, arg in opts:
        if opt == '-h':
            print_help(sys.argv[0])
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
        elif opt in ("-t", "--target"):
            target = arg

    if target == "enum":
        generate_enum(dbPath, headerFile, namespaces, version, jsonConfig)
    elif target == "struct":
        generate_struct(dbPath, headerFile, namespaces, version, jsonConfig)
    else:
        print_help(sys.argv[0])
        sys.exit()

###############################################
if __name__ == "__main__":    
    main()