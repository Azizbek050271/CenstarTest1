// /Inc/commands_gaskit.h
#ifndef COMMANDS_GASKIT_H
#define COMMANDS_GASKIT_H

#include <stdint.h>
#include <stddef.h>

/** Начальный байт кадра по спецификации GasKitLink */
#define GASKIT_START_BYTE    0x02

/** Вычисление контрольной суммы XOR от buf[1] до buf[len-2] включительно */
uint8_t Gaskit_CalcChecksum(const uint8_t *buf, size_t len);

/**
 * Формирует кадр GasKitLink:
 *  buf[0] = GASKIT_START_BYTE
 *  buf[1..2] = 16‑битный адрес (big-endian)
 *  buf[3] = командный символ
 *  buf[4..4+data_len-1] = data
 *  buf[4+data_len] = checksum
 * @param buf буфер не менее (5 + data_len) байт
 * @param address адрес ведомой станции (0x0001…0x0020)
 * @param cmd ASCII‑код команды
 * @param data указатель на data_len байт данных (ASCII‑цифры, ‘;’ и т.п.)
 * @param data_len длина data
 * @return общий размер кадра
 */
size_t Gaskit_BuildFrame(uint8_t *buf,
                         uint16_t address,
                         char    cmd,
                         const uint8_t *data,
                         size_t data_len);

// Специфичные генераторы команд:

size_t Gaskit_ReqStatus(uint8_t *buf, uint16_t address);
size_t Gaskit_StartVolume(uint8_t *buf, uint16_t address,
                          uint8_t nozzle,
                          uint32_t volume_cl,
                          uint16_t price);
size_t Gaskit_StartMoney(uint8_t *buf, uint16_t address,
                         uint8_t nozzle,
                         uint32_t money_cent,
                         uint16_t price);
size_t Gaskit_ReqResume(uint8_t *buf, uint16_t address);
size_t Gaskit_ReqRealVolume(uint8_t *buf, uint16_t address);
size_t Gaskit_ReqRealMoney(uint8_t *buf, uint16_t address);
size_t Gaskit_ReqTransactionData(uint8_t *buf, uint16_t address);
size_t Gaskit_ReqTotals(uint8_t *buf, uint16_t address, uint8_t nozzle);
size_t Gaskit_EndTransaction(uint8_t *buf, uint16_t address);
size_t Gaskit_Stop(uint8_t *buf, uint16_t address);
size_t Gaskit_SetParam(uint8_t *buf, uint16_t address,
                       uint8_t param_id,
                       uint16_t value);
size_t Gaskit_ReqParam(uint8_t *buf, uint16_t address, uint8_t param_id);
size_t Gaskit_ChannelSwitch(uint8_t *buf, uint8_t channel);

#endif // COMMANDS_GASKIT_H
