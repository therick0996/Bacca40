#include QMK_KEYBOARD_H

// Flash bootloader: avrdude -c avrisp -P /dev/cu.usbmodem123451 -p atmega32u4 -U flash:w:bootloader_atmega32u4_1_0_0.hex:i
 
// Layers
#define _BASE         0
#define _LOWER        1
#define _RAISE        2
#define _FUNCTION     3
#define _LWL0         4
#define _LWL1         5
#define _FUNCTION2    6

#define _BASEW        7
#define _LOWERW       8
#define _RAISEW       9
#define _FUNCTIONW    10
#define _LWL0W        11
#define _LWL1W        12
#define _FUNCTION2W   13

// Macro keycodes
enum custom_keycodes {
  INS_ROW = SAFE_RANGE,
  DEL_ROW,
  INS_COL,
  DEL_COL,
  INS_ROWW,
  DEL_ROWW,
  INS_COLW,
  DEL_COLW
};

typedef struct {
  bool is_press_action;
  int state;
} xtap;


// Tap dance
enum {
  SINGLE_TAP =        1,
  SINGLE_HOLD =       2,
  DOUBLE_TAP =        3,
  DOUBLE_HOLD =       4,
  DOUBLE_SINGLE_TAP = 5, // Send two single taps
  TRIPLE_TAP =        6,
  TRIPLE_HOLD =       7
};

// Tap dance enums
enum {
  // Simple 
  PIPE = 0,
  // Complex
  FIRM,
  EMAIL,
  SUM,
  LBKTS,
  RBKTS,
  TILDE
};

// Tap dance dance states
// To activate SINGLE_HOLD, you will need to hold for 200ms first.
// This tap dance favors keys that are used frequently in typing like 'f'
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    //If count = 1, and it has been interrupted - it doesn't matter if it is pressed or not: Send SINGLE_TAP
    if (state->interrupted) {
      //     if (!state->pressed) return SINGLE_TAP;
      //need "permissive hold" here.
      //     else return SINsGLE_HOLD;
      //If the interrupting key is released before the tap-dance key, then it is a single HOLD
      //However, if the tap-dance key is released first, then it is a single TAP
      //But how to get access to the state of the interrupting key????
      return SINGLE_TAP;
    }
    else {
      if (!state->pressed) return SINGLE_TAP;
      else return SINGLE_HOLD;
    }
  }
  // If count = 2, and it has been interrupted - assume that user is trying to type the letter associated
  // with single tap.
  else if (state->count == 2) {
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else if ((state->count == 3) && ((state->interrupted) || (!state->pressed))) return TRIPLE_TAP;
  else if (state->count == 3) return TRIPLE_HOLD;
  else return 8; //magic number. At some point this method will expand to work for more presses
}

// This works well if you want this key to work as a "fast modifier". It favors being held over being tapped.
int hold_cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted) {
      if (!state->pressed) return SINGLE_TAP;
      else return SINGLE_HOLD;
    }
    else {
      if (!state->pressed) return SINGLE_TAP;
      else return SINGLE_HOLD;
    }
  }
  // If count = 2, and it has been interrupted - assume that user is trying to type the letter associated
  // with single tap.
  else if (state->count == 2) {
    if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else if (state->count == 3) {
    if (!state->pressed) return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8; // Magic number. At some point this method will expand to work for more presses
}

// For complex tap dances. Put it here so it can be used in any keymap
void firm_finished (qk_tap_dance_state_t *state, void *user_data);
void firm_reset (qk_tap_dance_state_t *state, void *user_data);

void email_finished (qk_tap_dance_state_t *state, void *user_data);
void email_reset (qk_tap_dance_state_t *state, void *user_data);

void sum_finished (qk_tap_dance_state_t *state, void *user_data);
void sum_reset (qk_tap_dance_state_t *state, void *user_data);

void lbkts_finished (qk_tap_dance_state_t *state, void *user_data);
void lbkts_reset (qk_tap_dance_state_t *state, void *user_data);

void rbkts_finished (qk_tap_dance_state_t *state, void *user_data);
void rbkts_reset (qk_tap_dance_state_t *state, void *user_data);

void tilde_finished (qk_tap_dance_state_t *state, void *user_data);
void tilde_reset (qk_tap_dance_state_t *state, void *user_data);

// Tap dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
  [PIPE]      = ACTION_TAP_DANCE_DOUBLE(KC_BSLS, KC_PIPE),
  [EMAIL]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL, email_finished, email_reset),
  [FIRM]      = ACTION_TAP_DANCE_FN_ADVANCED(NULL, firm_finished, firm_reset),
  [SUM]       = ACTION_TAP_DANCE_FN_ADVANCED(NULL, sum_finished, sum_reset),
  [LBKTS]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lbkts_finished, lbkts_reset),
  [RBKTS]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rbkts_finished, rbkts_reset),
  [TILDE]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tilde_finished, tilde_reset),
};

// Readability keycodes
#define _______		  KC_TRNS

// Layer codes
  // macOS
  #define LOWER         MO(_LOWER) // Momentary Lower macOS layer
  #define RAISE         MO(_RAISE) // Momenraty raise macOS layer
  #define FN            MO(_FUNCTION) // Momentary Function macOS layer
  #define FN2           MO(_FUNCTION2) // momentary Function2 macOS layer

  #define LWR(kc)       LT(_LOWER, KC_##kc) // Lower macOS layer when held, KC when tapped
  #define RSE(kc)       LT(_RAISE, KC_##kc) // Raise macOS layer when held, KC when tapped
  #define FUN(kc)       LT(_FUNCTION, KC_##kc) // Function lmacOS ayer when held, KC when tapped
  #define FUN2(kc)      LT(_FUNCTION2, KC_##kc) // Function2 macOS layer when held, KC when tapped
  #define LWL0(kc)      LT(_LWL0, KC_##kc) // LWL0 macOS layer when held, KC when tapped
  #define LWL1(kc)      LT(_LWL1, KC_##kc) // LWL1 macOS layer when held, KC when tapped

  // Windows
  #define LOWERW        MO(_LOWERW) // Momentary Lower Windows layer
  #define RAISEW        MO(_RAISEW) // Momentary Raise Windows layer
  #define FNW           MO(_FUNCTIONW) // Momentary Function Windows layer
  #define FN2W          MO(_FUNCTION2W) // Momentary Function2 layer

  #define LWRW(kc)      LT(_LOWERW, KC_##kc) // Lower Windows layer when held, KC when tapped
  #define RSEW(kc)      LT(_RAISEW, KC_##kc) //  Raise Windows layer when held, KC when tapped
  #define FUNW(kc)      LT(_FUNCTIONW, KC_##kc) //  Function Windows layer when held, KC when tapped
  #define FUN2W(kc)     LT(_FUNCTION2W, KC_##kc) //  Function2 Windows layer when held, KC when tapped
  #define LWL0W(kc)     LT(_LWL0W, KC_##kc) //  LWL0 Windows layer when held, KC when tapped
  #define LWL1W(kc)     LT(_LWL1W, KC_##kc) //  LWL1 Windows layer when held, KC when tapped

// Dual key codes
  #define CTL(kc)       LCTL_T(KC_##kc) // Control when held, KC when pressed
  #define GUI(kc)       LGUI_T(KC_##kc) // GUI when held, KC when pressed
  #define SFT(kc)       LSFT_T(KC_##kc) // Shift when held, KC when pressed
  #define ALT(kc)       LALT_T(KC_##kc) // Alt when held, KC when pressed

// Screenshots
  // macOS
  #define WHLSCR        LGUI(LSFT(KC_3))  // Capture whole screen macOS
  #define PRNTSCR       LGUI(LSFT(KC_4))  // Select screen macOS
  #define PRNTSCRC      LGUI(LSFT(LCTL(KC_4)))  // Select screen copy macOS

// Windows
  #define PRNTSCRW      LGUI(LSFT(KC_S)) // Screen snip Windows

// Chrome Tabs
  // macOS
  #define TAB_LEFT      LGUI(LALT(KC_LEFT))  // Tab left macOS
  #define TAB_RIGHT     LGUI(LALT(KC_RIGHT))  // Tab right macOS
  
  // Windows
  #define TAB_LEFTW     LCTL(KC_PGDN) // Tab left Windows
  #define TAB_RIGHTW    LCTL(KC_PGUP) // Tab right Windows

// Google Sheets shortcuts
  // macOS
  #define SHT_LEFT      LALT(KC_UP)  // Go to next sheet in Sheets macOS
  #define SHT_RIGHT     LALT(KC_DOWN)  // Go to previous sheet in Sheets macOS
  #define HIDE_ROW      LGUI(LALT(KC_9))  // Hide row macOS
  #define UNHIDE_ROW    LGUI(LSFT(KC_9))  // Unhide row macOS
  #define HIDE_COL      LGUI(LALT(KC_0))  // Hide column macOS
  #define UNHIDE_COL    LGUI(LSFT(KC_0))  // Unhide column macOS

  // Windows
  #define SHT_LEFTW     LALT(KC_UP)  // Go to next sheet in Sheets Windows
  #define SHT_RIGHTW    LALT(KC_DOWN)  // Go to previous sheet in Sheets Windows
  #define HIDE_ROWW     LCTL(LALT(KC_9))  // Hide row Windows
  #define UNHIDE_ROWW   LCTL(LSFT(KC_9))  // Unhide row Windows
  #define HIDE_COLW     LCTL(LALT(KC_0))  // Hide column Windows
  #define UNHIDE_COLW   LCTL(LSFT(KC_0))  // Unhide column Windows

// Other
  #define CTL_HOME      LCTL(KC_HOME) // Ctrl + Home
  #define GUI_HOME      LGUI(KC_HOME) // CMD + Home
  #define SA_BS 		    MT(MOD_LSFT | MOD_LALT, KC_BSPC)

// Begin keymaps
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// macOS
/* Base macOS
  .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
  |           |  |    Esc    |     Q     |     W     |     E     |     R     |     T     |     Y     |     U     |     I     |     O     |     P     |    Del    |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |  Fn Tab   |     A     |     S     |     D     |     F     |     G     |     H     |     J     |     K     |     L     |     ;     |     '     |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |   Shift   |     Z     |     X     |     C     |     V     |     B     |     N     |     M     |     ,     |     .     |     /     |   Enter   |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |    GUI    |   Shift   |   Ctrl    |   Option  |  Lwr BS   |  FN2 BS   | FN Space  |  Rse Spc  |   Left    |   Down    |     Up    |   Right   |
  '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
*/ 
  [_BASE] = LAYOUT_ortho_1x4x12(
    _______,      KC_ESC,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_DEL,
    _______,      FUN(TAB),   ALT(A),     KC_S,       KC_D,       GUI(F),     KC_G,       KC_H,       GUI(J),     KC_K,       KC_L,       KC_SCLN,    SFT(QUOT),
    _______,      KC_LSFT,    GUI(Z),     SFT(X),     KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    SFT(ENT),
    _______,      KC_LGUI,    KC_LSFT,    KC_LCTRL,   KC_LALT,    LWR(BSPC),  FUN2(BSPC), FUN(SPC),   RSE(SPC),   KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT
  ),

/* Lower macOS
  .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
  |           |  |           |   ( [ {   |   ) ] }   |     -     |     =     |    ***    |    ***    |    BS     |     7     |     8     |     9     |     -     |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |   LWL0    |   Home    |   Pg Dn   |   Pg Up   |    End    |           |    F4     |    F2     |     4     |     5     |     6     |     +     |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |   Left    |    Down   |    Up     |   Right   |    ***    |           | Button 1  |     1     |     2     |     3     |   Enter   |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |   Lower   |           |           |   LWL0    |     0     |     *     |     .     |     /     |
  '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
*/
  [_LOWER] = LAYOUT_ortho_1x4x12(
    _______,      _______,    TD(LBKTS),  TD(RBKTS),  KC_MINS,    TD(SUM),    KC_NO,      KC_NO,      KC_BSPC,     KC_P7,     KC_P8,      KC_P9,      KC_PMNS,
    _______,      LWL0(TAB),  GUI(HOME),  KC_PGDN,    KC_PGUP,    LWL1(END),  _______,    KC_F4,      KC_F2,       KC_P4,     KC_P5,      KC_P6,      KC_PPLS,
    _______,      _______,    KC_LEFT,    KC_DOWN,    KC_UP,      KC_RGHT,    KC_NO,      _______,    KC_BTN1,     KC_P1,     KC_P2,      KC_P3,      KC_PENT,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    LWL0(SPC),   KC_P0,     KC_PAST,    KC_PDOT,    KC_PSLS
  ),

    /* LWL0 macOS
      .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
      |           |  |           |           |           |           |           |           |           |           |    BS     |     /     |     *     |    Del    |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  |   LWL0    |   GUI     |   Shift   |    Alt    |    Del    |           |           |           |   Left    |   Down    |     Up    |   Right   |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  |           |           |           |           |           |           |           |           |   Home    |   Pg Dn   |   Pg Up   |    End    |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  |           |           |           |           |   Lower   |           |  TG(Lwr)  |   LWL0    |           |           |           |           |
      '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
    */

      [_LWL0] = LAYOUT_ortho_1x4x12(
        _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_BSPC,    KC_PSLS,    KC_PAST,    KC_DEL,
        _______,      _______,    KC_LGUI,    KC_LSFT,    KC_LALT,    KC_DEL,     _______,    _______,    _______,    KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT,
        _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_HOME,    KC_PGDN,    KC_PGUP,    KC_END,
        _______,      _______,    _______,    _______,    _______,    _______,    _______,    TG(1),	    _______,    _______,    _______,    _______,    _______
      ),

    /* LWL1 macOS
      .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
      |           |  |   RESET   |           |           |           |           |           |           |           |           |           |           |           |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  |           | GUI+Home  | Prt Sel C | Prnt Sel  |   LWL1    |           |           |           |     $     |     ,     |     %     |           |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  | Caps Lock | Prnt Scr  |           |           |           |           |           |           |     !     |           |           |           |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  | Windows   |           |           |           |           |           |           |           |           |           |  NumLock  |           |
      '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
    */
      [_LWL1] = LAYOUT_ortho_1x4x12(
        _______,      RESET,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
        _______,      _______,    GUI_HOME,   PRNTSCRC,   PRNTSCR,    _______,    _______,    _______,    _______,    KC_DLR,     KC_COMM,    KC_PERC,    _______,
        _______,      KC_CAPS,    WHLSCR,     _______,    _______,    _______,    _______,    _______,    _______,    KC_EXLM,    _______,    _______,    _______,
        _______,      TG(7),      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_NLCK,    _______
      ),

/* Raise macOS
  .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
  |           |  |    ` ~    |     1     |     2     |     3     |     4     |     5     |     6     |     7     |     8     |     9     |     0     |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |     !     |     @     |     #     |     $     |     %     |     ^     |     &     |     *     |     (     |     )     |     _     |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |           |           |           |           |     [     |     ]     |    \ |    |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |           |           |  TG(Lwr)  |   Raise   |   Play    |   Vol-    |   Vol+    |   Mute    |
  '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
*/
  [_RAISE] = LAYOUT_ortho_1x4x12(
    _______,      TD(TILDE),  KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       _______,
    _______,      _______,    KC_EXLM,    TD(EMAIL),  KC_HASH,    KC_DLR,     KC_PERC,    KC_CIRC,    KC_AMPR,    KC_ASTR,    KC_LPRN,    KC_RPRN,    KC_UNDS,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_LBRC,    KC_RBRC,    TD(PIPE),   _______,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    TG(1),      _______,    KC_MPLY,    KC_VOLD,    KC_VOLU,    KC_MUTE
  ),

/* FN macOS
  .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
  |           |  |           |    F1     |    F2     |    F3     |    F4     |    F5     |    F6     |    F7     |    F8     |    F9     |   F10     |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |    Fn     |    GUI    |   Shift   |  Option   |    Del    |           |           |   Left    |    Down   |    Up     |   Right   |   Enter   |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           | Firmware  |           |           |   Home    |   Pg Dn   |   Pg Up   |    End    |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |   Enter   |           |           |           |           |           |   F11     |    F12    |
  '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
*/
  [_FUNCTION] = LAYOUT_ortho_1x4x12(
    _______,      _______,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     _______,
    _______,      _______,    KC_LGUI,    KC_LSFT,    KC_LALT,    KC_DEL,     _______,    _______,    CTL(LEFT),  KC_DOWN,    KC_UP,      CTL(RIGHT), KC_ENT,
    _______,      _______,    _______,    _______,    _______,    TD(FIRM),   _______,   _______,    KC_HOME,    KC_PGDN,    KC_PGUP,    KC_END,     _______,
    _______,      _______,    _______,    _______,    _______,    KC_ENT,     _______,    _______,    _______,    _______,    _______,    KC_F11,     KC_F12 
  ),

/* FN2 macOS
  .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
  |           |  |     ~     |  Tab Left | Tab Right | Sht Left  | Sht Right |           |           |           |           |           |           |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |  Del Col  |  Ins Col  |  Del Row  |  Ins Row  |           |           |           |           |           |           |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |  Hide Col | Unhide Col|  Hide Row | Unhide Row|           |           |           |           |           |           |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |           |           |           |           |           |           |           |           |
  '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
*/ 
  [_FUNCTION2] = LAYOUT_ortho_1x4x12(
    _______,      KC_GRAVE,   TAB_LEFT,   TAB_RIGHT,  SHT_LEFT,   SHT_RIGHT,  _______,    _______,    _______,    _______,    _______,    _______,    _______,
    _______,      _______,    DEL_COL,    INS_COL,    DEL_ROW,    INS_ROW,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
    _______,      _______,    HIDE_COL,   UNHIDE_COL, HIDE_ROW,   UNHIDE_ROW, _______,   _______,    _______,    _______,    _______,    _______,    _______,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______ 
  ) ,

// Windows
/* Base Windows
  .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
  |           |  |    Esc    |     Q     |     W     |     E     |     R     |     T     |     Y     |     U     |     I     |     O     |     P     |    Del    |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |  Fn Tab   |     A     |     S     |     D     |     F     |     G     |     H     |     J     |     K     |     L     |     ;     |     '     |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |   Shift   |     Z     |     X     |     C     |     V     |     B     |     N     |     M     |     ,     |     .     |     /     |   Enter   |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |    Ctrl   |   Shift   |    GUI    |    Alt    |  Lwr BS   |  FUN2 BS  |   Space   |  Rse Spc  |   Left    |   Down    |     Up    |   Right   |
  '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
*/
  [_BASEW] = LAYOUT_ortho_1x4x12(
    KC_MPLY,      KC_ESC,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_DEL,
    _______,      FUNW(TAB),  KC_A,       KC_S,       KC_D,       CTL(F),     KC_G,       KC_H,       CTL(J),     KC_K,       KC_L,       KC_SCLN,    SFT(QUOT),
    _______,      KC_LSFT,    CTL(Z),     SFT(X),     KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    SFT(ENT),
    _______,      KC_LCTL,    KC_LSFT,    KC_LGUI,    KC_LALT,    LWRW(BSPC), FUN2W(BSPC),FUNW(SPC),  RSEW(SPC),  KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT
  ),

/* Lower Windows
  .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
  |           |  |           |   ( [ {   |   ) ] }   |     -     |     =     |    ***    |    ***    |    BS     |     7     |     8     |     9     |     -     |
  |-----------|  |-----------|-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |   LWL0    |   Home    |   Pg Dn   |   Pg Up   | End LWL1  |    BS     |    F4     |    F2     |     4     |     5     |     6     |     +     |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |   Left    |    Down   |    Up     |   Right   |    ***    |           |   Calc    |     1     |     2     |     3     |   Enter   |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |   Lower   |           |           |   LWL0    |     0     |     .     |     *     |     /     |
  '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
*/
  [_LOWERW] = LAYOUT_ortho_1x4x12(
    _______,      _______,    TD(LBKTS),  TD(RBKTS),  KC_MINS,    TD(SUM),    KC_NO,      KC_NO,      KC_BSPC,     KC_P7,     KC_P8,      KC_P9,      KC_PMNS,
    _______,      LWL0W(TAB), CTL(HOME),  KC_PGDN,    KC_PGUP,    LWL1W(END), KC_BSPC,    KC_F4,      KC_F2,       KC_P4,     KC_P5,      KC_P6,      KC_PPLS,
    _______,      _______,    KC_LEFT,    KC_DOWN,    KC_UP,      KC_RGHT,    KC_NO,      _______,    KC_CALC,     KC_P1,     KC_P2,      KC_P3,      KC_PENT,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    LWL0W(SPC),  KC_P0,     KC_PAST,    KC_PDOT,    KC_PSLS
  ),

    /* LWL0 Windows
      .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
      |           |  |           |           |           |           |           |           |           |           |    BS     |     /     |     *     |    Del    |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  |   LWL0    |   Ctrl    |   Shift   |    Del    |   Del     |           |           |           |   Left    |   Down    |     Up    |   Right   |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  |           |           |           |           |           |           |           |           |   Home    |   Pg Dn   |   Pg Up   |    End    |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  |           |           |           |           |           |           |  TG(Lwr)  |   LWL0    |           |           |           |   LWL1    |
      '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
    */
      [_LWL0W] = LAYOUT_ortho_1x4x12(
        _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_BSPC,    KC_PSLS,    KC_PAST,    KC_DEL,
        _______,      _______,    KC_LCTL,    KC_LSFT,    KC_DEL,     KC_DEL,     _______,    _______,    _______,    KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT,
        _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_HOME,    KC_PGDN,    KC_PGUP,    KC_END,
        _______,      _______,    _______,    _______,    _______,    _______,    _______,    TG(8),	    _______,    _______,    _______,    _______,    _______
      ),
    
    /* LWL1 Windows
      .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
      |           |  |   RESET   |           |           |           |           |           |           |           |           |           |           |           |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  |           | Ctl+Home  |  Prt Scr  |           |   LWL1    |           |           |           |     $     |     ,     |     %     |           |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  |   Caps    |           |           |           |           |           |           |           |     !     |           |           |           |
      |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
      |           |  |   macOS   |           |           |           |           |           |           |           |           |           |  NumLock  |           |
      '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
    */
      [_LWL1W] = LAYOUT_ortho_1x4x12(
        _______,      RESET,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
        _______,      _______,    CTL_HOME,   PRNTSCRW,   _______,    _______,    _______,    _______,    _______,    KC_DLR,     KC_COMM,    KC_PERC,    _______,
        _______,      KC_CAPS,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_EXLM,    _______,    _______,    _______,
        _______,      TG(7),      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_NLCK,    _______
      ),

/* Raise Windows
  .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
  |           |  |    ` ~    |     1     |     2     |     3     |     4     |     5     |     6     |     7     |     8     |     9     |     0     |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |     !     |     @     |     #     |     $     |     %     |     ^     |     &     |     *     |     (     |     )     |     _     |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |     `     |           |           |           |  Insert   |           |     [     |     ]     |    \ |    |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |           |           |  TG(Lwr)  |   Raise   |   Play    |   Vol-    |   Vol+    |   Mute    |
  '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
*/
  [_RAISEW] = LAYOUT_ortho_1x4x12(
    _______,      TD(TILDE),  KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       _______,
    _______,      _______,    KC_EXLM,    TD(EMAIL),  KC_HASH,    KC_DLR,     KC_PERC,    KC_CIRC,    KC_AMPR,    KC_ASTR,    KC_LPRN,    KC_RPRN,    KC_UNDS,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    KC_INS,     _______,    KC_LBRC,    KC_RBRC,    TD(PIPE),   _______,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    TG(8),      _______,    KC_MPLY,    KC_VOLD,    KC_VOLU,    KC_MUTE
  ),

/* FN Windows
  .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
  |           |  |           |    F1     |    F2     |    F3     |    F4     |    F5     |    F6     |    F7     |    F8     |    F9     |   F10     |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |    Fn     |   Ctrl    |   Shift   |    Del    |   Del     |           |           |   Left    |    Down   |    Up     |   Right   |   Enter   |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           | Firmware  |           |           |   Home    |   Pg Dn   |   Pg Up   |    End    |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |   Enter   |           |           |           |           |           |   F11     |    F12    |
  '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
*/
  [_FUNCTIONW] = LAYOUT_ortho_1x4x12(
    _______,      _______,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     _______,
    _______,      _______,    KC_LCTL,    KC_LSFT,    KC_DEL,     KC_DEL,     _______,    _______,    ALT(LEFT),  KC_DOWN,    KC_UP,      ALT(RIGHT), KC_ENT,
    _______,      _______,    _______,    _______,    _______,    TD(FIRM),   _______,    _______,    CTL(HOME),  SFT(PGDN),  SFT(PGUP),  CTL(END),   _______,
    _______,      _______,    _______,    _______,    _______,    KC_ENT,     _______,    _______,    _______,    _______,    _______,    KC_F11,     KC_F12 
  ),

/* FN2 Windows
  .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
  |           |  |           |  Tab Left | Tab Right | Sht Left  | Sht Right |           |           |           |           |           |           |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |  Del Col  |  Ins Col  |  Del Row  |  Ins Row  |           |           |           |           |           |           |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |  Hide Col | Unhide Col|  Hide Row | Unhide Row|           |           |           |           |           |           |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |           |           |           |           |           |           |           |           |
  '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
*/ 
  [_FUNCTION2W] = LAYOUT_ortho_1x4x12(
    _______,      _______,    TAB_LEFTW,  TAB_RIGHTW, SHT_LEFTW,  SHT_RIGHTW, _______,    _______,    _______,    _______,    _______,    _______,    _______,
    _______,      _______,    DEL_COLW,   INS_COLW,   DEL_ROWW,   INS_ROWW,   _______,    _______,    _______,    _______,    _______,    _______,    _______,
    _______,      _______,    HIDE_COLW,  UNHIDE_COLW,HIDE_ROWW,  UNHIDE_ROWW,_______,   _______,    _______,    _______,    _______,    _______,    _______,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______ 
  )

};

const uint16_t PROGMEM maactions[] = {

};

/* uint16_t get_tapping_term(uint16_t keycode) {
  switch (keycode) {
    case CTL_T(KC_A):
 	 return 500;
	case SFT_T(KC_S):
	 return 500;
    default:
      return TAPPING_TERM;
  }
} */ 

// Macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // macOS
    case INS_ROW: // Insert row in Sheets
      if (record->event.pressed) { // when keycode is pressed
        SEND_STRING(SS_LCTL(SS_LALT("i") SS_DELAY(250)) "r"); // Ctrl+Alt+i, r
      } else { // when keycode is released
      }
      break;

    case DEL_ROW: // Delete row in Sheets
      if (record->event.pressed) { // when keycode is pressed
        SEND_STRING(SS_LCTL(SS_LALT("e") SS_DELAY(250)) "d"); // Ctrl+Alt+e, d
      } else { // when keycode is released
      }
      break;

    case INS_COL: // Insert column in Sheets
      if (record->event.pressed) { // when keycode is pressed
        SEND_STRING(SS_LCTL(SS_LALT("i") SS_DELAY(250)) "c"); // Ctrl+Alt+i, c
      } else { // when keycode is released
      }
      break;    

    case DEL_COL: // Delete column in Sheets
      if (record->event.pressed) { // when keycode is pressed
        SEND_STRING(SS_LCTL(SS_LALT("e") SS_DELAY(250)) "e"); // Ctrl+Alt+e, e
      } else { // when keycode is released
      }
      break;

    // Windows
    case INS_ROWW: // Insert row in Sheets
      if (record->event.pressed) { // when keycode is pressed
        SEND_STRING(SS_LALT(SS_LSFT("i") SS_DELAY(250)) "r"); // Alt+Shift+i, r
      } else { // when keycode is released
      }
      break;

    case DEL_ROWW: // Delete row in Sheets
      if (record->event.pressed) { // when keycode is pressed
        SEND_STRING(SS_LALT(SS_LSFT("e") SS_DELAY(250)) "d"); // Alt+Shift+e, d
      } else { // when keycode is released
      }
      break;

    case INS_COLW: // Insert column in Sheets
      if (record->event.pressed) { // when keycode is pressed
        SEND_STRING(SS_LALT(SS_LSFT("i") SS_DELAY(250)) "c"); // Alt+Shift+i, c
      } else { // when keycode is released
      }
      break;    

    case DEL_COLW: // zDelete column in Sheets
      if (record->event.pressed) { // when keycode is pressed
        SEND_STRING(SS_LALT(SS_LSFT("e") SS_DELAY(250)) "e"); // Alt+Shift+e, e
      } else { // when keycode is released
      }
      break;

  }
  return true;
};

// Encoder stuff
// #ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) {
    switch(biton32(layer_state)){
      // Base layers
      case 0: // Windows
      if (clockwise) { // Volume
        tap_code(KC_VOLD);
      } else {
        tap_code(KC_VOLU);
      }
      break;

      case 7: // macOS
      if (clockwise) { // Volume
        tap_code(KC_VOLD);
      } else {
        tap_code(KC_VOLU);
      }
      break;

      // Raise layers
      case 2: // Windows
      if (clockwise) { // Vertical scroll
        tap_code(KC_MS_WH_DOWN);
      } else {
        tap_code(KC_MS_WH_UP);
      }
      break;

      case 9: // macOS
      if (clockwise) { // Vertical scroll
        tap_code(KC_MS_WH_DOWN);
      } else {
        tap_code(KC_MS_WH_UP);
      }
      break;

      // Function layers
      case 3: // Windows
      if (clockwise) { // Horizontal scroll
        tap_code(KC_MS_WH_RIGHT);
      } else {
        tap_code(KC_MS_WH_LEFT);
      }
      break;

      case 10: // macOS
      if (clockwise) { // Horizontal scroll
        tap_code(KC_MS_WH_RIGHT);
      } else {
        tap_code(KC_MS_WH_LEFT);
      }
      break;

    }
  }
}
// #endif

// Tap dance stuff
static xtap firm_state = {
  .is_press_action = true,
  .state = 0
};

static xtap email_state = {
  .is_press_action = true,
  .state = 0
};

static xtap sum_state = {
  .is_press_action = true,
  .state = 0
};

static xtap lbkts_state = {
  .is_press_action = true,
  .state = 0
};

static xtap rbkts_state = {
  .is_press_action = true,
  .state = 0
};

static xtap tilde_state = {
  .is_press_action = true,
  .state = 0
};

// Flash firmware
void firm_finished (qk_tap_dance_state_t *state, void *user_data) {
  firm_state.state = cur_dance(state); // Use the dance that favors being held
  switch (firm_state.state) {
    case SINGLE_TAP: SEND_STRING("make ori:default:dfu"); break; // send ori make code
    case DOUBLE_TAP: SEND_STRING("make nori:default:avrdude"); break; // send nori make code
    case TRIPLE_TAP: SEND_STRING("make therick48:default:dfu"); break; // send therick48 make code
  }
}

void firm_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (firm_state.state) {
    case SINGLE_TAP: ; break;
    case DOUBLE_TAP: ; break;
    case TRIPLE_TAP: ; break;
  }
  firm_state.state = 0;
}

// email
void email_finished (qk_tap_dance_state_t *state, void *user_data) {
  email_state.state = cur_dance(state); // Use the dance that favors being held
  switch (email_state.state) {
    case SINGLE_TAP: register_code(KC_LSFT); register_code(KC_2); break; //send @
    case DOUBLE_TAP: SEND_STRING("/email"); break; // send email address
    case TRIPLE_TAP: SEND_STRING("/workemail"); // send work email
  }
}

void email_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (email_state.state) {
    case SINGLE_TAP: unregister_code(KC_LSFT); unregister_code(KC_2); break; // unregister @
    case DOUBLE_TAP: ; break;
    case TRIPLE_TAP: ; break;
  }
  email_state.state = 0;
}

// Sum
void sum_finished (qk_tap_dance_state_t *state, void *user_data) {
  sum_state.state = cur_dance(state); // Use the dance that favors being held
  switch (sum_state.state) {
    case SINGLE_TAP: register_code(KC_EQL); break; // send =
    case DOUBLE_TAP: SEND_STRING("=sum("); break; // =sum(
    case TRIPLE_TAP: SEND_STRING("=iferror(vlookup("); // send =iferror(vlookup(
  }
}

void sum_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (sum_state.state) {
    case SINGLE_TAP: unregister_code(KC_EQL); break; // unregister =
    case DOUBLE_TAP: ; break;
    case TRIPLE_TAP: ; break;
  }
  sum_state.state = 0;
}

// Left brackets 
void lbkts_finished (qk_tap_dance_state_t *state, void *user_data) {
  lbkts_state.state = cur_dance(state); // Use the dance that favors being held
  switch (lbkts_state.state) {
    case SINGLE_TAP: register_code(KC_LSFT); register_code(KC_9); break; // send (
    case DOUBLE_TAP: register_code(KC_LBRC); break; // send [
    case TRIPLE_TAP: register_code(KC_LSFT); register_code(KC_LBRC); // send {
  }
}

void lbkts_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (lbkts_state.state) {
    case SINGLE_TAP: unregister_code(KC_LSFT); unregister_code(KC_9); break; // unregister (
    case DOUBLE_TAP: unregister_code(KC_LBRC); break; // unregister [
    case TRIPLE_TAP: unregister_code(KC_LSFT); unregister_code(KC_LBRC); // unregsister {
  }
  lbkts_state.state = 0;
}

// Right brackets
void rbkts_finished (qk_tap_dance_state_t *state, void *user_data) {
  rbkts_state.state = cur_dance(state); //Use the dance that favors being held
  switch (rbkts_state.state) {
    case SINGLE_TAP: register_code(KC_LSFT); register_code(KC_0); break; // send )
    case DOUBLE_TAP: register_code(KC_RBRC); break; // send ]
    case TRIPLE_TAP: register_code(KC_LSFT); register_code(KC_RBRC); // send }
  }
}

void rbkts_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (rbkts_state.state) {
    case SINGLE_TAP: unregister_code(KC_LSFT); unregister_code(KC_0); break; // unregister )
    case DOUBLE_TAP: unregister_code(KC_RBRC); break; // unregister ]
    case TRIPLE_TAP: unregister_code(KC_LSFT); unregister_code(KC_RBRC); // unregsister }
  }
  rbkts_state.state = 0;
}

// Tilde
void tilde_finished (qk_tap_dance_state_t *state, void *user_data) {
  tilde_state.state = cur_dance(state); // Use the dance that favors being held
  switch (tilde_state.state) {
    case SINGLE_TAP: register_code(KC_GRAVE); break; // send `
    case DOUBLE_TAP: register_code(KC_LSFT); register_code(KC_GRAVE); break; // send ~
    case TRIPLE_TAP: SEND_STRING("```"); // ```
  }
}

void tilde_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (tilde_state.state) {
    case SINGLE_TAP: unregister_code(KC_GRAVE); break; // unregister `
    case DOUBLE_TAP: unregister_code(KC_LSFT); unregister_code(KC_GRAVE); break; // unregister ~
    case TRIPLE_TAP: ; break;
  }
  tilde_state.state = 0;
}

/* FN2
  .--------------------------------------------------------------------------------------------------------------------------------------------------------------.
  |           |  |           |           |           |           |           |           |           |           |           |           |           |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |           |           |           |           |           |           |           |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |           |           |           |           |           |           |           |           |
  |-----------|  |-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------|
  |           |  |           |           |           |           |           |           |           |           |           |           |           |           |
  '--------------------------------------------------------------------------------------------------------------------------------------------------------------'
*/ 
/*
  [_FN2] = LAYOUT_ortho_1x4x12(
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______ 
  ) */