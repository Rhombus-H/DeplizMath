add_library(deplizmath_lib INTERFACE)
target_include_directories(deplizmath_lib INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(deplizmath_lib INTERFACE
)

add_executable(DeplizMath main.cpp)
target_link_libraries(DeplizMath PRIVATE deplizmath_lib)

if(WIN32)
    target_link_libraries(DeplizMath PRIVATE opengl32 winmm)

    if(BUILD_SHARED_LIBS)
        add_custom_command(TARGET DeplizMath POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:sfml-graphics> $<TARGET_FILE_DIR:DeplizMath>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:sfml-window>   $<TARGET_FILE_DIR:DeplizMath>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:sfml-system>   $<TARGET_FILE_DIR:DeplizMath>
        )
    endif()
endif()

if(APPLE)
    target_link_libraries(DeplizMath PRIVATE
            "-framework OpenGL"
            "-framework Foundation"
            "-framework AppKit"
    )
endif()
