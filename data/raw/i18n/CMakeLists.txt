set(AKAGORIA_LANGUAGES
  fr
)

if(L10N_FOUND)
  gettext_xgettext("akagoria"
    FILES "${CMAKE_CURRENT_BINARY_DIR}/../../strings.cc"
  )

  gettext_merge("akagoria.pot"
    ALL
    LANGUAGES ${AKAGORIA_LANGUAGES}
  )

  gettext_fmt("akagoria"
    ALL
    LANGUAGES ${AKAGORIA_LANGUAGES}
    INSTALL_DESTINATION "${CMAKE_INSTALL_LOCALEDIR}"
  )
endif()
