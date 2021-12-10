
# /*  © Copyright CERN, 2015. All rights not expressly granted are reserved.
# 
#     This file was generated by quasar (https://github.com/quasar-team/quasar/)
# 
#     Quasar is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Lesser General Public Licence as published by
#     the Free Software Foundation, either version 3 of the Licence.
#     Quasar is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU Lesser General Public Licence for more details.
# 
#     You should have received a copy of the GNU Lesser General Public License
#     along with Quasar.  If not, see <http://www.gnu.org/licenses/>.
# 
#  */
# 

  add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/AddressSpace/include/ASRegs.h
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMAND python3 quasar.py generate asclass Regs --project_binary_dir ${PROJECT_BINARY_DIR}
  DEPENDS ${DESIGN_FILE} ${PROJECT_SOURCE_DIR}/AddressSpace/templates/designToClassHeader.jinja Configuration.hxx_GENERATED validateDesign ${QUASAR_TRANSFORM_SENSITIVE_FILES}
  )

add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/AddressSpace/src/AddressSpaceClasses.cpp
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
COMMAND ${PYTHON_COMMAND} quasar.py generate asclass_cpp_all --project_binary_dir ${PROJECT_BINARY_DIR}
DEPENDS ${DESIGN_FILE} ${PROJECT_SOURCE_DIR}/AddressSpace/templates/designToClassBody.jinja Configuration.hxx_GENERATED validateDesign
)

set(ADDRESSSPACE_CLASSES
${PROJECT_BINARY_DIR}/AddressSpace/src/AddressSpaceClasses.cpp
)

set(ADDRESSSPACE_HEADERS
  ${PROJECT_BINARY_DIR}/AddressSpace/include/ASRegs.h
)