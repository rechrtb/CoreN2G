
extern "C" {
#include "tusb.h"
}

#include "DualRoleSerialCDC.h"

int DualRoleSerialCDC::available() noexcept
{
    if (!usbHostMode)
    {
        return SerialCDC::available();
    }
    return 0;
}

int DualRoleSerialCDC::read() noexcept
{
    if (!usbHostMode)
    {
        return SerialCDC::read();
    }
    return 0;
}

size_t DualRoleSerialCDC::readBytes(char * _ecv_array buffer, size_t length) noexcept
{
    if (!usbHostMode)
    {
        return SerialCDC::readBytes(buffer, length);
    }

    return 0;
}

void DualRoleSerialCDC::flush() noexcept
{
    if (!usbHostMode)
    {
        SerialCDC::flush();
    }
}

size_t DualRoleSerialCDC::write(uint8_t c) noexcept
{
    if (!usbHostMode)
    {
        return SerialCDC::write(c);
    }

    return 0;
}

size_t DualRoleSerialCDC::write(const uint8_t *_ecv_array buffer, size_t size) noexcept
{
    if (!usbHostMode)
    {
        return SerialCDC::write(buffer, size);
    }

    return 0;
}

void DualRoleSerialCDC::Start(Pin p_vBusPin) noexcept
{
    if (!usbHostMode)
    {
        return SerialCDC::Start(p_vBusPin);
    }
}

void DualRoleSerialCDC::end(void) noexcept
{
    if (!usbHostMode)
    {
        SerialCDC::end();
    }
}

void DualRoleSerialCDC::setHostMode(bool hostMode) noexcept
{
    usbHostMode = hostMode;
}

size_t DualRoleSerialCDC::canWrite() noexcept
{
    if (!usbHostMode)
    {
        return SerialCDC::canWrite();
    }

    return 0;
}

bool DualRoleSerialCDC::IsConnected() const noexcept
{
    return !usbHostMode && SerialCDC::IsConnected();
}
