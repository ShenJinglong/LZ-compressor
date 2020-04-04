cmake_minimum_required( VERSION 2.6 )

add_definitions(-std=c++11)

project( lz )

aux_source_directory( . SRC_LIST )
aux_source_directory( ./BitOperator SRC_LIST )
aux_source_directory( ./HuffmanCodec SRC_LIST )
aux_source_directory( ./tools SRC_LIST )
message( "SRC_LIST added as follows: " )
message( ${SRC_LIST} )

add_executable( lz ${SRC_LIST} )