add_executable(a4keyboard
    args.cpp
    cmd.cpp
    device.cpp
    kernelapi.cpp
    main.cpp
)

target_link_libraries(a4keyboard PRIVATE kernel)

install(TARGETS a4keyboard PERMISSIONS
    OWNER_WRITE OWNER_READ OWNER_EXECUTE SETUID
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)
