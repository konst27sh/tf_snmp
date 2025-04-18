/*
 * Note: this file originally auto-generated by mib2c
 * using mib2c.column_enums.conf
 */
#ifndef SWITCH_ENUMS_H
#define SWITCH_ENUMS_H

/* enums for column comfStState */
           #define COMFSTSTATE_ENABLED		1
           #define COMFSTSTATE_DISABLED		2

/* enums for column autoRstMode */
           #define AUTORSTMODE_DISABLED		0
           #define AUTORSTMODE_LINK		1
           #define AUTORSTMODE_PING		2
           #define AUTORSTMODE_SPEED		3
           #define AUTORSTMODE_TIME		4

/* enums for column portPoeState */
           #define PORTPOESTATE_ENABLED		1
           #define PORTPOESTATE_DISABLED		2

/* enums for column inputType */
           #define INPUTTYPE_BUILD_IN		1
           #define INPUTTYPE_PLC		2

/* enums for column inputState */
           #define INPUTSTATE_SHORT		1
           #define INPUTSTATE_OPEN		2

/* enums for column inputAlarm */
           #define INPUTALARM_SHORT		1
           #define INPUTALARM_OPEN		2
           #define INPUTALARM_ANY		3

/* enums for column portPoeStatusState */
           #define PORTPOESTATUSSTATE_UP		1
           #define PORTPOESTATUSSTATE_DOWN		2

/* enums for column arPortStatus */
           #define ARPORTSTATUS_NORMAL		1
           #define ARPORTSTATUS_NOLINK		2
           #define ARPORTSTATUS_NOPING		3
           #define ARPORTSTATUS_LOWSPEED		4

/* enums for column csPortStatus */
           #define CSPORTSTATUS_NORMAL		1
           #define CSPORTSTATUS_PROCESSING		2


/* enums for scalar state */
#define STATE_OPEN		1
#define STATE_SHORT		2

/* enums for scalar upsModeAvalible */
#define UPSMODEAVALIBLE_TRUE		1
#define UPSMODEAVALIBLE_FALSE		2

/* enums for scalar upsPwrSource */
#define UPSPWRSOURCE_BATTERY		1
#define UPSPWRSOURCE_AC		2

/* enums for scalar sensorConnected */
#define SENSORCONNECTED_CONNECTED		1
#define SENSORCONNECTED_DISCONNECTED		2

#endif /* SWITCH_ENUMS_H */
