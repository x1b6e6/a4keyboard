configure_file(80-a4keyboard.rules.in 80-a4keyboard.rules)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/80-a4keyboard.rules
    DESTINATION /usr/lib/udev/rules.d
    PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
    COMPONENT udev-rules
)
