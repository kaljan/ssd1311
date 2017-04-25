#ifndef SSD1311_H
#define SSD1311_H

// 1. Fundamental Command Set
#define SSD1311_CLEAR			0x01	// Clear Display
#define SSD1311_RET_HOME		0x02	// Return Home

#define SSD1311_MODE			0x04	// Entry Mode Set
#define SSD1311_MODE_S			0x01	//
#define SSD1311_MODE_ID			0x02	//
#define SSD1311_MODE_BDS		0x01	//
#define SSD1311_MODE_BDC		0x02	//

#define SSD1311_CTRL			0x08	// Display ON/OFFControl
#define SSD1311_CTRL_B			0x01	//
#define SSD1311_CTRL_C			0x02	//
#define SSD1311_CTRL_D			0x04	//

#define SSD1311_EXFNSET			0x08	// Extended Function Set
#define SSD1311_EXFNSET_NW		0x01	//
#define SSD1311_EXFNSET_BW		0x02	//
#define SSD1311_EXFNSET_FW		0x04	//

#define SSD1311_SHIFT			0x10	// Cursor or Display Shift
#define SSD1311_SHIFT_RL		0x08	// 
#define SSD1311_SHIFT_SC		0x04	// 

#define SSD1311_DDS				0x10	// Double Height (4-line)/Display-dot shift
#define SSD1311_DDS_DH			0x01	// 
#define SSD1311_DDS_UD1			0x04	// 
#define SSD1311_DDS_UD2			0x08	// 

#define SSD1311_SHIFT_EN		0x10	// Shift Enable
#define SSD1311_SHIFT_EN_DS1	0x01	//
#define SSD1311_SHIFT_EN_DS2	0x02	//
#define SSD1311_SHIFT_EN_DS3	0x04	//
#define SSD1311_SHIFT_EN_DS4	0x08	//

#define SSD1311_SCR_EN			0x10	// Scroll Enable
#define SSD1311_SCR_EN_HS1		0x01	//
#define SSD1311_SCR_EN_HS2		0x02	//
#define SSD1311_SCR_EN_HS3		0x04	//
#define SSD1311_SCR_EN_HS4		0x08	//

#define SSD1311_FNSET			0x20	// Function Set
#define SSD1311_FNSET_IS		0x01	//
#define SSD1311_FNSET_REV		0x01	//
#define SSD1311_FNSET_RE		0x02	//
#define SSD1311_FNSET_DH		0x04	//
#define SSD1311_FNSET_BE		0x04	//
#define SSD1311_FNSET_N			0x08	//

#define SSD1311_CGRAM_ADDR		0x40	// Set CGRAM address
#define SSD1311_DDRAM_ADDR		0x80	// Set DDRAM Address
#define SSD1311_SSRQ			0x80	// Set Scroll Quantity
#define SSD1311_BUSY			0x80	// Read Busy Flag and Address/Part ID

// 2. Extended Command Set
#define SSD1311_EX_FSELA		0x71	// Function Selection A
#define SSD1311_EX_FSELA_5V_DIS	0x00
#define SSD1311_EX_FSELA_5V_EN	0x5C
#define SSD1311_EX_FSELB		0x72	// Function Selection B
#define SSD1311_EX_CHZ			0x78	// OLED Characterization
#define SSD1311_EX_CHZ_SD		0x01	// 
#define SSD1311_EX_SETCC		0x81	// Set Contrast Control
#define SSD1311_EX_CLKCTRL		0xD5	// Set Display Clock Divide Ratio/Oscillator Frequency
#define SSD1311_EX_SET_PL		0xD9	// Set Phase Length
#define SSD1311_EX_SET_PINS		0xDA	// Set SEG Pins Hardware Configuration
#define SSD1311_EX_SET_VCOMH	0xDB	// Set VCOMH Deselect Level
#define SSD1311_EX_FSELC		0xDC	// Function Selection C
#define SSD1311_EX_SET_FADE		0x23	// Set Fade Out and Fade in / out

int ssd1311_init(void);
int ssd1311_on(void);
int ssd1311_set_string(const char *str, int start);

#endif // SSD1311_H
