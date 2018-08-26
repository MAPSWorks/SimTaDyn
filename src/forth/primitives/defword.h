#define ALL_DEFWORD_TOKENS                          \
  TOK_LBRACKET,                                     \
    TOK_RBRACKET,                                   \
    TOK_EXECUTE,                                    \
    TOK_COMPILE,                                    \
    TOK_ICOMPILE,                                   \
    TOK_POSTPONE,                                   \
    TOK_COLON,                                      \
    TOK_DOCOL,                                      \
    TOK_SEMICOLON,                                  \
    TOK_DOSEMI,                                     \
    TOK_LITERAL16,                                  \
    TOK_LITERAL32,                                  \
    TOK_IMMEDIATE,                                  \
    TOK_SMUDGE,                                     \
    TOK_LAST,                                       \
    TOK_CREATE,                                     \
    TOK_BUILDS,                                     \
    TOK_DOES,                                       \
    TOK_TICK

#define ALL_DEFWORD_LABELS                                  \
  LABELIZE(LBRACKET),                                       \
    LABELIZE(RBRACKET),                                     \
    LABELIZE(EXECUTE),                                      \
    LABELIZE(COMPILE),                                      \
    LABELIZE(ICOMPILE),                                     \
    LABELIZE(POSTPONE),                                     \
    LABELIZE(COLON),                                        \
    LABELIZE(DOCOL),                                        \
    LABELIZE(SEMICOLON),                                    \
    LABELIZE(DOSEMI),                                       \
    LABELIZE(LITERAL16),                                    \
    LABELIZE(LITERAL32),                                    \
    LABELIZE(IMMEDIATE),                                    \
    LABELIZE(SMUDGE),                                       \
    LABELIZE(LAST),                                         \
    LABELIZE(CREATE),                                       \
    LABELIZE(BUILDS),                                       \
    LABELIZE(DOES),                                         \
    LABELIZE(TICK)

#define ALL_DEFWORD_PRIMITIVES                   \
  PRIMITIVE(LBRACKET, "[");                                       \
    PRIMITIVE(RBRACKET, "]");                                     \
    PRIMITIVE(EXECUTE, "EXECUTE");                                      \
    PRIMITIVE(COMPILE, "COMPILE");                                      \
    PRIMITIVE(ICOMPILE, "[COMPILE]");                                     \
    PRIMITIVE(POSTPONE, "POSTPONE");                                     \
    PRIMITIVE(COLON, ":");                                        \
    PRIMITIVE(DOCOL, "(:)");                                        \
    PRIMITIVE(SEMICOLON, ";");                                    \
    PRIMITIVE(DOSEMI, "(;)");                                       \
    PRIMITIVE(LITERAL16, "LIT16");                                    \
    PRIMITIVE(LITERAL32, "LIT32");                                    \
    PRIMITIVE(IMMEDIATE, "IMMEDIATE");                                    \
    PRIMITIVE(SMUDGE, "SMUDGE");                                       \
    PRIMITIVE(LAST, "LAST");                                         \
    PRIMITIVE(CREATE, "CREATE");                                       \
    PRIMITIVE(BUILDS, "<BUILDS");                                       \
    PRIMITIVE(DOES, "DOES>");                                         \
    PRIMITIVE(TICK, "'");
