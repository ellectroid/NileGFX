#ifndef LIB_INC_GFX_GFX_CONTROL_H_
#define LIB_INC_GFX_GFX_CONTROL_H_

#include "nile_gfx/gfx_draw_op_queue.h"

enum {

	// ========== CR1 ==========
	// Control register 1

	//Draw queue number, a number [0..MAX] at an offset
	NILE_GFX_CR1_DRQN_MAX = 4,
	NILE_GFX_CR1_DRQN_POS = 0,
	NILE_GFX_CR1_DRQN_MASK = 0x07 << NILE_GFX_CR1_DRQN_POS,
	NILE_GFX_CR1_DRQN = NILE_GFX_CR1_DRQN_MAX << NILE_GFX_CR1_DRQN_POS,

	//Shadow register reload request
	NILE_GFX_CR1_SRELRQ_POS = 3,
	NILE_GFX_CR1_SRELRQ_MASK = 0x01 << NILE_GFX_CR1_SRELRQ_POS,
	NILE_GFX_CR1_SRELRQ = 0x01 << NILE_GFX_CR1_SRELRQ_POS,

	//Queue enable flags, one per queue
	NILE_GFX_CR1_QEN_POS = 4,
	NILE_GFX_CR1_QEN_MASK = 0x0F << NILE_GFX_CR1_QEN_POS,
	NILE_GFX_CR1_QEN0 = 0x01 << NILE_GFX_CR1_QEN_POS,
	NILE_GFX_CR1_QEN1 = 0x02 << NILE_GFX_CR1_QEN_POS,
	NILE_GFX_CR1_QEN2 = 0x04 << NILE_GFX_CR1_QEN_POS,
	NILE_GFX_CR1_QEN3 = 0x08 << NILE_GFX_CR1_QEN_POS,

	//Queue destructive read mode enable, one per queue
	NILE_GFX_CR1_QDRMEN_POS = 8,
	NILE_GFX_CR1_QDRMEN_MASK = 0x0F << NILE_GFX_CR1_QDRMEN_POS,
	NILE_GFX_CR1_QDRMEN0 = 0x01 << NILE_GFX_CR1_QDRMEN_POS,
	NILE_GFX_CR1_QDRMEN1 = 0x02 << NILE_GFX_CR1_QDRMEN_POS,
	NILE_GFX_CR1_QDRMEN2 = 0x04 << NILE_GFX_CR1_QDRMEN_POS,
	NILE_GFX_CR1_QDRMEN3 = 0x08 << NILE_GFX_CR1_QDRMEN_POS,

	//Queue active flag (being traversed)
	NILE_GFX_CR1_QACTIVE_POS = 12,
	NILE_GFX_CR1_QACTIVE_MASK = 0x0F << NILE_GFX_CR1_QACTIVE_POS,
	NILE_GFX_CR1_QACTIVE0 = 0x01 << NILE_GFX_CR1_QACTIVE_POS,
	NILE_GFX_CR1_QACTIVE1 = 0x02 << NILE_GFX_CR1_QACTIVE_POS,
	NILE_GFX_CR1_QACTIVE2 = 0x04 << NILE_GFX_CR1_QACTIVE_POS,
	NILE_GFX_CR1_QACTIVE3 = 0x08 << NILE_GFX_CR1_QACTIVE_POS,

	//Queue target cache enable
	NILE_GFX_CR1_QTCEN_POS = 16,
	NILE_GFX_CR1_QTCEN_MASK = 0x0F << NILE_GFX_CR1_QTCEN_POS,
	NILE_GFX_CR1_QTCEN0 = 0x01 << NILE_GFX_CR1_QTCEN_POS,
	NILE_GFX_CR1_QTCEN1 = 0x02 << NILE_GFX_CR1_QTCEN_POS,
	NILE_GFX_CR1_QTCEN2 = 0x04 << NILE_GFX_CR1_QTCEN_POS,
	NILE_GFX_CR1_QTCEN3 = 0x08 << NILE_GFX_CR1_QTCEN_POS,

	//Framebuffer displayed buffer pointer index
	NILE_GFX_CR1_FBDISPLAY_POS = 20,
	NILE_GFX_CR1_FBDISPLAY_MASK = 0x03 << NILE_GFX_CR1_FBDISPLAY_POS,

	//Framebuffer draw buffer pointer index
	NILE_GFX_CR1_FBDRAW_POS = 22,
	NILE_GFX_CR1_FBDRAW_MASK = 0x03 << NILE_GFX_CR1_FBDRAW_POS,

	//Cache enable flag
	NILE_GFX_CR1_CEN_POS = 24,
	NILE_GFX_CR1_CEN_MASK = 0x01 << NILE_GFX_CR1_CEN_POS,
	NILE_GFX_CR1_CEN = 0x01 << NILE_GFX_CR1_CEN_POS,

	//Cache dirty flag
	NILE_GFX_CR1_CDIR_POS = 25,
	NILE_GFX_CR1_CDIR_MASK = 0x01 << NILE_GFX_CR1_CDIR_POS,
	NILE_GFX_CR1_CDIR = 0x01 << NILE_GFX_CR1_CDIR_POS,

	//Hardware ready
	NILE_GFX_CR1_HWRDY_POS = 26,
	NILE_GFX_CR1_HWRDY_MASK = 1 << NILE_GFX_CR1_HWRDY_POS,
	NILE_GFX_CR1_HWRDY = 1 << NILE_GFX_CR1_HWRDY_POS,

	//Hardware error
	NILE_GFX_CR1_HWERR_POS = 27,
	NILE_GFX_CR1_HWERR_MASK = 1 << NILE_GFX_CR1_HWERR_POS,
	NILE_GFX_CR1_HWERR = 1 << NILE_GFX_CR1_HWERR_POS,

	//Configuration error
	NILE_GFX_CR1_CERR_POS = 28,
	NILE_GFX_CR1_CERR_MASK = 1 << NILE_GFX_CR1_CERR_POS,
	NILE_GFX_CR1_CERR = 1 << NILE_GFX_CR1_CERR_POS,

	//Global render enable
	NILE_GFX_CR1_GRENEN_POS = 29,
	NILE_GFX_CR1_GRENEN_MASK = 1 << NILE_GFX_CR1_GRENEN_POS,
	NILE_GFX_CR1_GRENEN = 1 << NILE_GFX_CR1_GRENEN_POS,


	// ========== QAR1 ==========
	// QAR1: Queue active read current operation number for Q0 and Q1
	NILE_GFX_QAR1_Q0COP_POS = 0,
	NILE_GFX_QAR1_Q0COP_MASK = 0xFFFF << NILE_GFX_QAR1_Q0COP_POS,
	NILE_GFX_QAR1_Q1COP_POS = 16,
	NILE_GFX_QAR1_Q1COP_MASK = 0xFFFF << NILE_GFX_QAR1_Q0COP_POS,

	// ========== QAR2 ==========
	// QAR2: Queue active read current operation number for Q2 and Q3
	NILE_GFX_QAR2_Q2COP_POS = 0,
	NILE_GFX_QAR2_Q2COP_MASK = 0xFFFF << NILE_GFX_QAR2_Q2COP_POS,
	NILE_GFX_QAR2_Q3COP_POS = 16,
	NILE_GFX_QAR2_Q3COP_MASK = 0xFFFF << NILE_GFX_QAR2_Q3COP_POS,

	// ========== SCR ==========
	// SCR: Screen configuration register
	NILE_GFX_SCR_SWIDTH_POS = 0,
	NILE_GFX_SCR_SWIDTH_MASK = 0xFFFF << NILE_GFX_SCR_SWIDTH_POS,
	NILE_GFX_SCR_SHEIGHT_POS = 16,
	NILE_GFX_SCR_SHEIGHT_MASK = 0xFFFF << NILE_GFX_SCR_SHEIGHT_POS,

	// ========== HWINITR ==========
	// HWINITR: Hardware initializer register
	NILE_GFX_HWINITR_FADDR_POS = 0, //function address
	NILE_GFX_HWINITR_FADDR_MASK = 0xFFFFFFFF << NILE_GFX_HWINITR_FADDR_POS,
	NILE_GFX_HWINITR_FADDR_THUMB_BIT = 1 << 0,

	// ========== CDOPR ==========
	//CDOPR: Current draw operation register
	NILE_GFX_CDOPR_OPADDR_POS = 0,
	NILE_GFX_CDOPR_OPADDR_MASK = 0xFFFFFFFF << NILE_GFX_CDOPR_OPADDR_POS,

};

typedef struct {
	//nile attributes
	uint32_t module_size;
	uint32_t module_type;

	//hw config
	uint32_t SCR; //screen configuration register
	void *FB[2]; //displayed and drawn framebuffer
	void *FB_CACHE; //cache framebuffer
	void *FB_DRAW; //draw framebuffer
	void *HWINITR; //hardware init void func(void)

	//internal render state
	nile_gfx_draw_op *CDOPR; //current draw operation register
	union {
			struct {
				uint32_t QAR1; //queue active read 1
				uint32_t QAR2; //queue active read 2
			};
			uint16_t QAR[4];
	};

	//current screen config
	uint32_t CR1; //control register 1
	void* RSEAR; //reload screen export address
	nile_gfx_draw_op_queue DQ[NILE_GFX_CR1_DRQN >> NILE_GFX_CR1_DRQN_POS]; //draw queue


	//shadow screen config
	uint32_t SCR1;
	void* SRSEAR; //shadow reload screen export address
	nile_gfx_draw_op_queue SDQ[NILE_GFX_CR1_DRQN >> NILE_GFX_CR1_DRQN_POS];

	//blanking io variable
	uint32_t* BLSIG; //blanking signal
	uint32_t* ACTSIG; //active signal (opposite of blanking)


} nile_gfx;


#endif /* LIB_INC_GFX_GFX_CONTROL_H_ */
