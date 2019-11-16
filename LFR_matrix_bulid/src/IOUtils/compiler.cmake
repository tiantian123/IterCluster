set(IOUtilsDir ${PROJECT_SOURCE_DIR}/IOUtils)
include_directories(${IOUtilsDir})

set(source_cpp ${source_cpp} 
    ${IOUtilsDir}/LFRReader.cpp
    ${IOUtilsDir}/BQ2UKWriter.cpp
    ${IOUtilsDir}/BC2UK_BinaryReader.cpp
    ${IOUtilsDir}/BC2UK_BinaryWriter.cpp
    ${IOUtilsDir}/BQ2UKReader.cpp
    ${IOUtilsDir}/BQMatrixWriter.cpp
    )

set(source_h ${source_h}
    ${IOUtilsDir}/LFRReader.h
    ${IOUtilsDir}/BQ2UKWriter.h
    ${IOUtilsDir}/BQ2UKReader.h
    ${IOUtilsDir}/BQMatrixWriter.h
    ${IOUtilsDir}/BC2UK_BinaryReader.h
    ${IOUtilsDir}/BC2UK_BinaryWriter.h
    )
