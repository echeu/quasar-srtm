file(REMOVE_RECURSE
  "bin/OpcUaServer"
  "bin/OpcUaServer.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C CXX)
  include(CMakeFiles/OpcUaServer.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
