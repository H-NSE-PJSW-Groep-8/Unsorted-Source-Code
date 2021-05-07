#include "XBEE_driver.h"
#include <string.h>

static void xbeeWriteCommand(xbee_driver* self, char cmd[2], char data[64])
{
	
	
}

static void xbeeReadCommand(xbee_driver* self)
{
	
	
}

void xbeeSetDefault(xbee_driver* self)
{
	self->channel = 0x0C;
	self->device_id = 0x3332;
	self->destination_addr_h = 0x00000000;
	self->destination_addr_l = 0x00000000;
	self->source_addr = 0x0000;
	self->retries = 0x00;
	self->random_delay_slots = 0x00;
	self->mac_mode = MM_DIGI_MODE_ACK;
	memset(self->node_identifier, 0, 20);
	self->node_discover_time = 0x19;
	self->node_discover_options = NO_DISABLE;
	self->coordinator_enable = CE_END_DEVICE;
	self->scan_duration = 0x04;
	self->end_device_association = 0x00;
	self->coordinator_association = 0x00;
	self->power_level = PL_N0DBM;
	self->cca_threshold = 0x2C;
	self->sleep_mode = SM_NO_SLEEP;
	self->sleep_option = SO_NORMAL_OPERATION;
	self->time_before_sleep = 0x1388;
	self->cyclic_sleep_period = 0x0000;
	self->disassociated_cyclic_sleep_period = 0x03E8;
	self->interface_data_rate = BD_9600;
	self->packetization_timeout = 0x03;
	self->api_enable = AP_DISABLED;
	self->parity = NB_NO_PARITY;
	self->pull_up = 0xFF;
	self->command_mode_timeout = 0x0064;
	self->guard_times = 0x03E8;
	self->command_sequence_char = '+';
}

void xbeeInit(xbee_driver* self)
{
	
	
}
