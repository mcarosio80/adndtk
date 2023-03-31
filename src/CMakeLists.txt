add_library(adndtk STATIC adndtk.cpp adndtk.h
                        dice.cpp dice.h
                        skills.cpp skills.h
                        query_result.cpp query_result.h
                        skill_creator.cpp skill_creator.h
                        advancement_table.cpp advancement_table.h
                        experience.cpp experience.h
                        hit_points.cpp hit_points.h
)

add_dependencies(adndtk rapidjson)

target_include_directories(adndtk PUBLIC ${SQLite3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (adndtk ${SQLite3_LIBRARIES})