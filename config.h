// Key definition macros
#define BothShiftKeys ShiftMask, XK_Shift_R
#define MODKEY Mod3Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << (TAG - 1)} }, \
    { Mod1Mask,                     KEY,      toggleview,     {.ui = 1 << (TAG - 1)} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << (TAG - 1)} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << (TAG - 1)} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << (TAG - 1)} },

// Macro for unused actions to silence warnings
#define UNUSED(action) { 0, 0, action, { .i = 0 } },

// Tag bit-mask macros
#define TAG(N) (1 << (N - 1))
#define TAGS2(A, B) (TAG(A) | TAG(B))
#define TAGS3(A, B, C) (TAG(A) | TAG(B) | TAG(C))

// Shell command helper
#define SHELL(cmd) { .v = (const char*[]) { "/bin/sh", "-c", cmd, NULL } }
// Helper for launching arbitrary commands
#define EXECL(...) { .v = (const char*[]) { __VA_ARGS__, NULL } }

// Rule macros
#define CLASS(C) (C), NULL, NULL
#define INSTANCE(I) NULL, (I), NULL
#define TITLE(T) NULL, NULL, (T)
#define CLASS_W_TITLE(C, T) (C), NULL, (T)

static const float mfact      = 0.668;
static const int nmaster      = 1;
static const int resizehints  = 0;

static const unsigned int borderpx  = 1;
static const unsigned int snap      = 0;
static const int showbar            = 1;
static const int topbar             = 1;
static const int defaultpriority    = 50;

static const unsigned int systraypinning = 0;
static const unsigned int systrayspacing = 4;
static const Bool showsystray = 1;

// 1: if pinning fails, display systray on the first monitor
// 0: display systray on the last monitor
static const Bool systraypinningfailfirst = 1;

static const char orange_red[]  = "#ff4500";
static const char blue[]        = "#224488";
static const char bright_blue[] = "#0066ff";
static const char black[]       = "#000000";
static const char gray[]        = "#bbbbbb";
static const char white[]       = "#ffffff";

/**
 * Color scheme definitions
 *
 * Each entry in the array is a set of three, hexadecimal colors representing
 * the foreground, backgroudn and border color for the spcified scheme.
 */
static const char *colors[][3] = {
    // Scheme           Foreground  Background  Border
    [SchemeNorm]    = { gray,       black,      black,      },
    [SchemeSel]     = { white,      blue,       bright_blue },
    [SchemeUrg]     = { white,      orange_red, orange_red  },
};

/**
 * Array of primary and fallback fonts
 *
 * The first array is the primary font while the rest of the fonts will be used
 * as fallbacks when preceding fonts are missing a specific glyph.
 */
static const char *fonts[] = {
    "Sans:pixelsize=14",
    "VL Gothic:pixelsize=14",
    "WenQuanYi Micro Hei:pixelsize=14",
};

/**
 * Labels used for each tag
 */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "Chats" };

/**
 * Window rules
 *
 * To find out a window's class, run "xprop | grep '^WM_CLASS'", then click on
 * the window. The instance is the first item in the list and the class the
 * second.
 */
static const Rule rules[] = {
    // Match conditions                                               Tags      Float  Monitor  Priority

    { TITLE("File Operation Progress"),                               0,        1,     0,       0   },
    { INSTANCE("eog"),                                                0,        1,     0,       0   },
    { INSTANCE("gpick"),                                              0,        1,     0,       0   },
    { CLASS("VirtualBox"),                                            TAG(4),   0,     0,       0   },
    { CLASS("Pidgin"),                                                TAG(9),   0,     0,       20  },
    { CLASS("st-256color|xterm|rxvt"),                                TAG(1),   0,     0,       100 },

    // Firefox and Chrome both go on tag 2. I use Chromium at home, and at
    // work, I use Google Chrome, so Chrome and Firefox are never run together.
    { CLASS("Firefox|Iceweasel"),                                     TAG(2),   0,     0,       0   },
    { INSTANCE("google-chrome"),                                      TAG(2),   0,     0,       0   },
    { INSTANCE("chromium"),                                           TAG(3),   0,     0,       0   },

    // Google Hangouts Chrome Extension; all Chrome extensions have instance
    // names of "crx_$EXTENSION_ID".
    { INSTANCE("crx_nckgahadagoaajjgafhacjanaoiihapd"),               TAG(9),   0,     0,       20  },

    // All Steam windows go on the 5th tag, and anything other than the main
    // Steam window should have a reduced attachment priority. When the chat
    // windows are first created, they are created without a properly set
    // title, so dwm defaults to setting the window title to "broken."
    { CLASS_W_TITLE("Steam", "Friends|- Chat|broken|Update News"),    TAG(5),   0,     0,       10  },
    { CLASS("Steam"),                                                 TAG(5),   0,     0,       0   },
};

/**
 * Layout variable names
 *
 * These enum elements can be re-arranged to change the default layout.
 * Deletions from or additions to this list should be accompanied with changes
 * to the "layouts" variable.
 */
enum {
    monocle_layout,
    tile_layout,
    floating_layout,
};

/**
 * Layout symbols and functions
 *
 * Deletions from or additions to this list should be accompanied with changes
 * to the layout variable names enum.
 */
static const Layout layouts[] = {
    [monocle_layout] =      { "[M]",    monocle },
    [tile_layout] =         { "[]=",    tile },
    [floating_layout] =     { "><>",    NULL },
};

/**
 * Layout rules that trigger based on active tags
 */
static const TagRule tagrules[] = {
    // Tag          Exact   Monitor  Mn,Mx      nmaster Layout Index        mfact
    { TAG(9),       0,      -1,      1, 0,      -1,     tile_layout,        -1  },
    { TAG(1),       0,      -1,      2, 0,      -1,     tile_layout,        0.5 },
    { TAGS2(2, 3),  0,      -1,      2, 0,      -1,     tile_layout,        0.5 },
};

/**
 * Rules hook
 *
 * This function is called once applyrules is done processing a client with the
 * client in question passed as an argument.
 */
void ruleshook(Client *c)
{
    // Certain floating Wine windows always get positioned off-screen. When
    // that happens, this code will center them.
    if (!strcmp(c->class, "Wine") && c->x < 1) {
        c->x = c->mon->mx + (c->mon->mw / 2 - WIDTH(c) / 2);
        c->y = c->mon->my + (c->mon->mh / 2 - HEIGHT(c) / 2);
    }

    // Mark windows that get created offscreen as urgent.
    if (!scanning && !ISVISIBLE(c)) {
        seturgent(c);
    }
}

/**
 * Keyboard shortcuts
 */
static Key keys[] = {
    // Modifier                     Key                     Function         Argument
    { MODKEY,                       XK_j,                   focusstack,      {.i = +1 } },
    { MODKEY,                       XK_k,                   focusstack,      {.i = -1 } },
    { MODKEY,                       XK_e,                   toggleview,      {.ui = TAG(9) } },
    { MODKEY,                       XK_i,                   incnmaster,      {.i = +1 } },
    { MODKEY,                       XK_d,                   incnmaster,      {.i = -1 } },
    { MODKEY,                       XK_h,                   setmfact,        {.f = -0.05} },
    { MODKEY,                       XK_l,                   setmfact,        {.f = +0.05} },
    { MODKEY,                       XK_Return,              zoom,            {0} },
    { Mod1Mask,                     XK_Tab,                 view,            {0} },
    { MODKEY,                       XK_w,                   killclient,      {0} },
    { MODKEY,                       XK_t,                   setlayout,       {.v = &layouts[tile_layout]} },
    { MODKEY,                       XK_m,                   setlayout,       {.v = &layouts[monocle_layout]} },
    { MODKEY,                       XK_f,                   setlayout,       {.v = &layouts[floating_layout]} },
    { MODKEY,                       XK_s,                   lastclient,      {0} },
    { 0,                            XK_Print,               spawn,           SHELL("mkdir -p ~/screenshots/ && cd ~/screenshots/ && scrot") },
    { Mod1Mask,                     XK_Print,               spawn,           SHELL("mkdir -p ~/screenshots/ && cd ~/screenshots/ && scrot -u") },
    { MODKEY,                       XK_q,                   spawn,           EXECL("session-logout") },
    { Mod4Mask,                     XK_l,                   spawn,           SHELL("eval \" $DISPLAY_LOCKER\"") },
    { MODKEY,                       XK_space,               spawn,           SHELL("dwm-launcher") },
    { MODKEY,                       XK_0,                   spawn,           EXECL("dwm-shortcuts", "play") },
    { MODKEY,                       XK_bracketleft,         spawn,           EXECL("dwm-shortcuts", "prev") },
    { MODKEY,                       XK_bracketright,        spawn,           EXECL("dwm-shortcuts", "next") },
    { MODKEY,                       XK_minus,               spawn,           EXECL("dwm-shortcuts", "volume_down") },
    { MODKEY,                       XK_equal,               spawn,           EXECL("dwm-shortcuts", "volume_up") },
    { MODKEY,                       XK_b,                   spawn,           EXECL("bass-toggle") },
    { BothShiftKeys,                                        spawn,           EXECL("getpass", "-sb", "#aa0000", "-nb", "#222222") },

    TAGKEYS(                        XK_1,                      1)
    TAGKEYS(                        XK_2,                      2)
    TAGKEYS(                        XK_3,                      3)
    TAGKEYS(                        XK_4,                      4)
    TAGKEYS(                        XK_5,                      5)
    TAGKEYS(                        XK_6,                      6)
    TAGKEYS(                        XK_7,                      7)
    TAGKEYS(                        XK_8,                      8)
    TAGKEYS(                        XK_9,                      9)

    UNUSED(focusmon)
    UNUSED(quit)
    UNUSED(spawn)
    UNUSED(tagmon)
    UNUSED(togglebar)
};

/**
 * Mouse button actions
 *
 * Click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or
 * ClkRootWin.
 */
static Button buttons[] = {
    // Click                Event mask      Button          Function        Argument

    // Left-click the layout button to toggle between layouts.
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },

    // Right-click the layout button to set monocle mode.
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[monocle_layout]} },

    // Middle-mouse-click a window with Alt or MODKEY to toggle floating.
    { ClkClientWin,         Mod1Mask,       Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },

    // Alt + right-click or MODKEY + right-click and drag to resize a window.
    { ClkClientWin,         Mod1Mask,       Button3,        resizemouse,    {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },

    // Alt + left-click and drag to move a window.
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },

    // Left-click a tag to switch to that tag alone.
    { ClkTagBar,            0,              Button1,        view,           {0} },

    // Right-click a tag to toggle its status.
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },

    // MODKEY + left-click to move a window to the clicked tag.
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },

    // MODKEY + right-click a tag to it for the currently selected window.
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },

    // Scroll wheel up and down on the bar to cycle between windows.
    { ClkWinTitle,          0,              Button5,        focusstack,     {.i = +1} },
    { ClkWinTitle,          0,              Button4,        focusstack,     {.i = -1} },

    // Middle-mouse-click on the title bar to close a window.
    { ClkWinTitle,          0,              Button2,        killclient,     {0} },
};

// Unused settings
char *dmenucmd = NULL;
char *dmenumon = NULL;
