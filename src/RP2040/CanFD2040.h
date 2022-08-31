/*
 * CanFD2040.h
 *
 *  Created on: 22 Aug 2022
 *      Author: David
 */

#ifndef SRC_CANFD2040_H_
#define SRC_CANFD2040_H_

#include <CoreIO.h>
#undef from				// 'from' is used as an identifier in the Pico SDK

#include "VirtualCanRegisters.h"

extern "C" {
#include <pico.h>
#include <pico/critical_section.h>
#include <hardware/structs/pio.h>				// for pio_hw_t
}

// Class to de-stuff received bits. It also maintains the CRCs because for CAN-FD the stuffing bits are included in the CRC.
class BitUnstuffer
{
public:
	void AddBits(uint32_t data, uint32_t count) noexcept;
	void SetCount(uint32_t count) noexcept;
	void ClearState() noexcept;
	int PullBits() noexcept;
	uint32_t GetStuffedBits() const noexcept { return stuffed_bits; }
	uint32_t GetUnstuffedBits() const noexcept { return unstuffed_bits; }
	uint32_t GetStuffCount() const noexcept { return count_stuff; }
	void ClearStuffedBits() noexcept { stuffed_bits = 0; }
	void ClearStuffCount() noexcept { count_stuff = 0; usingFixedStuffBits = false; }
	void UseFixedStuffBits() noexcept;
	void UseNormalStuffBits() noexcept { usingFixedStuffBits = false; }
	uint32_t GetTotalStuffBits() const noexcept { return totalStuffBits; }
	uint32_t GetCrc17() const noexcept { return crc17; }
	uint32_t GetCrc21() const noexcept { return crc21; }

private:
	uint32_t stuffed_bits;						// the last 5 (or more) bits received
	unsigned int count_stuff;					// how many unprocessed bits we have in stuffed_bits
	uint32_t unstuffed_bits;					// bits we have destuffed, but we don't have as many as asked for
	unsigned int count_unstuff;					// how may more destuffed bits we want
	uint32_t crc17;
	uint32_t crc21;
	unsigned int totalStuffBits;				// the total number of stuffing bits we removed
	unsigned int bitsUntilFixedStuffBit;		// how many bits we need to process before we expect a fixed stuffing bit
	bool doCrc17;
	bool doCrc21;
	bool usingFixedStuffBits;					// whether we are using fixed stuff bits
};

class CanFD2040
{
public:
	CanFD2040() noexcept { }

	// Start CAN-FD running
	[[noreturn]] void Entry(VirtualCanRegisters *p_regs) noexcept;
	void pio_irq_handler() noexcept;

private:
	// Low level PIO control functions
	void pio_setup() noexcept;
	void pio_sm_setup() noexcept;
	void pio_sync_setup() noexcept;
	void pio_rx_setup() noexcept;
	void pio_match_setup() noexcept;
	void pio_tx_setup() noexcept;
	void pio_sync_normal_start_signal() noexcept;
	void pio_sync_slow_start_signal() noexcept;
	int pio_rx_check_stall() noexcept;
	int pio_rx_fifo_level() noexcept;
	void pio_match_check(uint32_t match_key) noexcept;
	static uint32_t pio_match_calc_key(uint32_t raw_bits, uint32_t rx_bit_pos) noexcept;
	void pio_match_clear() noexcept;
	void pio_tx_reset() noexcept;
	void pio_tx_send(uint32_t *data, uint32_t count) noexcept;
	void pio_tx_inject_ack(uint32_t match_key) noexcept;
	int pio_tx_did_conflict() noexcept;
	void pio_irq_set_maytx() noexcept;
	void pio_irq_set_maytx_matched() noexcept;
	void pio_irq_set_maytx_ackdone() noexcept;
	void pio_irq_atomic_set_maytx() noexcept;
	void pio_irq_set_none() noexcept;

	// Transmit functions
	void tx_schedule_transmit() noexcept;
	bool tx_check_local_message() noexcept;

	void report_callback_error(uint32_t error_code) noexcept;
	void report_callback_rx_msg() noexcept;
	void report_callback_tx_msg() noexcept;
	void report_handle_eof() noexcept;
	int report_is_acking_rx() noexcept;
	void report_note_message_start() noexcept;
	void report_note_crc_start() noexcept;
	void report_note_ack_success() noexcept;
	void report_note_eof_success() noexcept;
	void report_note_parse_error() noexcept;
	void report_line_ackdone() noexcept;
	void report_line_matched() noexcept;
	void report_line_maytx() noexcept;

	// Parsing states (stored in cd->parse_state)
	enum ParseState : uint32_t { MS_START, MS_HEADER, MS_EXT_HEADER, MS_DATA, MS_STUFFCOUNT, MS_CRC, MS_ACK, MS_EOF0, MS_EOF1, MS_DISCARD };

	// Parsing functions
	void data_state_go_next(ParseState state, uint32_t bits) noexcept;
	void data_state_go_discard() noexcept;
	void data_state_line_error() noexcept;
	void data_state_line_passive() noexcept;
	void data_state_go_data() noexcept;
	void data_state_go_stuff_count() noexcept;
	void data_state_go_crc() noexcept;
	void data_state_update_start(uint32_t data) noexcept;
	void data_state_update_header(uint32_t data) noexcept;
	void data_state_update_ext_header(uint32_t data) noexcept;
	void data_state_update_data(uint32_t data) noexcept;
	void data_state_update_stuffCount(uint32_t data) noexcept;
	void data_state_update_crc(uint32_t data) noexcept;
	void data_state_update_ack(uint32_t data) noexcept;
	void data_state_update_eof0(uint32_t data) noexcept;
	void data_state_update_eof1(uint32_t data) noexcept;
	void data_state_update_discard(uint32_t data) noexcept;
	void data_state_update(uint32_t data) noexcept;

	void process_rx(uint32_t rx_byte) noexcept;

	void PopulateTransmitBuffer() noexcept;
	void SendInterrupts() noexcept;

	// Setup
	VirtualCanRegisters *regs;
	pio_hw_t *pio_hw;

	// Bit unstuffing
	BitUnstuffer unstuf;
	uint32_t raw_bit_count;

	// Input data state
	uint32_t rxTimeStamp;
	uint32_t parse_id;
	uint32_t parse_state;
	uint32_t parse_crc;
    uint32_t parse_dlc;
    uint32_t parse_bytesReceived;
	uint32_t parse_bytesLeft;				// how many bytes of data are left to receive
	uint32_t *rxMessage;
	int rxFifoNumber;

	// Reporting
	// Report state flags (stored in report_state)
	enum ReportState : uint32_t { RS_IDLE = 0, RS_IS_TX = 1, RS_IN_MSG = 2, RS_AWAIT_EOF = 4, };

	ReportState report_state;
	uint32_t report_eof_key;

	// Transmit states (stored in tx_state)
	enum TxState : uint32_t { TS_IDLE = 0, TS_QUEUED = 1, TS_ACKING_RX = 2, TS_CONFIRM_TX = 3 };

	static constexpr size_t MaxTxMessageBits = ((12 + 1 + 18 + 9 + (64 * 8)) * 6)/5		// max frame length before the stuff count and CRC, including SOF
												+ 5										// fixed stuff bit and stuff count
												+ 27;									// fixed stuff bits and 21-bit CRC
	static constexpr size_t MaxTxMessageDwords = (MaxTxMessageBits + 3)/4;
	uint32_t txMessage[MaxTxMessageDwords];
	uint32_t txId;
	uint32_t txDlc;
	uint32_t txCrc;
	uint32_t txStuffedWords;
	TxState tx_state;

	uint32_t pendingIrqs;

	uint32_t rxDummyMessage[64/sizeof(uint32_t)];

	uint32_t numInterrupts;
};

#endif /* SRC_CANFD2040_H_ */
