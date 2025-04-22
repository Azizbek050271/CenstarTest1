// /Src/commands_gaskit.c
#include "commands_gaskit.h"
#include <string.h>
#include <stdio.h>

uint8_t Gaskit_CalcChecksum(const uint8_t *buf, size_t len) {
    uint8_t cs = 0;
    // XOR от buf[1] до buf[len-2]
    for (size_t i = 1; i + 1 < len; i++) {
        cs ^= buf[i];
    }
    return cs;
}

size_t Gaskit_BuildFrame(uint8_t *buf,
                         uint16_t address,
                         char    cmd,
                         const uint8_t *data,
                         size_t data_len)
{
    size_t idx = 0;
    buf[idx++] = GASKIT_START_BYTE;
    buf[idx++] = (uint8_t)((address >> 8) & 0xFF);
    buf[idx++] = (uint8_t)(address & 0xFF);
    buf[idx++] = (uint8_t)cmd;
    if (data && data_len) {
        memcpy(&buf[idx], data, data_len);
        idx += data_len;
    }
    buf[idx++] = Gaskit_CalcChecksum(buf, idx );
    return idx;
}

size_t Gaskit_ReqStatus(uint8_t *buf, uint16_t address) {
    return Gaskit_BuildFrame(buf, address, 'S', NULL, 0);
}

size_t Gaskit_StartVolume(uint8_t *buf, uint16_t address,
                          uint8_t nozzle,
                          uint32_t volume_cl,
                          uint16_t price)
{
    char data[1 + 1 + 6 + 1 + 4]; // nozzle + ';' + 6 цифр + ';' + 4 цифры
    int len = snprintf(data, sizeof(data), "%u;%06lu;%04u",
                       nozzle, (unsigned long)volume_cl, (unsigned)price);
    return Gaskit_BuildFrame(buf, address, 'V',
                             (uint8_t*)data, (size_t)len);
}

size_t Gaskit_StartMoney(uint8_t *buf, uint16_t address,
                         uint8_t nozzle,
                         uint32_t money_cent,
                         uint16_t price)
{
    char data[1 + 1 + 6 + 1 + 4];
    int len = snprintf(data, sizeof(data), "%u;%06lu;%04u",
                       nozzle, (unsigned long)money_cent, (unsigned)price);
    return Gaskit_BuildFrame(buf, address, 'M',
                             (uint8_t*)data, (size_t)len);
}

size_t Gaskit_ReqResume(uint8_t *buf, uint16_t address) {
    return Gaskit_BuildFrame(buf, address, 'G', NULL, 0);
}

size_t Gaskit_ReqRealVolume(uint8_t *buf, uint16_t address) {
    return Gaskit_BuildFrame(buf, address, 'L', NULL, 0);
}

size_t Gaskit_ReqRealMoney(uint8_t *buf, uint16_t address) {
    return Gaskit_BuildFrame(buf, address, 'R', NULL, 0);
}

size_t Gaskit_ReqTransactionData(uint8_t *buf, uint16_t address) {
    return Gaskit_BuildFrame(buf, address, 'T', NULL, 0);
}

size_t Gaskit_ReqTotals(uint8_t *buf, uint16_t address, uint8_t nozzle) {
    char data[2];
    int len = snprintf(data, sizeof(data), "%u", nozzle);
    return Gaskit_BuildFrame(buf, address, 'C',
                             (uint8_t*)data, (size_t)len);
}

size_t Gaskit_EndTransaction(uint8_t *buf, uint16_t address) {
    return Gaskit_BuildFrame(buf, address, 'N', NULL, 0);
}

size_t Gaskit_Stop(uint8_t *buf, uint16_t address) {
    return Gaskit_BuildFrame(buf, address, 'B', NULL, 0);
}

size_t Gaskit_SetParam(uint8_t *buf, uint16_t address,
                       uint8_t param_id,
                       uint16_t value)
{
    char data[2 + 4];
    int len = snprintf(data, sizeof(data), "%02u%04u",
                       (unsigned)param_id, (unsigned)value);
    return Gaskit_BuildFrame(buf, address, 'W',
                             (uint8_t*)data, (size_t)len);
}

size_t Gaskit_ReqParam(uint8_t *buf, uint16_t address, uint8_t param_id) {
    char data[2];
    int len = snprintf(data, sizeof(data), "%02u", (unsigned)param_id);
    return Gaskit_BuildFrame(buf, address, 'Z',
                             (uint8_t*)data, (size_t)len);
}

size_t Gaskit_ChannelSwitch(uint8_t *buf, uint8_t channel) {
    // адрес всегда 0x0000 при переключении
    char data[2];
    int len = snprintf(data, sizeof(data), "%02u", (unsigned)channel);
    return Gaskit_BuildFrame(buf, 0x0000, 'D',
                             (uint8_t*)data, (size_t)len);
}
