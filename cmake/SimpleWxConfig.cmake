macro(SimpleWxConfig)
 if(NOT QT_ANDROID)
    set( wxWidgets_USE_DEBUG OFF)
    set( wxWidgets_USE_UNICODE ON)
    set( wxWidgets_USE_UNIVERSAL OFF)
    set( wxWidgets_USE_STATIC OFF)
    set(wxUSE_STD_CONTAINERS OFF CACHE INTERNAL "Disable STL containers for wxWidgets")
    if(OCPN_WXWIDGETS_FORCE_VERSION)
      set (wxWidgets_CONFIG_OPTIONS --version=${OCPN_WXWIDGETS_FORCE_VERSION})
    endif()
    # 关键修改：添加编译宏，强制使用 wx 容器（禁用 STL 容器）
    add_definitions(-DwxUSE_STL=0)
    add_definitions(-DwxUSE_STD_CONTAINERS=0)  # 进一步确保不使用 STL 容器
    
    if(MSVC)
        # Exclude wxexpat.lib, since we use our own version.
        # Other things are excluded as well, but we don't need them
        SET(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)
    endif(MSVC)
    set(wxWidgets_USE_LIBS base core xml html)
    find_package(GTK2)
    if(GTK2_FOUND)
        message("FIND GTK2")
        set(wxWidgets_CONFIG_OPTIONS
            ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk2)
    else ()
        find_package(GTK3)
        if(GTK3_FOUND)
            message("FIND GTK3")
            set(wxWidgets_CONFIG_OPTIONS
                ${wxWidgets_CONFIG_OPTIONS} --toolkit=gtk3)
        endif ()
    endif ()
    # set(wxWidgets_ROOT_DIR "D:/MAP_SDK/wxWidgets-3.2.8") # STL容器
    # set(wxWidgets_ROOT_DIR "D:/wxWidgets-3.2.8-ucrt-STL") # 编译时选定了wx容器
    set(wxWidgets_CONFIG_OPTIONS
            ${wxWidgets_CONFIG_OPTIONS} --toolkit=msw)
    find_package(wxWidgets REQUIRED)
    INCLUDE(${wxWidgets_USE_FILE})
    message("wxWidgets_INCLUDE_DIRS: ${wxWidgets_INCLUDE_DIRS}")
 endif(NOT QT_ANDROID)
endmacro(SimpleWxConfig)
    
    # set(wxWidgets_ROOT_DIR "/ucrt64")
    # set(wxWidgets_CONFIG_OPTIONS "--prefix=/ucrt64")
    # set(wxWidgets_ROOT_DIR "D:/SOFTWARES/msys64/ucrt64")  # 替换为你的 wxWidgets 根目录
    # find_package(wxWidgets REQUIRED)
    # 显式设置头文件和库路径（MSYS2 UCRT64 默认路径）
    # set(wxWidgets_INCLUDE_DIRS
    # "/ucrt64/include/wx-3.2"      # 基础头文件
    # "/ucrt64/lib/wx/include/msvc-unicode-static-3.2"  # 平台配置头文件
    # )
    # set(wxWidgets_LIBRARIES
    # "/ucrt64/lib/libwx_baseu-3.2.a"
    # "/ucrt64/lib/libwx_mswu_core-3.2.a"
    # )
    # set(wxWidgets_FOUND TRUE)  # 标记为已找到
    # set(wxWidgets_USE_FILE "/ucrt64/share/cmake/wxWidgets/wxWidgets.cmake")
    # INCLUDE(${wxWidgets_USE_FILE})
    # message("wxWidgets_INCLUDE_DIRS: ${wxWidgets_INCLUDE_DIRS}")

