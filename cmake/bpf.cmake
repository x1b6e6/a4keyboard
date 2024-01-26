if (NOT DEFINED CMAKE_BPFTOOL)
    set(CMAKE_BPFTOOL bpftool)
endif()

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/.gen/vmlinux/vmlinux.h
    COMMAND ${CMAKE_BPFTOOL}
    ARGS btf dump file /sys/kernel/btf/vmlinux format c > "${CMAKE_BINARY_DIR}/.gen/vmlinux/vmlinux.h"
)
add_custom_target(vmlinux.h
    DEPENDS "${CMAKE_BINARY_DIR}/.gen/vmlinux/vmlinux.h"
    COMMENT "Generating vmlinux.h"
)

macro(add_bpf TARGET SOURCE)
    set(BPF_C_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE})
    set(BPF_SKEL_FILE ${CMAKE_BINARY_DIR}/.gen/${TARGET}/${TARGET}.skel.h)
    set_property(SOURCE ${BPF_C_FILE} APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_BINARY_DIR}/.gen/vmlinux/vmlinux.h")

    add_library(${TARGET}.bpf OBJECT ${BPF_C_FILE})
    target_include_directories(${TARGET}.bpf PUBLIC ${CMAKE_BINARY_DIR}/.gen/vmlinux)
    target_compile_options(${TARGET}.bpf BEFORE PRIVATE -target bpf -g)
    target_compile_options(${TARGET}.bpf PRIVATE -O2)

    add_custom_command(OUTPUT ${BPF_SKEL_FILE}
        COMMAND ${CMAKE_BPFTOOL}
        ARGS gen skeleton $<TARGET_OBJECTS:${TARGET}.bpf> > ${BPF_SKEL_FILE}
        DEPENDS $<TARGET_OBJECTS:${TARGET}.bpf>
    )
    add_custom_target(${TARGET}.skel.h
        DEPENDS ${BPF_SKEL_FILE}
        COMMENT "Generating ${TARGET}.skel.h"
    )

    add_library(${TARGET} INTERFACE)
    add_dependencies(${TARGET} ${TARGET}.skel.h)
    target_sources(${TARGET} INTERFACE ${BPF_SKEL_FILE})
    target_include_directories(${TARGET} INTERFACE ${CMAKE_BINARY_DIR}/.gen/${TARGET} ${CMAKE_BINARY_DIR}/.gen/vmlinux)
    target_link_libraries(${TARGET} INTERFACE bpf elf z)
endmacro()
