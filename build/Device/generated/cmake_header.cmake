
add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/Device/generated/Base_All.cpp
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
COMMAND ${PYTHON_COMMAND} quasar.py generate base_cpp_all --project_binary_dir ${PROJECT_BINARY_DIR}
DEPENDS
  ${DESIGN_FILE}
  ${PROJECT_SOURCE_DIR}/quasar.py
  Configuration.hxx_GENERATED
  validateDesign
  templates/commonDeviceTemplates.jinja
  ${QUASAR_TRANSFORM_SENSITIVE_FILES}
  templates/designToDeviceBaseBody.jinja

)

add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/Device/generated/Base_DRegs.h
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
COMMAND ${PYTHON_COMMAND} quasar.py generate base_h Regs --project_binary_dir ${PROJECT_BINARY_DIR}
DEPENDS
  ${DESIGN_FILE}
  validateDesign
  ${PROJECT_SOURCE_DIR}/quasar.py
  ${PROJECT_SOURCE_DIR}/Device/templates/designToDeviceBaseHeader.jinja
  Configuration.hxx_GENERATED
  templates/commonDeviceTemplates.jinja
  ${QUASAR_TRANSFORM_SENSITIVE_FILES}
)

set(DEVICEBASE_GENERATED_FILES
      include/DRoot.h
      src/DRoot.cpp
      generated/Base_All.cpp
            generated/Base_DRegs.h
      )

set(DEVICE_CLASSES
    src/DRegs.cpp
  )

add_custom_target(DeviceBase DEPENDS ${DEVICEBASE_GENERATED_FILES} )

add_custom_target(DeviceGeneratedHeaders DEPENDS include/DRoot.h ${DEVICEBASE_GENERATED_FILES})