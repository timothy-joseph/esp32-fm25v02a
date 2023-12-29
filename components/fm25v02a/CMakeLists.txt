idf_component_register(
PRIV_REQUIRES driver esp_timer
SRCS "fm25v02a_init.c" "fm25v02a_read_status_register.c" "fm25v02a_write.c" "fm25v02a_write_status_register.c" "fm25v02a_device_id.c" "fm25v02a_read.c" "fm25v02a_sleep.c" "fm25v02a_write_enable_disable.c"
INCLUDE_DIRS "include"
)
