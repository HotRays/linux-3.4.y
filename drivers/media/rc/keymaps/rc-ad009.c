/* nec-terratec-cinergy-xs.h - Keytable for nec_terratec_cinergy_xs Remote Controller
 *
 * keymap imported from ir-keymaps.c
 *
 * Copyright (c) 2010 by Mauro Carvalho Chehab <mchehab@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <media/rc-map.h>
#include <linux/module.h>

/* Terratec Cinergy Hybrid T USB XS FM
   Mauro Carvalho Chehab <mchehab@redhat.com>
 */

static struct rc_map_table ad009_xs[] = {
	/* Nexell key map*/
	{ 262, KEY_BACK},
	{ 20,KEY_ESC},
	{ 24, KEY_POWER},
	{ 2, KEY_1},
	{ 4, KEY_2},
	{ 6, KEY_3},
	{ 8, KEY_4},
	{ 10, KEY_5},
	{ 12, KEY_6},
	{ 14, KEY_7},
	{ 16, KEY_8},
	{ 18, KEY_9},
	{ 0, KEY_0},

	{ 260, KEY_HOME},
	{ 168, KEY_MENU},

	{ 176, KEY_UP},
	{ 180, KEY_LEFT},
	{ 184, KEY_ENTER},
	{ 182, KEY_RIGHT},
	{ 178, KEY_DOWN},

	{ 32, KEY_VOLUMEUP},
	{ 34, KEY_VOLUMEDOWN},
	{ 26, KEY_MUTE},
//	{ 218, KEY_SUBTITLE},
//	{ 110, KEY_CAMERA},		/* PIC */
	{ 30, KEY_INFO},
	{ 64, KEY_CHANNELUP},
	{ 66, KEY_CHANNELDOWN},
	{ 218, KEY_RED},
	{ 220, KEY_GREEN},
	{ 222, KEY_YELLOW},
	{ 224, KEY_BLUE},
	{ 98, KEY_STOP},
	{ 318, KEY_KPASTERISK},
	{ 280, KEY_A},
	{ 282, KEY_B},
	{ 284, KEY_C},
	{ 286, KEY_D},
	{ 490, KEY_LIST},
	{ 110, KEY_RECORD},
};

static struct rc_map_list ad009_xs_map = {
	.map = {
		.scan    = ad009_xs,
		.size    = ARRAY_SIZE(ad009_xs),
		.rc_type = RC_TYPE_AD009,
		.name    = RC_MAP_AD009,
	}
};

static int __init init_rc_map_ad009(void)
{
	return rc_map_register(&ad009_xs_map);
}

static void __exit exit_rc_map_ad009(void)
{
	rc_map_unregister(&ad009_xs_map);
}

module_init(init_rc_map_ad009)
module_exit(exit_rc_map_ad009)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
