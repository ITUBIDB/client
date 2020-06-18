set( APPLICATION_NAME       "Kovan" )
set( APPLICATION_SHORTNAME  "Kovan" )
set( APPLICATION_EXECUTABLE "Kovan" )
set( APPLICATION_DOMAIN     "kovan.itu.edu.tr" )
set( APPLICATION_VENDOR     "ITU BIDB" )
set( APPLICATION_UPDATE_URL "https://kovan.itu.edu.tr/kovan-clients/update.php" CACHE STRING "URL for updater" )
set( APPLICATION_ICON_NAME  "owncloud" )
set( APPLICATION_VIRTUALFILE_SUFFIX "Kovan" CACHE STRING "Virtual file suffix (not including the .)")

set( LINUX_PACKAGE_SHORTNAME "kovan" )

set( THEME_CLASS            "ownCloudTheme" )
set( APPLICATION_REV_DOMAIN "tr.edu.itu.kovan" )
set( WIN_SETUP_BITMAP_PATH  "${CMAKE_SOURCE_DIR}/admin/win/nsi" )

set( MAC_INSTALLER_BACKGROUND_FILE "${CMAKE_SOURCE_DIR}/admin/osx/installer-background.png" CACHE STRING "The MacOSX installer background image")

# set( THEME_INCLUDE          "${OEM_THEME_DIR}/mytheme.h" )
# set( APPLICATION_LICENSE    "${OEM_THEME_DIR}/license.txt )

option( WITH_CRASHREPORTER "Build crashreporter" OFF )
set( CRASHREPORTER_SUBMIT_URL "http://yardim.itu.edu.tr" CACHE STRING "URL for crash reporter" )

