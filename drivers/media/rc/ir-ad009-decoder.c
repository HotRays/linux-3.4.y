/* ir-ad009-decoder.c - handle AD009 IR Pulse/Space protocol
 *
 * Copyright (C) 2010 by Mauro Carvalho Chehab <mchehab@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include <linux/bitrev.h>
#include <linux/module.h>
#include "rc-core-priv.h"
#include <linux/delay.h>
static int count,bit_count,flag,start_flag;
/**
 * ir_ad009_decode() - Decode one AD009 pulse or space
 * @dev:	the struct rc_dev descriptor of the device
 * @duration:	the struct ir_raw_event descriptor of the pulse/space
 *
 * This function returns -EINVAL if the pulse violates the state machine
 */

static int ir_ad009_decode(struct rc_dev *dev, struct ir_raw_event ev)
{
	struct nec_dec *data = &dev->raw->nec;
	u32 scancode;
//	printk("ev.pulse = %d , ev.duration = %d\n",ev.pulse,ev.duration);
	if ((!(dev->raw->enabled_protocols & RC_TYPE_AD009)))
		return 0;
	if(ev.duration >10000000)
	{
		count=0;
		start_flag=0;
		data->bits=0;
		bit_count=0;
		flag=0;
		
		return 0;
	}
	else if(count >= 39 && ev.duration > 2600000 && ev.duration < 2900000)
	{	
			count=0;
			start_flag=0;
			data->bits=0;
			bit_count=0;
			flag=0;
			return 0;
	}
	else if(count < 39)
	{
		if(ev.duration > 2600000 && ev.duration < 2900000)
		{
			start_flag=1;
			data->bits=0;
			bit_count=0;
			flag=0;
			count=2;
		}
		if(start_flag==1)
			count++;
			return 0;
	}
	switch(ev.pulse)
	{
		case true:
			if(ev.duration < 500000)
			{
				if(flag == 0)
				{
					data->bits |= 1;
					data->bits <<= 1;
					bit_count++;
					flag = 1;
				}
				else
				{
					flag = 0;
					break;
				}
			}
			else if(ev.duration > 790000 && ev.duration < 1000000)
			{
				data->bits |= 1;
				data->bits <<= 1;
				bit_count++;
				flag = 1;
			}
			break;
		case false:
			if(ev.duration < 500000)
			{
				if(flag == 0)
				{
					data->bits &= ~1;
					data->bits <<= 1;
					bit_count++;
					flag = 1;
				}
				else
				{
					flag = 0;
					break;
				}
			}
			else if(ev.duration > 790000 && ev.duration < 1000000)
			{
				data->bits &= ~1;
				data->bits <<= 1;
				bit_count++;
				flag = 1;
			}
			break;
	}
	if(bit_count==17)
	{
		scancode=data->bits & 0x1ff;
//		printk("***************scancode = %d ***************\n",scancode);
		if(scancode == 320)
		{
			input_report_key(dev->input_dev,KEY_LEFTSHIFT,1);			
			input_sync(dev->input_dev);
			input_report_key(dev->input_dev,KEY_3,1);
			input_sync(dev->input_dev);
			input_report_key(dev->input_dev,KEY_3,0);
			input_sync(dev->input_dev);
			input_report_key(dev->input_dev,KEY_LEFTSHIFT,0);
			input_sync(dev->input_dev);
		}
		else	
			rc_keydown(dev,scancode,0);
		data->bits=0;
		bit_count=0;
		count=0;
		flag=0;
		start_flag=0;
	}
	count++;
	return 0;
}

static struct ir_raw_handler ad009_handler = {
	.protocols	= RC_TYPE_AD009,
	.decode		= ir_ad009_decode,
};

static int __init ir_ad009_decode_init(void)
{
	ir_raw_handler_register(&ad009_handler);
	printk(KERN_INFO "IR AD009 protocol handler initialized\n");
	return 0;
}

static void __exit ir_ad009_decode_exit(void)
{
	ir_raw_handler_unregister(&ad009_handler);
}

module_init(ir_ad009_decode_init);
module_exit(ir_ad009_decode_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
MODULE_AUTHOR("Red Hat Inc. (http://www.redhat.com)");
MODULE_DESCRIPTION("AD009 IR protocol decoder");
