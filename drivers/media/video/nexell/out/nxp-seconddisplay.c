#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/videodev2.h>
#include <linux/videodev2_nxp_media.h>

#include <media/v4l2-common.h>
#include <media/v4l2-dev.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-ctrls.h>

#include <mach/platform.h>
#include <mach/nxp-v4l2-platformdata.h>
#include <mach/soc.h>

#include <linux/switch.h>
#include "nxp-v4l2.h"
#include "nxp-seconddisplay.h"
extern int sscreen_type;
static int nxp_seconddisp_s_stream(struct v4l2_subdev *sd, int enable)
{
    printk("%s: %d\n", __func__, enable);
    nxp_soc_disp_device_enable_all(1, enable);
    return 0;
}

static const struct v4l2_subdev_video_ops nxp_seconddisp_video_ops = {
    .s_stream = nxp_seconddisp_s_stream,
};

static const struct v4l2_subdev_ops nxp_seconddisp_subdev_ops = {
    .video = &nxp_seconddisp_video_ops,
};

static int _init_entities(struct nxp_seconddisplay *me)
{
    int ret;
    struct v4l2_subdev *sd = &me->sd;
    struct media_pad *pad  = &me->pad;
    struct media_entity *entity = &sd->entity;

    v4l2_subdev_init(sd, &nxp_seconddisp_subdev_ops);

    strlcpy(sd->name, "NXP SECONDDISPLAY", sizeof(sd->name));
    v4l2_set_subdevdata(sd, me);
    sd->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;

    pad->flags = MEDIA_PAD_FL_SINK;
    ret = media_entity_init(entity, 1, pad, 0);
    if (ret < 0) {
        pr_err("%s: failed to media_entity_init()\n", __func__);
        return ret;
    }

    return 0;
}
static struct switch_dev seconddisplay_switch;
static ssize_t _enable_seconddisplay(struct device *pdev,
        struct device_attribute *attr, const char *buf, size_t n)
{
    if (!strncmp(buf, "1", 1)) {
        printk("%s: enable second display\n", __func__);
        if (!switch_get_state(&seconddisplay_switch)) {
            switch_set_state(&seconddisplay_switch, 1);
        }
    } else {
        printk("%s: disable second display\n", __func__);
        if (switch_get_state(&seconddisplay_switch)) {
            switch_set_state(&seconddisplay_switch, 0);
        }
    }
    return n;
}

static ssize_t _lcd_seconddisplay(struct device *d, struct device_attribute *attr, char *buf)
{
    if(sscreen_type == 1){
        return sprintf(buf, "lvds\n");
    }
    else if(sscreen_type == 2)
    {
        return sprintf(buf, "hdmi\n");
    }
    else if(sscreen_type == 3)
    {
        return sprintf(buf, "mipi\n");
    }
    else
    {
        return sprintf(buf, "lcd7\n");
    }
}

static struct device_attribute seconddisplay_attr = __ATTR(enable, 0666, NULL, _enable_seconddisplay);
static struct device_attribute seconddisplay_lcd_attr = __ATTR(lcd, 0666, _lcd_seconddisplay, NULL);
static struct attribute *attrs[] = {
    &seconddisplay_attr.attr,
    &seconddisplay_lcd_attr.attr,
    NULL,
};

static struct attribute_group attr_group = {
    .attrs = (struct attribute **)attrs,
};

static int _create_sysfs(void)
{
    struct kobject *kobj = NULL;
    int ret = 0;

    kobj = kobject_create_and_add("seconddisplay", &platform_bus.kobj);
    if (! kobj) {
        printk(KERN_ERR "Fail, create kobject for seconddisplay\n");
        return -ret;
    }

    ret = sysfs_create_group(kobj, &attr_group);
    if (ret) {
        printk(KERN_ERR "Fail, create sysfs group for seconddisplay\n");
        kobject_del(kobj);
        return -ret;
    }

    return 0;
}


struct nxp_seconddisplay *create_nxp_seconddisplay(void)
{
    int ret;
    struct nxp_seconddisplay *me;

    if (_create_sysfs())
        printk(KERN_ERR "%s: failed to create sysfs()\n", __func__);
    seconddisplay_switch.name = "seconddisplay";
    switch_dev_register(&seconddisplay_switch);
    switch_set_state(&seconddisplay_switch, 0);

    me = kzalloc(sizeof(*me), GFP_KERNEL);
    if (!me) {
        pr_err("%s: failed to alloc me!!!\n", __func__);
        return NULL;
    }

    ret = _init_entities(me);
    if (ret < 0) {
        pr_err("%s: failed to _init_entities()\n", __func__);
        kfree(me);
        return NULL;
    }

    me->module = 1;

    return me;
}

void release_nxp_seconddisplay(struct nxp_seconddisplay *me)
{
    kfree(me);
}

int register_nxp_seconddisplay(struct nxp_seconddisplay *me)
{
    int ret = v4l2_device_register_subdev(nxp_v4l2_get_v4l2_device(), &me->sd);
    if (ret < 0) {
        pr_err("%s: failed to v4l2_device_register_subdev()\n", __func__);
        return ret;
    }

    return 0;
}

void unregister_nxp_seconddisplay(struct nxp_seconddisplay *me)
{
    v4l2_device_unregister_subdev(&me->sd);
}

int suspend_nxp_seconddisplay(struct nxp_seconddisplay *me)
{
    int ret = 0;
    PM_DBGOUT("%s,me->module=%d\n", __func__, me->module);
    ret = nxp_soc_disp_device_suspend_all(me->module);
    return ret;
}

int resume_nxp_seconddisplay(struct nxp_seconddisplay *me)
{
    PM_DBGOUT("%s,me->module=%d\n", __func__, me->module);
    nxp_soc_disp_device_resume_all(me->module);
    return 0;
}
