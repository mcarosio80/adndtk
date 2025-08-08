import sys
import json
import getopt
from sys import argv

import common

################################
def print_free_helpers(outFile, indentationLevel):
    outFile.write(f"""{common.indent(indentationLevel)}namespace Helpers\n""")
    outFile.write(f"""{common.indent(indentationLevel)}{{""")

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

    outFile.write(f"""{common.indent(indentationLevel)}}} // namespace Helpers\n\n""")

################################
def print_struct_methods(tableName, fields, indexedFields, outFile, indentationLevel):
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
    
    # Generate function select_index
    fieldCount = len(indexedFields)
    if fieldCount > 0:
        outFile.write(f"""
            /// Returns a subset of records from table '{tableName}' where indexed fields match parameters
            template <""")
        
        for f in indexedFields:
            fieldCount -= 1
            paramName = f[2]
            outFile.write(f"typename _{paramName}_t")
            if fieldCount > 0:
                outFile.write(", ")
        outFile.write(">")

        outFile.write(f"""
            static std::vector<{tableName}> select_by(""")

        fieldCount = len(indexedFields)
        for f in indexedFields:
            fieldCount -= 1
            paramName = f[2]
            #paramType = f[3]
            outFile.write(f"const _{paramName}_t& {paramName}")
            if fieldCount > 0:
                outFile.write(", ")
        
        outFile.write(f""")
            {{""");

        for f in indexedFields:
            fieldCount -= 1
            paramName = f[2]
            paramType = f[3]
            outFile.write(f"""
                {paramType} {paramName}_in{{}};
                if constexpr(std::is_enum_v<_{paramName}_t>)
                {{
                    using _{paramName}_ut = typename std::underlying_type_t<_{paramName}_t>;
                    {paramName}_in = static_cast<_{paramName}_ut>({paramName});
                }}
                else
                {{
                    {paramName}_in = {paramName};
                }}
            """)

        outFile.write(f"""
                std::vector<{tableName}> vec{{}};
                std::stringstream stmt{{}};
                stmt << "select json_group_array(json_object(""")
        
        count = len(fields)
        for t in fields:
            count -= 1
            fieldName = t[0]
            outFile.write(f"'{fieldName}', {fieldName}")
            if count > 0:
                outFile.write(", ")
                
        outFile.write(f")) from {tableName} where ")

        fieldCount = len(indexedFields)
        for f in indexedFields:
            fieldCount -= 1
            paramName = f[2]
            paramType = f[3]
            outFile.write(f"{paramName} = ")
            if paramType == "std::string":
                outFile.write("'")
            outFile.write(f"\" << {paramName}_in")

            if fieldCount > 0:
                if paramType == "std::string":
                    outFile.write(" << \"' and ")
                else:
                    outFile.write(" << \" and ")
            else:
                if paramType == "std::string":
                    outFile.write(" << \"';\";\n")
                else:
                    outFile.write(" << \";\";\n")

        outFile.write(f"""
                auto results = Cyclopedia::get_instance().exec(stmt.str().c_str());
                for (auto& r : results)
                {{
                    {tableName} stats = convert(r);
                    vec.push_back(stats);
                }}
                return vec;
            }}\n\n""")


    # Begins private block
    outFile.write(f"""{common.indent(indentationLevel)}private:""")

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

        outFile.write(f"{common.indent(indentationLevel)}")
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
def print_struct(tableName, fields, indexedFields, outFile, indentationLevel):
     if len(fields) > 0:
        outFile.write(f"{common.indent(indentationLevel)}")
        outFile.write(f"struct {tableName}\n")
            
        outFile.write(f"{common.indent(indentationLevel)}")
        outFile.write("{\n")

        for field in fields:
            outFile.write(f"{common.indent(indentationLevel+1)}")
            if field[2] == 1:
                outFile.write(f"{field[1]} {field[0]}{{}};\n")
            else:
                outFile.write(f"std::optional<{field[1]}> {field[0]}{{}};\n")
        
        print_struct_methods(tableName, fields, indexedFields, outFile, indentationLevel)

        outFile.write(f"{common.indent(indentationLevel)}")
        outFile.write("};\n")

################################
def generate_struct(dbPath, headerFile, namespaces, version, jsonConfig):
    cfg = open(jsonConfig)
    data = json.load(cfg)

    #open database connection
    conn = common.connect_db(dbPath)
    if conn is None:
        return 1
    
    tables = common.fetch_tables(conn, data)
    
    with open(headerFile, 'w') as outFile:
        common.write_heading(outFile, headerFile, version)

        outFile.write(f'#include <optional>\n')
        outFile.write(f'#include <string>\n')
        outFile.write(f'#include <vector>\n')
        outFile.write(f'#include <set>\n')
        outFile.write(f'#include <algorithm>\n')
        outFile.write(f'#include <sstream>\n')
        outFile.write(f'#include <defs.h>\n')
        outFile.write(f'#include <common_types.h>\n')
        outFile.write(f'#include <cyclopedia.h>\n')
        outFile.write(f'#include <query_result.h>\n\n')
    
        indentation = common.open_namespace(outFile, namespaces)

        print_free_helpers(outFile, indentation)

        firstElement = True
        fields = list()
        for table in tables:
            info = common.fetch_table_info(conn, table)
            indexedFields = common.fetch_index_info(conn, table.upper())
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

            print_struct(tableName, fields, indexedFields, outFile, indentation)
            fields.clear()


        common.close_namespace(outFile, namespaces, indentation)

        common.write_footer(outFile, headerFile)

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

    generate_struct(dbPath, headerFile, namespaces, version, jsonConfig)

###############################################
if __name__ == "__main__":    
    main()