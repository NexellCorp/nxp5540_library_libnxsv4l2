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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "nxs-v4l2.h"

int nxs_v4l2_querycap(int fd, struct v4l2_capability *cap)
{
	return ioctl(fd, VIDIOC_QUERYCAP, cap);
}

int nxs_v4l2_enum_format(int fd, struct v4l2_fmtdesc *desc)
{
	return ioctl(fd, VIDIOC_ENUM_FMT, desc);
}

/* TODO: pix_mp, num_planes, plane_fmt[] entry setting */
int nxs_v4l2_set_format(int fd, uint32_t buf_type, uint32_t w, uint32_t h,
			uint32_t f, uint32_t num_planes, uint32_t strides[],
			uint32_t sizes[])
{
	struct v4l2_format v4l2_fmt;
	int i;

	bzero(&v4l2_fmt, sizeof(v4l2_fmt));

	v4l2_fmt.type = buf_type;

	switch (buf_type) {
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
	case V4L2_BUF_TYPE_VIDEO_OUTPUT:
		v4l2_fmt.fmt.pix.width = w;
		v4l2_fmt.fmt.pix.height = h;
		v4l2_fmt.fmt.pix.pixelformat = f;
		v4l2_fmt.fmt.pix.bytesperline = strides[0];
		v4l2_fmt.fmt.pix.sizeimage = sizes[0];
		v4l2_fmt.fmt.pix.field = V4L2_FIELD_NONE;
		break;
	case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
	case V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:
		v4l2_fmt.fmt.pix_mp.width = w;
		v4l2_fmt.fmt.pix_mp.height = h;
		v4l2_fmt.fmt.pix_mp.pixelformat = f;
		v4l2_fmt.fmt.pix_mp.field = V4L2_FIELD_NONE;
		v4l2_fmt.fmt.pix_mp.num_planes = num_planes;
		for (i = 0; i < num_planes; i++) {
			struct v4l2_plane_pix_format *plane_fmt;

			plane_fmt = &v4l2_fmt.fmt.pix_mp.plane_fmt[i];
			plane_fmt->sizeimage = sizes[i];
			plane_fmt->bytesperline = strides[i];
		}
		break;
	default:
		fprintf(stderr, "not supported buf type: 0x%x\n", buf_type);
		return -EINVAL;
	}

	return ioctl(fd, VIDIOC_S_FMT, &v4l2_fmt);
}

int nxs_v4l2_get_format(int fd, uint32_t buf_type, uint32_t *w, uint32_t *h,
			uint32_t *f)
{
	struct v4l2_format v4l2_fmt;
	int ret;

	bzero(&v4l2_fmt, sizeof(v4l2_fmt));

	v4l2_fmt.type = buf_type;

	switch (buf_type) {
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
	case V4L2_BUF_TYPE_VIDEO_OUTPUT:
		v4l2_fmt.fmt.pix.field = V4L2_FIELD_NONE;
		break;
	case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
	case V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:
		v4l2_fmt.fmt.pix_mp.field = V4L2_FIELD_NONE;
		break;
	default:
		fprintf(stderr, "not supported buf type: 0x%x\n", buf_type);
		return -EINVAL;
	}

	ret = ioctl(fd, VIDIOC_G_FMT, &v4l2_fmt);
	if (ret)
		return ret;

	switch (buf_type) {
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
	case V4L2_BUF_TYPE_VIDEO_OUTPUT:
		*w = v4l2_fmt.fmt.pix.width;
		*h = v4l2_fmt.fmt.pix.height;
		*f = v4l2_fmt.fmt.pix.pixelformat;
		break;
	case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
	case V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:
		*w = v4l2_fmt.fmt.pix_mp.width;
		*h = v4l2_fmt.fmt.pix_mp.height;
		*f = v4l2_fmt.fmt.pix_mp.pixelformat;
		break;
	}

	return 0;
}

int nxs_v4l2_try_format(int fd, uint32_t buf_type, uint32_t w, uint32_t h,
			uint32_t f)
{
	struct v4l2_format v4l2_fmt;

	bzero(&v4l2_fmt, sizeof(v4l2_fmt));

	v4l2_fmt.type = buf_type;

	switch (buf_type) {
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
	case V4L2_BUF_TYPE_VIDEO_OUTPUT:
		v4l2_fmt.fmt.pix.width = w;
		v4l2_fmt.fmt.pix.height = h;
		v4l2_fmt.fmt.pix.pixelformat = f;
		v4l2_fmt.fmt.pix.field = V4L2_FIELD_NONE;
		break;
	case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
	case V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:
		v4l2_fmt.fmt.pix_mp.width = w;
		v4l2_fmt.fmt.pix_mp.height = h;
		v4l2_fmt.fmt.pix_mp.pixelformat = f;
		v4l2_fmt.fmt.pix_mp.field = V4L2_FIELD_NONE;
		break;
	default:
		fprintf(stderr, "not supported buf type: 0x%x\n", buf_type);
		return -EINVAL;
	}

	return ioctl(fd, VIDIOC_TRY_FMT, &v4l2_fmt);
}

int nxs_v4l2_reqbuf(int fd, uint32_t buf_type, uint32_t memory, int count)
{
	struct v4l2_requestbuffers req;

	bzero(&req, sizeof(req));
	req.type = buf_type;
	req.memory = memory;
	req.count = count;

	return ioctl(fd, VIDIOC_REQBUFS, &req);
}

int nxs_v4l2_querybuf(int fd, struct v4l2_buffer *buf)
{
	return ioctl(fd, VIDIOC_QUERYBUF, buf);
}

int nxs_v4l2_qbuf(int fd, struct v4l2_buffer *buf)
{
	return ioctl(fd, VIDIOC_QBUF, buf);
}

int nxs_v4l2_dqbuf(int fd, struct v4l2_buffer *buf)
{
	return ioctl(fd, VIDIOC_DQBUF, buf);
}

int nxs_v4l2_streamon(int fd, uint32_t buf_type)
{
	uint32_t type = buf_type;

	return ioctl(fd, VIDIOC_STREAMON, &type);
}

int nxs_v4l2_streamoff(int fd, uint32_t buf_type)
{
	uint32_t type = buf_type;

	return ioctl(fd, VIDIOC_STREAMOFF, &type);
}

int nxs_v4l2_get_parm(int fd, struct v4l2_streamparm *parm)
{
	return ioctl(fd, VIDIOC_G_PARM, parm);
}

int nxs_v4l2_set_parm(int fd, struct v4l2_streamparm *parm)
{
	return ioctl(fd, VIDIOC_S_PARM, parm);
}

int nxs_v4l2_get_ctrl(int fd, struct v4l2_control *ctrl)
{
	return ioctl(fd, VIDIOC_G_CTRL, ctrl);
}

int nxs_v4l2_set_ctrl(int fd, struct v4l2_control *ctrl)
{
	return ioctl(fd, VIDIOC_S_CTRL, ctrl);
}

int nxs_v4l2_cropcap(int fd, struct v4l2_cropcap *cap)
{
	return ioctl(fd, VIDIOC_CROPCAP, cap);
}

int nxs_v4l2_get_crop(int fd, uint32_t buf_type, uint32_t *x, uint32_t *y,
		      uint32_t *w, uint32_t *h)
{
	struct v4l2_crop crop;
	int ret;

	bzero(&crop, sizeof(crop));

	crop.type = buf_type;
	ret = ioctl(fd, VIDIOC_G_CROP, &crop);
	if (ret)
		return ret;

	*x = crop.c.left;
	*y = crop.c.top;
	*w = crop.c.width;
	*h = crop.c.height;

	return 0;
}

int nxs_v4l2_set_crop(int fd, uint32_t buf_type, uint32_t x, uint32_t y,
		      uint32_t w, uint32_t h)
{
	struct v4l2_crop crop;

	bzero(&crop, sizeof(crop));

	crop.type = buf_type;
	crop.c.left = x;
	crop.c.top = y;
	crop.c.width = w;
	crop.c.height = h;

	return ioctl(fd, VIDIOC_S_CROP, &crop);
}

int nxs_v4l2_get_ext_ctrls(int fd, struct v4l2_ext_controls *ctrls)
{
	return ioctl(fd, VIDIOC_G_EXT_CTRLS, ctrls);
}

int nxs_v4l2_set_ext_ctrls(int fd, struct v4l2_ext_controls *ctrls)
{
	return ioctl(fd, VIDIOC_S_EXT_CTRLS, ctrls);
}

int nxs_v4l2_get_selection(int fd, struct v4l2_selection *sel)
{
	return ioctl(fd, VIDIOC_G_SELECTION, sel);
}

int nxs_v4l2_set_selection(int fd, struct v4l2_selection *sel)
{
	return ioctl(fd, VIDIOC_S_SELECTION, sel);
}

int nxs_v4l2_ioctl(int fd, int cmd, void *arg)
{
	return ioctl(fd, cmd, arg);
}
