/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=8" };
static const char dmenufont[]       = "monospace:size=8";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_norm_border[] = "#161616";
static const char col_sel_border[]  = "#ffffff";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_norm_border },
	[SchemeSel]  = { col_gray4, col_cyan,  col_sel_border  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance  title           tags mask  iscentered isfloating  isterminal  noswallow  monitor */
	{ "Gimp",         NULL,     NULL,           0,           0,          1,          0,           0,        -1 },
	{ "discord",      NULL,     NULL,           1 << 7,      0,          0,          0,          -1,        -1 },
	{ "firefox",      NULL,     NULL,           1 << 8,      0,          0,          0,          -1,        -1 },
	{ "Pavucontrol",  NULL,     NULL,           0,           1,          1,          0,           0,        -1 },
	{ "St",           NULL,     NULL,           0,           0,          0,          1,           0,        -1 },
	{ NULL,           NULL,     "Event Tester", 0,           0,          0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */
static int attachbelow = 0;          /* 1 means attach at the end */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      grid },
	{ "[D]",      deck },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *logoutcmd[]  = { "killall", "xinit", NULL };

static Key keys[] = {
	/* modifier             key                function        argument */
	{ MODKEY,               XK_b,              togglebar,      {0} },
	{ MODKEY,               XK_j,              focusstack,     {.i = +1 } },
	{ MODKEY,               XK_k,              focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,     XK_j,              movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,     XK_k,              movestack,      {.i = -1 } },
	{ MODKEY,               XK_apostrophe,     incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,     XK_apostrophe,     incnmaster,     {.i = -1 } },
	{ MODKEY,               XK_h,              setmfact,       {.f = -0.05} },
	{ MODKEY,               XK_l,              setmfact,       {.f = +0.05} },
    { MODKEY,               XK_period,         shiftview,      { .i = +1 } },
    { MODKEY,               XK_comma,          shiftview,      { .i = -1 } },
	{ MODKEY,               XK_z,              zoom,           {0} },
	{ MODKEY,               XK_Tab,            view,           {0} },
    { MODKEY,               XK_space,          focusmaster,    {0} },
    { MODKEY|ShiftMask,     XK_space,          togglefloating, {0} },
    { MODKEY,               XK_grave,          toggleattach,   {0} },
	{ MODKEY,               XK_q,              killclient,     {0} },
	{ MODKEY|ShiftMask,     XK_q,              killunsel,      {0} },
	{ MODKEY,               XK_p,              setlayout,      {.v = &layouts[0]} }, // tiled
	{ MODKEY,               XK_f,              setlayout,      {.v = &layouts[1]} }, // floating
	{ MODKEY,               XK_y,              setlayout,      {.v = &layouts[2]} }, // monocle
	{ MODKEY,               XK_g,              setlayout,      {.v = &layouts[3]} }, // grid
	{ MODKEY,               XK_c,              setlayout,      {.v = &layouts[4]} }, // deck
	{ MODKEY,               XK_r,              setlayout,      {.v = &layouts[5]} }, // centeredmaster
	{ MODKEY|ShiftMask,     XK_r,              setlayout,      {.v = &layouts[6]} }, // centeredfloatingmaster
	{ MODKEY,               XK_0,              view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,     XK_0,              tag,            {.ui = ~0 } },
	TAGKEYS(                XK_1,                              0)
	TAGKEYS(                XK_2,                              1)
	TAGKEYS(                XK_3,                              2)
	TAGKEYS(                XK_4,                              3)
	TAGKEYS(                XK_5,                              4)
	TAGKEYS(                XK_6,                              5)
	TAGKEYS(                XK_7,                              6)
	TAGKEYS(                XK_8,                              7)
	TAGKEYS(                XK_9,                              8)
	{ MODKEY,               XK_Delete,         quit,           {0} },
	{ MODKEY|ShiftMask,     XK_Delete,         spawn,          {.v = logoutcmd } },

    /*start applications*/
	{ MODKEY,               XK_Return,         spawn,          SHCMD("$TERMINAL") },
	{ MODKEY|ShiftMask,     XK_Return,         spawn,          {.v = dmenucmd } },
	{ MODKEY,               XK_w,              spawn,          SHCMD("$BROWSER") },
	{ MODKEY,               XK_F1,             spawn,          SHCMD("st -d $PROG_SOURCE_HOME") },
	{ MODKEY,               XK_m,              spawn,          SHCMD("pavucontrol") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          SHCMD("$TERMINAL") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

