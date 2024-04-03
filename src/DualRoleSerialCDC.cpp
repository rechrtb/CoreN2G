
extern "C" {
#include "tusb.h"
}

#include <Core.h>

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

void DualRoleSerialCDC::Start(Pin p_vBusPin, Pin p_powSwPin) noexcept
{
	if (!CoreUsbGetMode())
	{
		powSwPin = p_powSwPin;
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

bool DualRoleSerialCDC::setHostMode(bool hostMode, const StringRef& reply) noexcept
{
	if (hostMode && digitalRead(vBusPin))
	{
		if (digitalRead(vBusPin))
		{
			reply.printf("Board still plugged in to host\n");
			return false;
		}
	}

	digitalWrite(powSwPin, hostMode);
	CoreUsbSetHostMode(hostMode);
	return true;
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
