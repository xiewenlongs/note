#include <curses.h> 

#define MAX_CHOICES       300
#define MIN_HEIGHT        7
#define MIN_WIDTH         0
#define MAX_HEIGHT_FOOTER 1
#define MAX_HEIGHT_HEADER 6

#define CONF_MENU_H       6
#define CONF_MENU_W       48
#define CONF_MENU_X       2
#define CONF_MENU_Y       4
#define CONF_WIN_H        17
#define CONF_WIN_W        52

#define FIND_DLG_HEIGHT   8
#define FIND_DLG_WIDTH    50
#define FIND_MAX_MATCHES  1

#define SCHEME_MENU_H     2
#define SCHEME_MENU_W     38
#define SCHEME_MENU_X     2
#define SCHEME_MENU_Y     4
#define SCHEME_WIN_H      8
#define SCHEME_WIN_W      42

#define SORT_MENU_H       4
#define SORT_MENU_W       38
#define SORT_MENU_X       2
#define SORT_MENU_Y       4
#define SORT_WIN_H        11
#define SORT_WIN_W        42

#define AGENTS_MENU_X     2
#define AGENTS_MENU_Y     4

#define HELP_MENU_HEIGHT  12
#define HELP_MENU_WIDTH   52
#define HELP_MENU_X       2
#define HELP_MENU_Y       4
#define HELP_WIN_HEIGHT   17
#define HELP_WIN_WIDTH    56

#define BLACK_CYAN        9
#define BLACK_GREEN       8
#define BLUE_GREEN        7
#define COL_BLACK         4
#define COL_BLUE          1
#define COL_CYAN          5
#define COL_GREEN         11
#define COL_RED           3
#define COL_WHITE         0
#define COL_YELLOW        6
#define WHITE_RED         10

#define HIGHLIGHT         1

#define TOTAL_MODULES     11

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

static WINDOW *header_win, *main_win;

typedef enum MODULES
{
   VISITORS,
   REQUESTS,
   REQUESTS_STATIC,
   NOT_FOUND,
   HOSTS,
   OS,
   BROWSERS,
   REFERRERS,
   REFERRING_SITES,
   KEYPHRASES,
   STATUS_CODES,
} GModule;

typedef enum SCHEMES
{
   MONOCHROME = 1,
   STD_GREEN
} GShemes;

typedef struct GFind_
{
   GModule module;
   char *pattern;
   int next_idx;
   int next_parent_idx;
   int next_sub_idx;
   int look_in_sub;
   int done;
   int icase;
} GFind;

typedef struct GSortModule_
{
   int hits;
   int data;
   int bw;
   int usecs;
   const char *choices[5];
} GSortModule;

typedef struct GSort_
{
   GModule module;
   enum
   {
      SORT_BY_HITS,
      SORT_BY_DATA,
      SORT_BY_BW,
      SORT_BY_USEC,
   } field;
   enum
   {
      SORT_ASC,
      SORT_DESC
   } sort;
} GSort;

typedef struct GScrollModule_
{
   int scroll;
   int offset;
} GScrollModule;

typedef struct GScrolling_
{
   GScrollModule module[TOTAL_MODULES];
   GModule current;
   int dash;
   int expanded;
} GScrolling;

typedef struct GAgents_
{
   char *agents;
   int size;
} GAgents;


void
set_input_opts (void)
{
   noecho ();
   halfdelay (10);
   nonl ();
   intrflush (stdscr, FALSE);
   keypad (stdscr, TRUE);
   curs_set (0);
}

void
init_colors ()
{
   use_default_colors ();

   init_pair (COL_BLUE, COLOR_BLUE, -1);
   init_pair (COL_GREEN, COLOR_GREEN, -1);

   init_pair (COL_RED, COLOR_RED, -1);
   init_pair (COL_BLACK, COLOR_BLACK, -1);
   init_pair (COL_CYAN, COLOR_CYAN, -1);
   init_pair (COL_YELLOW, COLOR_YELLOW, -1);

   init_pair (BLUE_GREEN, COLOR_BLUE, COLOR_GREEN);

   init_pair (BLACK_GREEN, COLOR_BLACK, COLOR_GREEN);
   init_pair (BLACK_CYAN, COLOR_BLACK, COLOR_CYAN);
   init_pair (WHITE_RED, COLOR_WHITE, COLOR_RED);
}

main() 
{ 
    int row, col;
    initscr(); 
    set_input_opts ();
    start_color ();
    init_colors ();

    attron (COLOR_PAIR (COL_WHITE));
    getmaxyx (stdscr, row, col);

    header_win = newwin (5, col, 0, 0);
    keypad (header_win, TRUE);

    main_win = newwin (row - 7, col, 6, 0);
    keypad (main_win, TRUE);

    char *cstm_log, *cstm_date;
    int c, quit = 1;
    size_t i, n;
    int invalid = 1;
    int y, x, h = CONF_WIN_H, w = CONF_WIN_W;
    int w2 = w - 2;

    /* conf dialog menu options */
    char *choices[] = {
       "Common Log Format (CLF)",
       "Common Log Format (CLF) with Virtual Host",
       "NCSA Combined Log Format",
       "NCSA Combined Log Format with Virtual Host",
       "W3C",
       "CloudFront (Download Distribution)"
    };
    n = 6;
    getmaxyx (stdscr, y, x);


    WINDOW *win = newwin (h, w, (y - h) / 2, (x - w) / 2);
    keypad (win, TRUE);
    wborder (win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh (win);
    sleep (3);
    endwin(); 
} 
