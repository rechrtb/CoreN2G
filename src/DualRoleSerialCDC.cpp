
extern "C" {
#include "tusb.h"
}

#include "DualRoleSerialCDC.h"

int DualRoleSerialCDC::available() noexcept
{
    if (!CoreUsbGetMode())
    {
        return SerialCDC::available();
    }
    return 0;
}

int DualRoleSerialCDC::read() noexcept
{
    if (!CoreUsbGetMode())
    {
        return SerialCDC::read();
    }
    return 0;
}

size_t DualRoleSerialCDC::readBytes(char * _ecv_array buffer, size_t length) noexcept
{
    if (!CoreUsbGetMode())
    {
        return SerialCDC::readBytes(buffer, length);
    }

    return 0;
}

void DualRoleSerialCDC::flush() noexcept
{
    if (!CoreUsbGetMode())
    {
        SerialCDC::flush();
    }
}

size_t DualRoleSerialCDC::write(uint8_t c) noexcept
{
    if (!CoreUsbGetMode())
    {
        return SerialCDC::write(c);
    }

    return 0;
}

size_t DualRoleSerialCDC::write(const uint8_t *_ecv_array buffer, size_t size) noexcept
{
    if (!CoreUsbGetMode())
    {
        return SerialCDC::write(buffer, size);
    }

    return 0;
}

void DualRoleSerialCDC::Start(Pin p_vBusPin) noexcept
{
    if (!CoreUsbGetMode())
    {
        return SerialCDC::Start(p_vBusPin);
    }
}

void DualRoleSerialCDC::end(void) noexcept
{
    if (!CoreUsbGetMode())
    {
        SerialCDC::end();
    }
}

void DualRoleSerialCDC::setHostMode(bool hostMode) noexcept
{
	CoreUsbSetHostMode(hostMode);
}

size_t DualRoleSerialCDC::canWrite() noexcept
{
    if (!CoreUsbGetMode())
    {
        return SerialCDC::canWrite();
    }

    return 0;
}

bool DualRoleSerialCDC::IsConnected() const noexcept
{
    return !CoreUsbGetMode() && SerialCDC::IsConnected();
}
