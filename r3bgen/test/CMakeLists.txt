generate_root_test_script(${R3BROOT_SOURCE_DIR}/r3bgen/test/testR3BPhaseSpaceGeneratorIntegration.C)
add_test(NAME testR3BPhaseSpaceGeneratorIntegration COMMAND ${R3BROOT_BINARY_DIR}/r3bgen/test/testR3BPhaseSpaceGeneratorIntegration.sh)
set_tests_properties(testR3BPhaseSpaceGeneratorIntegration PROPERTIES TIMEOUT "100")
set_tests_properties(testR3BPhaseSpaceGeneratorIntegration PROPERTIES PASS_REGULAR_EXPRESSION "Macro finished successfully.")
