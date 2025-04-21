#include "strings.h"
#include "config.h"
#include <stdio.h>
#include <string.h>

const char* Str_MenuTitle_Main(language_id_t lang) {
    switch (lang) {
    case LANG_EN: return "Main Menu";
    case LANG_RU: return "Главное меню";
    case LANG_UZ: return "Asosiy menyu";
    default:      return "Menu";
    }
}

const char* Str_MainMenu_Value(language_id_t lang, uint8_t index, const eeprom_config_t* cfg) {
    static char buf[21];
    switch (index) {
    case 0:
        snprintf(buf, sizeof(buf), lang == LANG_RU ? "Протокол:%s" : "Protocol:%s",
                 cfg->protocol == PROTO_ID_GASKITLINK ? "GKitL" : "AZT2");
        break;
    case 1:
        snprintf(buf, sizeof(buf), lang == LANG_RU ? "UART3:%lu" : "UART3:%lu",
                 (unsigned long)cfg->uart3_baud);
        break;
    case 2:
        snprintf(buf, sizeof(buf), lang == LANG_RU ? "Цена:%u.%02u" : "Price:%u.%02u",
                 (unsigned)(cfg->price_cent / 100), (unsigned)(cfg->price_cent % 100));
        break;
    case 3:
        snprintf(buf, sizeof(buf), lang == LANG_RU ? "Пост:%u" : "Post:%u",
                 cfg->post_addr);
        break;
    case 4:
        snprintf(buf, sizeof(buf), lang == LANG_RU ? "Рукав:%u" : "Nozzle:%u",
                 cfg->nozzle_addr);
        break;
    case 5:
        switch (cfg->language_id) {
        case LANG_RU: snprintf(buf, sizeof(buf), "Язык:RU"); break;
        case LANG_UZ: snprintf(buf, sizeof(buf), "Язык:UZ"); break;
        default:      snprintf(buf, sizeof(buf), "Language:EN"); break;
        }
        break;
    case 6:
        snprintf(buf, sizeof(buf), lang == LANG_RU ? "Сохранить" : "Save");
        break;
    case 7:
        snprintf(buf, sizeof(buf), lang == LANG_RU ? "Отмена" : "Cancel");
        break;
    default:
        buf[0] = '\0';
        break;
    }
    return buf;
}

const char* Str_MenuTitle_Protocol(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Выбор протокола:";
    case LANG_UZ: return "Protokol tanlash:";
    default:      return "Select protocol:";
    }
}

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

const char* Str_MenuTitle_UART3(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Скорость UART3:";
    case LANG_UZ: return "UART3 tezligi:";
    default:      return "UART3 baud:";
    }
}

const char* Str_MenuTitle_Price(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Цена за литр:";
    case LANG_UZ: return "Narx litr:";
    default:      return "Price per L:";
    }
}

const char* Str_MenuTitle_PostAddr(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Адрес поста:";
    case LANG_UZ: return "Stansiya adi:";
    default:      return "Post address:";
    }
}

const char* Str_MenuTitle_NozzleAddr(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Адрес рукава:";
    case LANG_UZ: return "Shlangi adi:";
    default:      return "Nozzle address:";
    }
}

const char* Str_MenuTitle_Language(language_id_t lang) {
    switch (lang) {
    case LANG_RU: return "Выбор языка:";
    case LANG_UZ: return "Tilni tanlash:";
    default:      return "Select language:";
    }
}

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

const char* Str_MenuSavedMessage(language_id_t lang, const char *field) {
    static char msg[21];
    if (lang == LANG_RU) snprintf(msg, sizeof(msg), "Сохранено: %s", field);
    else                 snprintf(msg, sizeof(msg), "Saved: %s", field);
    return msg;
}
