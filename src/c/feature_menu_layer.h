#include <pebble.h>
#include <pebble_process_info.h>

typedef enum ITEMS {
	TOTAL			    	= 0,
 	COCAINE 		  	= 1,
 	HEROINE 		  	= 2,
 	ACID			    	= 3,
 	WEED			    	= 4,
 	SPEED			    	= 5,
	LUDES			    	= 6,
} TRENCHCOAT_ITEMS;

enum LOCATIONS {
		NEVERMIND	  	= 0,
		BRONX       	= 1,
		GHETTO       	= 2,
		CENTRAL_PARK 	= 3,
		MANHATTEN    	= 4,
		CONEY_ISLAND 	= 5,
		BROOKLYN     	= 6
};

enum KEYS {
	VERSION			  	= 0,
	VIBRATE			  	= 1,
	LIGHT 			  	= 2,
	DAYS				    = 3,
	AUTOSAVE			  = 4,
  READY           = 5
};

#define HIGH_SCORE_KEY					      0
#define SETTINGS_DATA_KEY				      247
#define PLAYER_SIZE_KEY					      20
#define PLAYER_DATA_KEY					      24
#define NUM_MENU_ICONS 					      9
#define MENU_CELL_BASIC_HT			  	  19
#define	MENU_CELL_BASIC_HEADER_HT		  35
#define SUBTITLED_MENU_HEADER_HT	   	54
#define TEXT_ORIGIN_Y_SHIFT           -7
#define NUM_MENU_SECTIONS			      	1
#define NUM_HOME_MENU_ITEMS 		    	7
#define NUM_PRICES_MENU_ITEMS		    	7
#define NUM_SELL_MENU_ITEMS			    	7
#define BASIC_ITEM_LENGTH			      	19
#define MAX_ITEM_LENGTH				      	63
#define SHORT_MESSAGE_DELAY			    	2000
#define LONG_MESSAGE_DELAY			    	5000
#define PUNISHMENT_DELAY			      	10000

#define HEADER_FONT                   fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD)
#define CELL_FONT                     fonts_get_system_font(FONT_KEY_GOTHIC_24)
#define CONFIRM_FONT                  fonts_get_system_font(FONT_KEY_GOTHIC_24)

MenuLayer 		  *home_menu_layer;
MenuIndex 		  *p_NumWindowContext = NULL;
GBitmap 	      *menu_icons[NUM_MENU_ICONS];
GBitmap 	      *game_icon = NULL;

// Get app version info
extern const    PebbleProcessInfo __pbl_app_info;

// In-Game Variables
int				      value, X, Y, Score;
bool			      num_window_is_visible;
short 			    current_icon;
short			      menu_number = 0;

// HighScore Array
int high_scores[4];
								
const short     NUM_DAYS[4] = {30, 15, 45, 60};
const char      postfix[4] 	= {' ','K', 'M', 'B'};

// Menu Header Heights
const short     menu_header_heights[10] =
{
	MENU_CELL_BASIC_HEADER_HT			,
	MENU_CELL_BASIC_HEADER_HT			, 
	SUBTITLED_MENU_HEADER_HT			,
	MENU_CELL_BASIC_HEADER_HT			, 
	MENU_CELL_BASIC_HEADER_HT			, 
	SUBTITLED_MENU_HEADER_HT			,
	MENU_CELL_BASIC_HEADER_HT * 3	,
	MENU_CELL_BASIC_HEADER_HT * 3 ,
	SUBTITLED_MENU_HEADER_HT  + MENU_CELL_BASIC_HT,
	SUBTITLED_MENU_HEADER_HT  + 3 * MENU_CELL_BASIC_HT
};

// Drug Names
const char* drug_names[7] =
{
	"TOTAL",
 	"DOGE",
 	"SPONGEBOB",
 	"ODNS",
 	"GIFS",
 	"BRUH",
	"FAGGOTRY",
};

// Messages
const char* messages[12] = 
{
  "MEMEWARS\nFOR PEBBLE\nv%s\nHACKED BY ELIJAH\n2016\nWISCONSIN ,USA",      // 0
  "RIVAL DEALERS ARE SELLING CHEAP FAGGOTRY MEMES!\n",                         // 1
  "YOUR MEMES WERE STOLEN IN THE SUBWAY!\nYOU LOST $%i!",                    // 2
  "YOUR MEMES WERE STOLEN IN THE SUBWAY!\nYOU LOST $%i AND %i OF YOUR %s!",  // 3
  "OFFICER MEMEHATER AND %i TROLLS ARE AFTER YOU!",                   // 4
  "OFFICER MEMEHATER IS AFTER YOU!",                                    // 5
  "WILL YOU BUY\n%s BARS FOR $%i?",                                   // 6
  "WILL YOU BUY A \n%s \n FOR $%i? ",                                 // 7
  "WILL YOU BUY A BIGGER PHONE STORAGE FOR $%i?",                        // 8
  "YOU FOUND %i UNITS OF %s ON A DEAD DUDE IN THE SUBWAY!",           // 9
  "THERE'S SOME GIFS HERE THAT SMELLS DANK!",                         // 10
  "WANNA WATCH IT?"                                                   // 11
};

// Gun Names
const char* gun_names[4] =
{
	"Total",
	"GOOGLE+",
	"IFUNNY",
	"FACEBOOK"
};

// Home Menu
const char* menu_items[8] =
{
	"BACK",
	"PRICES",
	"PHONE",
	"BUY",
	"SELL",
	"SUBWAY",
	"MEME SHARK",
	"BANK"
};

// Trenchcoat Menu
const char* trenchcoat_items[6] =
{
	"BACK",
	"MEMES",
	"SERVICES",
	"BARS",
	"DAMAGE",
	"FREESPACE"
};

// Subway Menu
const char* locations[7] =
{
	"",
	"TEH INTERWEBZ",
	"THE MEMO",
	"CENTRAL MEME",
	"MEMEHATTEN",
	"MEME ISLAND",
	"MEMELYN"
};

// Loan Menu
const char* loan_menu[3] =
{
	"",
	"PAY",
	"BORROW"
};

// Bank Menu
const char* bank_menu[3] =
{
	"",
	"WITHDRAW",
	"DEPOSIT"
};

// Chased Menu
const char* chased_menu[7] =
{
	"%1i HATER",
	"IN PERSUIT!!",
	"BARS",
	"SERVICES",
	"DAMAGE",
	"RUN",
	"FIGHT"
};

// Confirm Menu
const char* confirm_menu[2] =
{
	"NAH",
	"YEAH"
};

typedef void					                  (*MenuCallback)(MenuIndex *);

// In-Game functions
void 	Intro					                    (MenuIndex *);
void 	Being_Shot				                (MenuIndex *);
void 	Buy_Trenchcoat			              (MenuIndex *);
void 	Buy_Gun					                  (MenuIndex *);
void 	Cop_187					                  (MenuIndex *);
void 	Doctor					                  (MenuIndex *);
void 	Event_Generator			              (MenuIndex *);
void 	Exit					                    (MenuIndex *);
void 	Game_Over				                  (MenuIndex *);
void	Load_Game				                  (MenuIndex *);
void 	Smoke_It				                  (MenuIndex *);
void 	UpdateFreespace			              (MenuIndex *);
void 	Play_Again				                (MenuIndex *);
void 	BuyDrugs				                  (int, MenuIndex *);
void 	SellDrugs				                  (int, MenuIndex *);
void 	Save_Game				                  (void);
void  set_invert_layer		              (void);
void 	Show_Instructions		              (void *);
void 	show_number_window_layer          (void *);
void 	hide_number_window_layer          (void);
void 	num_selected_callback	            (struct NumberWindow *, void *);
void 	number_window_incremented_callback(struct NumberWindow *, void *);

MenuCallback p_MenuCallbackContext[2] = {NULL, NULL};

// Pebble wrapper conditional functions
void 	Num_Input(char *, int, int, int, int, MenuIndex *);

// App specific number functions
int 	LOG10(int val);
int 	EXP(int val);
void 	floatstrcat(char *, double, int);

// Menu Header Draw function for Title only
void 	menu_header_simple_draw(GContext *, const Layer *, const char *);

// Menu Header Draw function for Icon and Title
void 	menu_header_simple_icon_draw(GContext *, const Layer *, const char *, const GBitmap *);

// Menu Header Draw function for Icon, Title, and Subtitle
void 	menu_header_draw(GContext *, const Layer *, const char *, const char *, const GBitmap *);

// Menu Header Draw function for titles (multiple-lines)
void 	menu_header_long_draw(GContext *, const Layer *, const char *);

// Menu Row Draw function for Title only
void 	menu_cell_simple_draw(GContext *, const Layer *, const char *);

// Menu Row Draw function for Title w/ Value
void 	menu_cell_value_draw(GContext* , const Layer *, const char *, const char *);

// Menu Row Draw function for Title only (Centered)
void 	menu_cell_simple_centered_draw(GContext *, const Layer *, const char *);

// Menu Row Draw function for Icon and Title
void 	menu_cell_simple_icon_draw(GContext *, const Layer *, const char *, const GBitmap *);

//! Menu row drawing function to draw a basic cell with the title, subtitle, and icon. 
//! Call this function inside the `.draw_row` callback implementation, see \ref MenuLayerCallbacks.
//! @param ctx The destination graphics context
//! @param cell_layer The layer of the cell to draw
//! @param title Draws a title in larger text (18 points, Lucida Console font).
//! @param subtitle Draws a subtitle in smaller text (14 points, Lucida Console font).
//! @param icon Draws an icon to the left of the text.
void 	menu_cell_draw(GContext *, const Layer *, const char *, const char *, const GBitmap *);

char	*format 		= NULL;
char	*string 		= NULL;
char	*strval			= NULL;
char 	*confirm_header = NULL;
char	*version 		= "0.00";
char 	*number_window_value_text = NULL;