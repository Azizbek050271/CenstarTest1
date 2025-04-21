// /Inc/strings.h
#ifndef STRINGS_H
#define STRINGS_H

#include "eeprom_storage.h"

const char* Str_MenuTitle_Main(language_id_t lang);
const char* Str_MainMenu_Value(language_id_t lang, uint8_t index, const eeprom_config_t *cfg);

const char* Str_MenuTitle_Protocol(language_id_t lang);
const char* Str_ProtocolMenu_Value(language_id_t lang, uint8_t index);

const char* Str_MenuTitle_UART3(language_id_t lang);
const char* Str_MenuTitle_Price(language_id_t lang);
const char* Str_MenuTitle_PostAddr(language_id_t lang);
const char* Str_MenuTitle_NozzleAddr(language_id_t lang);

const char* Str_MenuTitle_Language(language_id_t lang);
const char* Str_LanguageMenu_Value(language_id_t lang, uint8_t index);

#endif // STRINGS_H
