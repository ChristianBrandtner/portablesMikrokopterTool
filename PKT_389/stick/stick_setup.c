/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/****************************************************************/
/* Setup für die Sticks                                         */
/* 2013 Cebra                                                   */
/****************************************************************/

//############################################################################
//# HISTORY  stick_setup.c
//#
//# 28.05.2014 OG
//# - chg: Setup's auf das neue Edit_generic() umgestellt
//#
//# 11.05.2014 OG
//# - chg: einge Setup-Menues umgestellt auf MenuCtrl_SetTitleFromParentItem()
//#        -> die Menues 'erben' damit ihren Titel vom aufrufenden Menuepunkt
//#
//# 30.03.2014 OG
//# - chg: ein paar englische Texte auf DE gemappt weil der Unterschied unerheblich war
//# - chg: Sprache Hollaendisch vollstaendig entfernt
//# - chg: MenuCtrl_PushML_P() umgestellt auf MenuCtrl_PushML2_P()
//#
//# 31.05.2013 OG
//# - chg: einige redundante Menuetitel auf define umgestellt
//#
//# 24.05.2013 OG
//# - chg: Aufrufe von MenuCtrl_Push() umgestellt auf MenuCtrl_PushML_P()
//#
//# 22.05.2013 OG
//# - umgestellt auf menuctrl
//# - del: include utils/menuold.h
//#
//# 17.05.2013 OG
//# - chg: umgestellt auf utils/menuold.h
//#
//# 16.04.2013 Cebra
//# - chg: PROGMEM angepasst auf neue avr-libc und GCC, prog_char depreciated
//#
//# 04.04.2013 Cebra
//# - chg: Texttuning
//#
//# 03.04.2013 Cebra
//# - chg: Holländische Menütexte
//#
//# 27.03.2013 Cebra
//# - chg: Fehler bei der Menüsteuerung behoben
//############################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../cpu.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../main.h"
#include "../timer/timer.h"
#include "stick_setup.h"
#include "../lcd/lcd.h"
#include "../timer/timer.h"
#include "../messages.h"
#include "../mk-data-structs.h"
#include "../eeprom/eeprom.h"
#include "../setup/setup.h"
#include "../utils/menuctrl.h"


//############################################################################
#ifdef USE_JOYSTICK
//############################################################################

//-----------------------------
// Setup_Joysticks()
//-----------------------------
#define JSTICKS_OVERVIEW    1
#define JSTICKS_STICK1      2
#define JSTICKS_STICK2      3
#define JSTICKS_STICK3      4
#define JSTICKS_STICK4      5
#define JSTICKS_STICK5      6
#define JSTICKS_LIPO        7

static const char JSTICKS_OVERVIEW_de[]     PROGMEM = "Übersicht";
static const char JSTICKS_OVERVIEW_en[]     PROGMEM = "summary";

static const char JSTICKS_STICK1_de[]       PROGMEM = "Stick 1";
#define           JSTICKS_STICK1_en         JSTICKS_STICK1_de

static const char JSTICKS_STICK2_de[]       PROGMEM = "Stick 2";
#define           JSTICKS_STICK2_en         JSTICKS_STICK2_de

static const char JSTICKS_STICK3_de[]       PROGMEM = "Stick 3";
#define           JSTICKS_STICK3_en         JSTICKS_STICK3_de

static const char JSTICKS_STICK4_de[]       PROGMEM = "Stick 4";
#define           JSTICKS_STICK4_en         JSTICKS_STICK4_de

static const char JSTICKS_STICK5_de[]       PROGMEM = "Stick 5";
#define           JSTICKS_STICK5_en         JSTICKS_STICK5_de

static const char JSTICKS_LIPO_de[]         PROGMEM = "PKT Lipomessung";
static const char JSTICKS_LIPO_en[]         PROGMEM = "PKT Lipo measure.";


//############################################################################

//-----------------------------
// Setup_Stick()
//-----------------------------
#define MSTICK_MIN      1
#define MSTICK_MAX      2
#define MSTICK_TYPE     3
#define MSTICK_DIR      4
#define MSTICK_NEUT     5

static const char MSTICK_MIN_de[]           PROGMEM = "Minimal Wert";
static const char MSTICK_MIN_en[]           PROGMEM = "minimal value";

static const char MSTICK_MAX_de[]           PROGMEM = "Maximal Wert";
static const char MSTICK_MAX_en[]           PROGMEM = "maximal value";

static const char MSTICK_TYPE_de[]          PROGMEM = "Type";
static const char MSTICK_TYPE_en[]          PROGMEM = "type";

static const char MSTICK_DIR_de[]           PROGMEM = "Richtung";
static const char MSTICK_DIR_en[]           PROGMEM = "direction";

static const char MSTICK_NEUT_de[]          PROGMEM = "Neutralisiered";
static const char MSTICK_NEUT_en[]          PROGMEM = "neutralizing";

//############################################################################


//--------------------------------------------------------------
//--------------------------------------------------------------
void Joysticks_Uebersicht(void)
{
    lcd_cls ();
    lcd_printpns_at(0, 0, PSTR("   Joystick Setup    "), 2);
    lcd_printpns_at(0, 1, PSTR("S Min Max Typ  Dir  N"), 0);

    for (uint8_t i = 0; i < 5; i++) {

        write_ndigit_number_u (0, 2+i,i+1, 1, 0,0);
        write_ndigit_number_u (2, 2+i,Config.stick_min[i], 3, 0,0);
        write_ndigit_number_u (6, 2+i,Config.stick_max[i], 3, 0,0);
        if (Config.stick_typ[i] == 0) lcd_printpns_at(10, 2+i, PSTR("Poti"), 0); else lcd_printpns_at(10, 2+i, PSTR("Tast"), 0);
          //if (Config.stick_typ[i] == 0) // MartinR: geändert
            //{
            if (Config.stick_dir[i] == 0) lcd_printpns_at(15, 2+i, PSTR("Norm"), 0); else lcd_printpns_at(15, 2+i, PSTR("Rev"), 0);
            if (Config.stick_neutral[i] == 0) lcd_printpns_at(20, 2+i, PSTR("N"), 0); else lcd_printpns_at(20, 2+i, PSTR("Y"), 0);
            //}
        if (i == 1) if (Config.Lipomessung == true) lcd_printpns_at(3, 2+i, PSTR("PKT Lipomessung   "), 0);

      }


    lcd_printp_at (18, 7, PSTR("OK"), 0);

    do{}
      while (!(get_key_press (1 << KEY_ENTER)));
}



//--------------------------------------------------------------
// Setup_Stick()
//--------------------------------------------------------------
void Setup_Stick( uint8_t stick )
{
    uint8_t itemid;

    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitleFromParentItem();      // "Stick n"
  //MenuCtrl_SetTitle_P( PSTR("Joystick") );

    //---------------
    // Menuitems
    //---------------
    MenuCtrl_PushML2_P( MSTICK_MIN  , MENU_ITEM, NOFUNC , MSTICK_MIN_de  , MSTICK_MIN_en  );
    MenuCtrl_PushML2_P( MSTICK_MAX  , MENU_ITEM, NOFUNC , MSTICK_MAX_de  , MSTICK_MAX_en  );
    MenuCtrl_PushML2_P( MSTICK_TYPE , MENU_ITEM, NOFUNC , MSTICK_TYPE_de , MSTICK_TYPE_en );
    MenuCtrl_PushML2_P( MSTICK_DIR  , MENU_ITEM, NOFUNC , MSTICK_DIR_de  , MSTICK_DIR_en  );
    MenuCtrl_PushML2_P( MSTICK_NEUT , MENU_ITEM, NOFUNC , MSTICK_NEUT_de , MSTICK_NEUT_en );

    //---------------
    // Control
    //---------------
    while( true )
    {
        MenuCtrl_Control( MENUCTRL_EVENT );

        if( MenuCtrl_GetKey() == KEY_ESC )  break;      // ENDE

        itemid = MenuCtrl_GetItemId();                  // welcher Menu-Punkt (ID)

        if( itemid == MSTICK_MIN  ) { Config.stick_min[stick]    = Edit_generic( Config.stick_min[stick]     ,0,300,Show_uint3,1  ,NULL,NULL);  }
        if( itemid == MSTICK_MAX  ) { Config.stick_max[stick]    = Edit_generic( Config.stick_max[stick]     ,0,300,Show_uint3,1  ,NULL,NULL);  }
        if( itemid == MSTICK_TYPE ) { Config.stick_typ[stick]    = Edit_generic( Config.stick_typ[stick]     ,0,  1,Sticktype,1   ,NULL,NULL);  }
        if( itemid == MSTICK_DIR  ) { Config.stick_dir[stick]    = Edit_generic( Config.stick_dir[stick]     ,0,  1,NormRev,1     ,NULL,NULL);  }
        if( itemid == MSTICK_NEUT ) { Config.stick_neutral[stick]= Edit_generic( Config.stick_neutral[stick] ,0,  1,YesNo,1       ,NULL,NULL);  }

    }

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}



//--------------------------------------------------------------
// Setup_Joysticks()
//--------------------------------------------------------------
void Setup_Joysticks( void )
{
    uint8_t itemid;

    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitleFromParentItem();      // "Joystick"
    //MenuCtrl_SetTitle_P( PSTR("Joystick Setup") );

    //---------------
    // Menuitems
    //---------------
    MenuCtrl_PushML2_P( JSTICKS_OVERVIEW  , MENU_ITEM, NOFUNC , JSTICKS_OVERVIEW_de , JSTICKS_OVERVIEW_en );
    MenuCtrl_PushML2_P( JSTICKS_STICK1    , MENU_SUB , NOFUNC , JSTICKS_STICK1_de   , JSTICKS_STICK1_en   );
    MenuCtrl_PushML2_P( JSTICKS_STICK2    , MENU_SUB , NOFUNC , JSTICKS_STICK2_de   , JSTICKS_STICK2_en   );
    MenuCtrl_PushML2_P( JSTICKS_STICK3    , MENU_SUB , NOFUNC , JSTICKS_STICK3_de   , JSTICKS_STICK3_en   );
    MenuCtrl_PushML2_P( JSTICKS_STICK4    , MENU_SUB , NOFUNC , JSTICKS_STICK4_de   , JSTICKS_STICK4_en   );
    MenuCtrl_PushML2_P( JSTICKS_STICK5    , MENU_SUB , NOFUNC , JSTICKS_STICK5_de   , JSTICKS_STICK5_en   );
    MenuCtrl_PushML2_P( JSTICKS_LIPO      , MENU_ITEM, NOFUNC , JSTICKS_LIPO_de     , JSTICKS_LIPO_en     );


    //---------------
    // Control
    //---------------
    while( true )
    {
        MenuCtrl_Control( MENUCTRL_EVENT );

        if( MenuCtrl_GetKey() == KEY_ESC )  break;      // ENDE

        itemid = MenuCtrl_GetItemId();                  // welcher Menu-Punkt (ID)

        if( itemid == JSTICKS_OVERVIEW  ) { Joysticks_Uebersicht(); }
        if( itemid == JSTICKS_STICK1    ) { Setup_Stick(0);          }
        if( itemid == JSTICKS_STICK2    ) { if (Config.Lipomessung == true)
                                            {
                                                lcd_cls ();
                                                lcd_printp_at(0, 3, PSTR("LiPo measure!"), MNORMAL);
                                                _delay_ms(1000);
                                            }
                                            else  Setup_Stick(1);           //Stick 2
                                          }
        if( itemid == JSTICKS_STICK3    ) { Setup_Stick(2);  }
        if( itemid == JSTICKS_STICK4    ) { Setup_Stick(3);  }
        if( itemid == JSTICKS_STICK5    ) { Setup_Stick(4);  }
        if( itemid == JSTICKS_LIPO      ) { Config.Lipomessung = Edit_generic( Config.Lipomessung ,0,1,YesNo,1  ,NULL,NULL);  }

    }

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}


//############################################################################
#endif // USE_JOYSTICK
//############################################################################
