add_executable(a4keyboard
    args.cpp
    device.cpp
    kernelapi.cpp
    main.cpp
    variants/bloody.cpp
)

target_include_directories(a4keyboard PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(a4keyboard PRIVATE kernel)

install(TARGETS a4keyboard PERMISSIONS
    OWNER_WRITE OWNER_READ OWNER_EXECUTE SETUID
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)
