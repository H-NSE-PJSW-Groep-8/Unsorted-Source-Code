#ifndef _XBEE_DRIVER_H_
#define _XBEE_DRIVER_H_
#include <stdint.h>

// command registers
#define REG_WRITE									"WR"
#define REG_RESTORE_DEFAULTS						"RE"
#define REG_SOFTWARE_RESET							"FR"
#define REG_CHANNEL									"CH"
#define REG_PAN_ID									"ID"
#define REG_DESTINATION_ADDRESS_H					"DH"
#define REG_DESTINATION_ADDRESS_L					"DL"
#define REG_SOURCE_ADDRESS							"MY"
#define REG_SERIAL_NUMBER_H							"SH"
#define REG_SERIAL_NUMBER_L							"SL"
#define REG_RETRIES									"RR"
#define REG_RANDOM_DELAY_SLOTS						"RN"
#define REG_MAC_MODE								"MM"
#define REG_NODE_IDENTIFIER							"NI"
#define REG_NODE_DISCOVER							"ND"
#define REG_NODE_DISCOVER_TIME						"NT"
#define REG_NODE_DISCOVER_OPTIONS					"NO"
#define REG_DESTINATION_NODE						"DN"
#define REG_COORDINATOR_ENABLE						"CE"
#define REG_SCAN_CHANNELS							"SC"
#define REG_SCAN_DURATION							"SD"
#define REG_END_DEVICE_ASSOCIATION					"A1"
#define REG_COORDINATOR_ASSOCIATION					"A2"
#define REG_ASSOCIATION_INDICATOR					"AI"
#define REG_FORCE_DISASSOCIATION					"DA"
#define REG_FORCE_POLL								"FP"
#define REG_ACTIVE_SCAN								"AS"
#define REG_ENERGY_SCAN								"ED"
#define REG_AES_ENCRYPTION_ENABLE					"EE"
#define REG_AES_ENCRYPTION_KEY						"KY"
#define REG_POWER_LEVEL								"PL"
#define REG_CCA_THRESHOLD							"CA"
#define REG_SLEEP_MODE								"SM"
#define REG_SLEEP_OPTIONS							"SO"
#define REG_TIME_BEFORE_SLEEP						"ST"
#define REG_CYCLIC_SLEEP_PERIOD						"SP"
#define REG_DISASSOCIATED_CYCLIC_PERIODE			"DP"
#define REG_INTERFACE_DATA_RATE						"BD"
#define REG_PACKETIZATION_TIMEOUT					"RO"
#define REG_API_ENABLE								"AP"
#define REG_PARITY									"NB"
#define REG_PULL_UP_RESISTOR_ENABLE					"PR"
#define REG_DI8_CONFIGURATION						"D8"
#define REG_DIO7_CONFIGURATION						"D7"
#define REG_DIO6_CONFIGURATION						"D6"
#define REG_DIO5_CONFIGURATION						"D5"
#define REG_DIO4_CONFIGURATION						"D4"
#define REG_DIO3_CONFIGURATION						"D3"
#define REG_DIO2_CONFIGURATION						"D2"
#define REG_DIO1_CONFIGURATION						"D1"
#define REG_DIO0_CONFIGURATION						"D0"
#define REG_IO_OUTPUT_ENABLE						"IU"
#define REG_SAMPLES_BEFORE_TX						"IT"
#define REG_FORCE_SAMPLE							"IS"
#define REG_DIGITAL_OUTPUT_LEVEL					"IO"
#define REG_DIO_CHANGE_DETECT						"IC"
#define REG_SAMPLE_RATE								"IR"
#define REG_IO_INPUT_ADDRESS						"IA"
#define REG_D7_OUTPUT_TIMEOUT						"T7"
#define REG_D6_OUTPUT_TIMEOUT						"T6"
#define REG_D5_OUTPUT_TIMEOUT						"T5"
#define REG_D4_OUTPUT_TIMEOUT						"T4"
#define REG_D3_OUTPUT_TIMEOUT						"T3"
#define REG_D2_OUTPUT_TIMEOUT						"T2"
#define REG_D1_OUTPUT_TIMEOUT						"T1"
#define REG_D0_OUTPUT_TIMEOUT						"T0"
#define REG_PWM0_CONFIGURATION						"P0"
#define REG_PWM1_CONFIGURATION						"P1"
#define REG_PWM0_OUTPUT_LEVEL						"M0"
#define REG_PWM1_OUTPUT_LEVEL						"M1"
#define REG_PWM_OUTPUT_TIMEOUT						"PT"
#define REG_RSSI_PWM_TIMER							"RP"
#define REG_FIRMWARE_VERSION						"VR"
#define REG_FIRMWARE_VERSION_VERBOSE				"VL"
#define REG_HARDWARE_VERSION						"HV"
#define REG_RECIEVED_SIGNAL_STRENGTH				"DB"
#define REG_CCA_FAILURE								"EC"
#define REG_ACK_FAILURES							"EA"
#define REG_COMMAND_MODE_TIMEOUT					"CT"
#define REG_EXIT_COMMAND_MODE						"CN"
#define REG_APPLY_CHANGES							"AC"
#define REG_GUARD_TIME								"GT"
#define REG_COMMAND_SEQUENCE_CHARACTER				"CC"

typedef enum MM_e
{
	MM_DIGI_MODE_ACK	=0,
	MM_DIGI_MODE_NOACK	=3,
	MM_802_15_4_ACK		=2,
	MM_802_15_4_NOACK	=1
	
} MM;

typedef enum NO_e
{
	NO_DISABLE	=0,
	NO_ENABLE	=1
} NO;

typedef enum CE_e
{
	CE_END_DEVICE	=0,
	CE_COORDINATOR	=1
} CE;

typedef enum PL_e
{
	PL_N10DBM	=0,
	PL_N6DBM	=1,
	PL_N4DBM	=2,
	PL_N2DBM	=3,
	PL_N0DBM	=4
} PL;

typedef enum SM_e
{
	SM_NO_SLEEP							=0,
	SM_PIN_HYBERNATE					=1,
	SM_PIN_DOZE							=2,
	SM_CYCLIC_SLEEP_REMOTE				=4,
	SM_CYCLIC_SLEEP_REMOTE_PIN_WAKE		=5
} SM;

typedef enum SO_e
{
	SO_NORMAL_OPERATION				=0,
	SO_DISABLE_WAKE_UP_POLL			=1,
	SO_SUPPRESS_SAMPLE_ON_WAKEUP	=1,
} SO;

typedef enum BD_e
{
	BD_1200		=0,
	BD_2400		=1,
	BD_4800		=2,
	BD_9600		=3,
	BD_19200	=4,
	BD_38400	=5,
	BD_57600	=6,
	BD_115200	=7
} BD;

typedef enum AP_e
{
	AP_DISABLED					=0,
	AP_API_ENABLED				=1,
	AP_API_ENABLED_ESCAPED		=2
} AP;

typedef enum NB_e
{
	NB_NO_PARITY	=0,
	NB_EVEN			=1,
	NB_ODD			=2,
	NB_MARK			=3,
	NB_SPACE		=4
} NB;

// command struct
typedef struct xbee_driver_s
{
	uint8_t channel;
	uint16_t device_id;
	uint32_t destination_addr_h;
	uint32_t destination_addr_l;
	uint16_t source_addr;
	uint8_t retries;
	uint8_t random_delay_slots;
	MM mac_mode;
	char node_identifier[20];
	uint8_t node_discover_time;
	NO node_discover_options;
	CE coordinator_enable;
	uint8_t scan_duration;
	uint8_t end_device_association;
	uint8_t coordinator_association;
	PL power_level;
	uint8_t cca_threshold;
	SM sleep_mode;
	SO sleep_option;
	uint16_t time_before_sleep;
	uint16_t cyclic_sleep_period;
	uint16_t disassociated_cyclic_sleep_period;
	BD interface_data_rate;
	uint8_t packetization_timeout;
	AP api_enable;
	NB parity;
	uint8_t pull_up;
	uint16_t command_mode_timeout;
	uint16_t guard_times;
	char command_sequence_char;
} xbee_driver;

void xbeeSetDefault(xbee_driver* self);
void xbeeInit(xbee_driver* self);



#endif // _XBEE_DRIVER_H_
