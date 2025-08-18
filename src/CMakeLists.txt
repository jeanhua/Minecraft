file(
        GLOB_RECURSE FUNCS ./ *.cpp
)

add_library(
        funcs ${FUNCS}
)