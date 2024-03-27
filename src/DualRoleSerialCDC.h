#pragma once

#include "SerialCDC.h"
#include "TinyUsbInterface.h"

class DualRoleSerialCDC : public SerialCDC
{
public:
    void Start(Pin p_vBusPin) noexcept override;
	void end(void) noexcept override;

	int available() noexcept override;
	int read() noexcept override;
	size_t readBytes(char * _ecv_array buffer, size_t length) noexcept override;
	void flush() noexcept override;
	size_t write(uint8_t) noexcept override;
	size_t write(const uint8_t *_ecv_array buffer, size_t size) noexcept override;

    size_t canWrite() noexcept override;
	bool IsConnected() const noexcept override;

    void setHostMode(bool host);

private:
};