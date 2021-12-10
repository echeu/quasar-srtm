
if(NOT "/home/root/quasar/opcua-server/build/FrameworkInternals/EnabledModules/open62541-compat_download/src/open62541-compat-stamp/open62541-compat-gitinfo.txt" IS_NEWER_THAN "/home/root/quasar/opcua-server/build/FrameworkInternals/EnabledModules/open62541-compat_download/src/open62541-compat-stamp/open62541-compat-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/root/quasar/opcua-server/build/FrameworkInternals/EnabledModules/open62541-compat_download/src/open62541-compat-stamp/open62541-compat-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/root/quasar/opcua-server/build/open62541-compat"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/root/quasar/opcua-server/build/open62541-compat'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --config "advice.detachedHead=false" "https://github.com/quasar-team/open62541-compat.git" "open62541-compat"
    WORKING_DIRECTORY "/home/root/quasar/opcua-server/build"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/quasar-team/open62541-compat.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout v1.3.6 --
  WORKING_DIRECTORY "/home/root/quasar/opcua-server/build/open62541-compat"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'v1.3.6'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/root/quasar/opcua-server/build/open62541-compat"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/root/quasar/opcua-server/build/open62541-compat'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/root/quasar/opcua-server/build/FrameworkInternals/EnabledModules/open62541-compat_download/src/open62541-compat-stamp/open62541-compat-gitinfo.txt"
    "/home/root/quasar/opcua-server/build/FrameworkInternals/EnabledModules/open62541-compat_download/src/open62541-compat-stamp/open62541-compat-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/root/quasar/opcua-server/build/FrameworkInternals/EnabledModules/open62541-compat_download/src/open62541-compat-stamp/open62541-compat-gitclone-lastrun.txt'")
endif()

