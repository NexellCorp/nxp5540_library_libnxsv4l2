/*
 * Copyright (C) 2016  Nexell Co., Ltd.
 * Author: Sungwoo, Park <swpark@nexell.co.kr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LIB_NXS_V4L2_H__
#define __LIB_NXS_V4L2_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/videodev2.h>
#include <linux/v4l2-subdev.h>
#include "nxs_v4l2.h"

int nxs_v4l2_querycap(int fd, struct v4l2_capability *cap);
int nxs_v4l2_enum_format(int fd, struct v4l2_fmtdesc *desc);
int nxs_v4l2_set_format(int fd, uint32_t buf_type, uint32_t w, uint32_t h,
			uint32_t f, uint32_t num_planes, uint32_t strides[],
			uint32_t sizes[]);
int nxs_v4l2_get_format(int fd, uint32_t buf_type, uint32_t *w, uint32_t *h,
			uint32_t *f);
int nxs_v4l2_try_format(int fd, uint32_t buf_type, uint32_t w, uint32_t h,
			uint32_t f);
int nxs_v4l2_reqbuf(int fd, uint32_t buf_type, uint32_t memory, int count);
int nxs_v4l2_querybuf(int fd, struct v4l2_buffer *buf);
int nxs_v4l2_qbuf(int fd, struct v4l2_buffer *buf);
int nxs_v4l2_dqbuf(int fd, struct v4l2_buffer *buf);
int nxs_v4l2_streamon(int fd, uint32_t buf_type);
int nxs_v4l2_streamoff(int fd, uint32_t buf_type);
int nxs_v4l2_get_parm(int fd, struct v4l2_streamparm *parm);
int nxs_v4l2_set_parm(int fd, struct v4l2_streamparm *parm);
int nxs_v4l2_get_ctrl(int fd, struct v4l2_control *ctrl);
int nxs_v4l2_set_ctrl(int fd, struct v4l2_control *ctrl);
int nxs_v4l2_cropcap(int fd, struct v4l2_cropcap *cap);
int nxs_v4l2_get_crop(int fd, uint32_t buf_type, uint32_t *x, uint32_t *y,
		      uint32_t *w, uint32_t *h);
int nxs_v4l2_set_crop(int fd, uint32_t buf_type, uint32_t x, uint32_t y,
		      uint32_t w, uint32_t h);
int nxs_v4l2_get_ext_ctrls(int fd, struct v4l2_ext_controls *ctrls);
int nxs_v4l2_set_ext_ctrls(int fd, struct v4l2_ext_controls *ctrls);
int nxs_v4l2_get_selection(int fd, struct v4l2_selection *sel);
int nxs_v4l2_set_selection(int fd, struct v4l2_selection *sel);
int nxs_v4l2_ioctl(int fd, int cmd, void *arg);

/* subdev */
int nxs_v4l2_subdev_set_format(int fd, uint32_t w, uint32_t h, uint32_t code,
			       uint32_t field);
int nxs_v4l2_subdev_get_format(int fd, uint32_t *w, uint32_t *h, uint32_t *code,
			       uint32_t *field);
int nxs_v4l2_subdev_try_format(int fd, uint32_t w, uint32_t h, uint32_t code,
			       uint32_t field);
int nxs_v4l2_subdev_set_dstformat(int fd, uint32_t w, uint32_t h, uint32_t code,
				  uint32_t field);
int nxs_v4l2_subdev_get_dstformat(int fd, uint32_t *w, uint32_t *h,
				  uint32_t *code, uint32_t *field);
int nxs_v4l2_subdev_try_dstformat(int fd, uint32_t w, uint32_t h, uint32_t code,
				  uint32_t field);
int nxs_v4l2_subdev_set_crop(int fd, uint32_t x, uint32_t y, uint32_t w,
			     uint32_t h);
int nxs_v4l2_subdev_get_crop(int fd, uint32_t *x, uint32_t *y, uint32_t *w,
			     uint32_t *h);
int nxs_v4l2_subdev_try_crop(int fd, uint32_t x, uint32_t y, uint32_t w,
			     uint32_t h);
int nxs_v4l2_subdev_start(int fd);
int nxs_v4l2_subdev_stop(int fd);
#ifdef __cplusplus
}
#endif

#endif
