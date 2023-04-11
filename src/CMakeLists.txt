add_library(adndtk STATIC adndtk.cpp adndtk.h
                        cyclopedia.cpp cyclopedia.h
                        common_types.h
                        options.cpp options.h
                        dice.cpp dice.h
                        skills.cpp skills.h
                        query_result.cpp query_result.h
                        skill_creator.cpp skill_creator.h
                        advancement_table.cpp advancement_table.h
                        experience.cpp experience.h
                        hit_points.cpp hit_points.h
                        saving_throws.cpp saving_throws.h
                        thaco.cpp thaco.h
                        turn_undead.cpp turn_undead.h
                        coin_exchange.cpp coin_exchange.h
                        coin.cpp coin.h
)

add_dependencies(adndtk rapidjson)

target_include_directories(adndtk PUBLIC ${SQLite3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (adndtk ${SQLite3_LIBRARIES})