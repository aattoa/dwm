/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Liberation Mono:size=11" };
static const char dmenufont[]       = "Liberation Mono:size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6" };

static const Rule rules[] = {
    /* xprop(1):
     *    WM_CLASS(STRING) = instance, class
     *    WM_NAME(STRING) = title
     */
    /* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
    { "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
    { "st",      NULL,     NULL,           0,         0,          1,           0,        -1 },
    { NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "grid.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
    { "HHH",      grid },
};

/* key definitions */
#define MODKEY Mod1Mask
#define WINKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static const Key keys[] = {
    /* modifier           key                       function           argument */
    { MODKEY,             XK_p,                     spawn,             {.v = dmenucmd} },
    { MODKEY|ShiftMask,   XK_Return,                spawn,             {.v = termcmd} },
    { MODKEY,             XK_b,                     togglebar,         {0} },
    { MODKEY|ShiftMask,   XK_j,                     rotatestack,       {.i = +1} },
    { MODKEY|ShiftMask,   XK_k,                     rotatestack,       {.i = -1} },
    { MODKEY,             XK_j,                     focusstack,        {.i = +1} },
    { MODKEY,             XK_k,                     focusstack,        {.i = -1} },
    { MODKEY,             XK_i,                     incnmaster,        {.i = +1} },
    { MODKEY,             XK_d,                     incnmaster,        {.i = -1} },
    { MODKEY,             XK_h,                     setmfact,          {.f = -0.025} },
    { MODKEY,             XK_l,                     setmfact,          {.f = +0.025} },
    { MODKEY|ShiftMask,   XK_h,                     setcfact,          {.f = +0.1} },
    { MODKEY|ShiftMask,   XK_l,                     setcfact,          {.f = -0.1} },
    { MODKEY|ShiftMask,   XK_o,                     setcfact,          {.f =  0.00} },
    { MODKEY,             XK_Return,                zoom,              {0} },
    { MODKEY,             XK_Tab,                   view,              {0} },
    { MODKEY|ShiftMask,   XK_c,                     killclient,        {0} },
    { MODKEY,             XK_t,                     setlayout,         {.v = &layouts[0]} }, // tile
    { MODKEY,             XK_f,                     setlayout,         {.v = &layouts[1]} }, // float
    { MODKEY,             XK_m,                     setlayout,         {.v = &layouts[2]} }, // monocle
    { MODKEY,             XK_g,                     setlayout,         {.v = &layouts[3]} }, // grid
    { MODKEY|ShiftMask,   XK_space,                 togglefloating,    {0} },
    { MODKEY,             XK_0,                     view,              {.ui = ~0} },
    { MODKEY|ShiftMask,   XK_0,                     tag,               {.ui = ~0} },
    { MODKEY,             XK_comma,                 focusmon,          {.i = -1} },
    { MODKEY,             XK_period,                focusmon,          {.i = +1} },
    { MODKEY|ShiftMask,   XK_comma,                 tagmon,            {.i = -1} },
    { MODKEY|ShiftMask,   XK_period,                tagmon,            {.i = +1} },
    TAGKEYS(              XK_1,                                        0)
    TAGKEYS(              XK_2,                                        1)
    TAGKEYS(              XK_3,                                        2)
    TAGKEYS(              XK_4,                                        3)
    TAGKEYS(              XK_5,                                        4)
    TAGKEYS(              XK_6,                                        5)
    { MODKEY|ShiftMask,   XK_q,                     quit,              {0} },

#ifdef SCRIPT
#error "conflict"
#endif
#define SCRIPT(cmd) SHCMD("$MY_DOTFILES_REPO/scripts/" cmd)
    { WINKEY,             XK_w,                     spawn,             SCRIPT("bookmark-add") },
    { WINKEY,             XK_e,                     spawn,             SCRIPT("bookmark-open") },
    { WINKEY,             XK_r,                     spawn,             SCRIPT("trigger-compilation") },
    { WINKEY,             XK_Menu,                  spawn,             SCRIPT("rotate-keyboard-layout") },
    { WINKEY,             XK_Escape,                spawn,             SCRIPT("swap-caps-and-escape") },
    { WINKEY,             XK_Return,                spawn,             SCRIPT("configure-mouse-and-keyboard toggle") },
    { WINKEY,             XK_n,                     spawn,             SCRIPT("show-news") },
    { WINKEY|ShiftMask,   XK_j,                     spawn,             SCRIPT("media-control next") },
    { WINKEY|ShiftMask,   XK_k,                     spawn,             SCRIPT("media-control previous") },
    { 0,                  XF86XK_AudioNext,         spawn,             SCRIPT("media-control next") },
    { 0,                  XF86XK_AudioPrev,         spawn,             SCRIPT("media-control previous") },
    { 0,                  XF86XK_AudioPlay,         spawn,             SCRIPT("media-control play") },
    { 0,                  XF86XK_AudioPause,        spawn,             SCRIPT("media-control pause") },
    { 0,                  XF86XK_AudioForward,      spawn,             SCRIPT("media-control forward") },
    { 0,                  XF86XK_AudioRewind,       spawn,             SCRIPT("media-control rewind") },
    { WINKEY,             XK_F6,                    spawn,             SCRIPT("toggle-headphones") },
    { 0,                  XF86XK_AudioMute,         spawn,             SCRIPT("volume-control toggle") },
    { WINKEY,             XK_F5,                    spawn,             SCRIPT("volume-control toggle") },
    { 0,                  XF86XK_AudioRaiseVolume,  spawn,             SCRIPT("volume-control up") },
    { WINKEY,             XK_F8,                    spawn,             SCRIPT("volume-control up") },
    { 0,                  XF86XK_AudioLowerVolume,  spawn,             SCRIPT("volume-control down") },
    { WINKEY,             XK_F7,                    spawn,             SCRIPT("volume-control down") },
    { 0,                  XF86XK_MonBrightnessUp,   spawn,             SCRIPT("change-brightness up") },
    { 0,                  XF86XK_MonBrightnessDown, spawn,             SCRIPT("change-brightness down") },
    { WINKEY,             XK_s,                     spawn,             SCRIPT("screenshot") },
    { WINKEY|MODKEY,      XK_s,                     spawn,             SCRIPT("screenshot gui") },
    { WINKEY,             XK_b,                     spawn,             SHCMD("$BROWSER") },
    { WINKEY,             XK_g,                     spawn,             SHCMD("/usr/bin/steam") },
    { WINKEY,             XK_l,                     spawn,             SHCMD("/usr/bin/slock") },
    { WINKEY,             XK_o,                     spawn,             SHCMD("/usr/bin/libreoffice") },
#undef SCRIPT
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask            button          function        argument */
    { ClkClientWin,         MODKEY|ShiftMask,     Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY|ShiftMask,     Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY|ShiftMask,     Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,                    Button1,        view,           {0} },
    { ClkTagBar,            0,                    Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,               Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,               Button3,        toggletag,      {0} },
};

