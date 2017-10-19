#ifndef _NXP_SECONDDISPLAY_H
#define _NXP_SECONDDISPLAY_H

struct media_pad;
struct v4l2_subdev;

struct nxp_seconddisplay {
    int module;

    struct v4l2_subdev sd;
    struct media_pad pad;
};

/**
 * publi api
 */
struct nxp_seconddisplay *create_nxp_seconddisplay(void);
void release_nxp_seconddisplay(struct nxp_seconddisplay *);
int register_nxp_seconddisplay(struct nxp_seconddisplay *);
void unregister_nxp_seconddisplay(struct nxp_seconddisplay *);
int suspend_nxp_seconddisplay(struct nxp_seconddisplay *);
int resume_nxp_seconddisplay(struct nxp_seconddisplay *);

#endif
