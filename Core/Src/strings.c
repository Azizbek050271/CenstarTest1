// /Src/strings.c
#include "strings.h"
#include <stdio.h>

// Главный заголовок
const char* Str_MenuTitle_Main(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Главное меню:";
    case LANG_UZ: return "Asosiy menyu:";  // заглушка
    default:      return "Main menu:";
    }
}

// Значение для пункта главного меню
const char* Str_MainMenu_Value(language_id_t lang, uint8_t idx, const eeprom_config_t *cfg) {
    static char buf[24];
    buf[0] = '\0';
    switch (idx) {
    case 0:  // Protocol
        switch (lang) {
        case LANG_RU: snprintf(buf,21,"Протокол=%s", cfg->protocol==PROTO_ID_GASKITLINK?"GKitL":"AZT2"); break;
        case LANG_UZ: snprintf(buf,21,"Protokol=%s",   cfg->protocol==PROTO_ID_GASKITLINK?"GKitL":"AZT2"); break;
        default:      snprintf(buf,21,"Protocol=%s",    cfg->protocol==PROTO_ID_GASKITLINK?"GKitL":"AZT2"); break;
        }
        break;
    case 1:  // UART3
        snprintf(buf,21,"UART3=%lu", (unsigned long)cfg->reserved[0]); // заглушка: хранится в reserved[0]
        break;
    case 2:  // Price
        snprintf(buf,21,"Price=%u.%02u",
            (unsigned)(cfg->price_cent/100),
            (unsigned)(cfg->price_cent%100));
        break;
    case 3:  // PostAddr
        snprintf(buf,21,"Post=%02u", (unsigned)cfg->reserved[1]); // заглушка
        break;
    case 4:  // NozzleAddr
        snprintf(buf,21,"Nozzle=%02u", (unsigned)cfg->reserved[2]); // заглушка
        break;
    case 5:  // Language
        switch (cfg->language_id) {
        case LANG_RU: snprintf(buf,21,"Language=RU"); break;
        case LANG_UZ: snprintf(buf,21,"Language=UZ"); break;
        default:      snprintf(buf,21,"Language=EN"); break;
        }
        break;
    case 6:  // Save & Exit
        switch (lang) {
        case LANG_RU: return "Сохранить и выйти";
        case LANG_UZ: return "Saqlash & Chiqish";  // заглушка
        default:      return "Save & Exit";
        }
    case 7:  // Cancel
        switch (lang) {
        case LANG_RU: return "Отмена";
        case LANG_UZ: return "Bekor qilish";      // заглушка
        default:      return "Cancel";
        }
    }
    return buf;
}

// Заголовок подменю протоколов
const char* Str_MenuTitle_Protocol(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Выбор протокола:";
    case LANG_UZ: return "Protokol tanlash:";  // заглушка
    default:      return "Select protocol:";
    }
}

// Значения в подменю протоколов
const char* Str_ProtocolMenu_Value(language_id_t lang, uint8_t idx) {
    static const char *names_en[] = { "GasKitLink", "AZT2.0" };
    static const char *names_ru[] = { "GasKitLink", "АЗТ2.0" };
    static const char *names_uz[] = { "GasKitLink", "AZT2.0" };
    if (idx > 1) idx = 0;
    switch (lang) {
    case LANG_RU: return names_ru[idx];
    case LANG_UZ: return names_uz[idx];
    default:      return names_en[idx];
    }
}

// Заголовок UART3
const char* Str_MenuTitle_UART3(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Скорость UART3:";
    case LANG_UZ: return "UART3 tezligi:";     // заглушка
    default:      return "UART3 baud:";
    }
}

// Заголовок цены
const char* Str_MenuTitle_Price(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Цена за литр:";
    case LANG_UZ: return "Narx litr:";         // заглушка
    default:      return "Price per L:";
    }
}

// Заголовок адреса поста
const char* Str_MenuTitle_PostAddr(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Адрес поста:";
    case LANG_UZ: return "Stansiya adi:";      // заглушка
    default:      return "Post address:";
    }
}

// Заголовок адреса рукава
const char* Str_MenuTitle_NozzleAddr(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Адрес рукава:";
    case LANG_UZ: return "Shlangi adi:";       // заглушка
    default:      return "Nozzle address:";
    }
}

// Заголовок выбора языка
const char* Str_MenuTitle_Language(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Выбор языка:";
    case LANG_UZ: return "Tilni tanlash:";     // заглушка
    default:      return "Select language:";
    }
}

// Значения подменю языков
const char* Str_LanguageMenu_Value(language_id_t lang, uint8_t idx) {
    static const char *vals_en[] = { "English", "Russian", "Uzbek" };
    static const char *vals_ru[] = { "Английский", "Русский", "Узбекский" };
    static const char *vals_uz[] = { "English", "Russian", "Uzbek" };
    if (idx > 2) idx = 0;
    switch (lang) {
    case LANG_RU: return vals_ru[idx];
    case LANG_UZ: return vals_uz[idx];
    default:      return vals_en[idx];
    }
}
