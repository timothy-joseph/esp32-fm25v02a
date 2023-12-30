idf_component_register(SRCS "esp32-fm25v02a.c"
                    INCLUDE_DIRS ".")
target_compile_options(${COMPONENT_LIB} PUBLIC -Wall -Wextra)
