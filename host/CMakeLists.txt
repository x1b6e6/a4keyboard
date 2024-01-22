add_executable(4akeyboard
    args.cpp
    cmd.cpp
    device.cpp
    kernelapi.cpp
    main.cpp
)
target_link_libraries(4akeyboard PRIVATE kernel)
