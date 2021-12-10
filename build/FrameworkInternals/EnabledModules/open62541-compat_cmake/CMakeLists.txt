cmake_minimum_required(VERSION 2.8)

#message ("Configuring optional module open62541-compat")
project(open62541-compat_download NONE)
include(ExternalProject)
ExternalProject_Add(
    open62541-compat
    GIT_REPOSITORY https://github.com/quasar-team/open62541-compat.git
    GIT_TAG v1.3.6
    PREFIX "/home/root/quasar/opcua-server/build/FrameworkInternals/EnabledModules/open62541-compat_download"
    SOURCE_DIR "/home/root/quasar/opcua-server/build/open62541-compat"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    TEST_COMMAND ""
    INSTALL_COMMAND ""
    LOG_UPDATE 1              # Wrap update in script to log output
    LOG_CONFIGURE 1           # Wrap configure in script to log output
    LOG_BUILD 1               # Wrap build in script to log output
    LOG_TEST 1                # Wrap test in script to log output
    LOG_INSTALL 1 
)
#message ("Configure of optional module open62541-compat finished")
