/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>


/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Ubuntu:weight=bold:size=10:antialias=true:hinting=true",
                                        "Hack:size=10:antialias=true:autohint=true",
                                        "JoyPixels:size=10:antialias=true:autohint=true" 
                                      };
static const char dmenufont[]       	= "monospace:size=11";
static const char normbgcolor[]         = "#222222";
static const char normbordercolor[]     = "#444444";
static const char normfgcolor[]         = "#bbbbbb";
static const char selfgcolor[]          = "#eeeeee";
static const char selbordercolor[] 	    = "#5294e2";
static const char selbgcolor[]          = "#5294e2";
static const char titlebordercolor[] 	= "#5294e2";
static const char titlebgcolor[]        = "#242424";
static const char titlefgcolor[]        = "#eeeeee";
static const char *colors[][3]          = {
	/*               fg           bg           border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor },
	/*[SchemeTitle] = { titlefgcolor, titlebgcolor }, */
};

/* tagging */
static const char *tags[] = { "term", "www", "dev", "steam", "images", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "< >",      NULL },    /* no layout function means floating behavior */
	{ "[ ]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", selbgcolor, "-nf", selfgcolor, "-sb", selbgcolor, "-sf", selbgcolor, NULL };
static const char *termcmd[]  		= { "kitty", NULL };
static const char *firefoxcmd[] 	= { "firefox", NULL };
static const char *steamcmd[] 		= { "steam", NULL };
static const char *thunarcmd[] 		= { "thunar", NULL };
static const char *vscodiumcmd[] 	= { "vscodium", NULL };
static const char *gimpcmd[] 			= { "gimp", NULL };

/* Volume/Brightness controls */
static const char *voldown[] 	= { "/usr/bin/pactl", "set-sink-volume", "0", "-10%", NULL };
static const char *volmute[] 	= { "/usr/bin/pactl", "set-sink-mute", "0", "toggle", NULL };
static const char *volup[] 		= { "/usr/bin/pactl", "set-sink-volume", "0", "+10%", NULL };
static const char *dimmer[] 	= { "brightnessctl", "s", "+5%", NULL };
static const char *brighter[] = { "brightnessctl", "s", "5%-", NULL };


static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          {.v = firefoxcmd } },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          {.v = thunarcmd } },
	{ MODKEY|ShiftMask,             XK_g,      spawn,          {.v = steamcmd } },
	{ MODKEY|ShiftMask,             XK_k,      spawn,          {.v = vscodiumcmd } },
	{ MODKEY|ShiftMask,             XK_i,      spawn,          {.v = gimpcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ 0,                            XF86XK_AudioMute,            spawn,           {.v = volmute } },
	{ 0,                            XF86XK_AudioLowerVolume,     spawn,           {.v = voldown } },
	{ 0,                            XF86XK_AudioRaiseVolume,     spawn,           {.v = volup } },
	{ 0,                            XF86XK_MonBrightnessUp,      spawn,           {.v = brighter } },
	{ 0,                            XF86XK_MonBrightnessDown,    spawn,           {.v = dimmer } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

