/*
 * Minimal FIFO implementation
 * by warmonkey (luoshumymail@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _MFIFO_H
#define _MFIFO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct __mfifo {
	uint8_t *buf;
	size_t mask;
	size_t wpos;
	size_t rpos;
};

#define DECLARE_MFIFO(name)	struct __mfifo name;
#define DEFINE_MFIFO(NAME,SZ)	uint8_t __##NAME##_alloc[SZ];   \
struct __mfifo NAME = {__##NAME##_alloc,(SZ)-1,0,0};

static inline void mfifo_reset(struct __mfifo *f)//discard all data
{f->rpos = f->wpos = 0;}

static inline void mfifo_skip(struct __mfifo *f)//skip data
{f->rpos = f->wpos;}

static inline size_t mfifo_len(struct __mfifo *f)//used elements
{return (f->wpos - f->rpos) & f->mask; }

static inline size_t mfifo_size(struct __mfifo *f)
{return f->mask+1;}

static inline size_t mfifo_avail(struct __mfifo *f)//empty elements
{return mfifo_size(f)-mfifo_len(f);}

static inline uint8_t* mfifo_in_ptr(struct __mfifo *f)
{return f->buf+f->wpos;}

static inline void mfifo_move_in_ptr(struct __mfifo *f,size_t offset)
{f->wpos=(f->wpos+offset)&f->mask;}

static inline size_t mfifo_put(struct __mfifo *f,uint8_t data)
{
	if(mfifo_avail(f)>=1) {
		*mfifo_in_ptr(f)=data;
		mfifo_move_in_ptr(f,1);
		return 1;
 	}
	else return 0;
}

static inline size_t mfifo_in(struct __mfifo *f,uint8_t *buf,size_t n)
{
	size_t count = 0;
	while(n--) count+=mfifo_put(f, *buf++);
	return count;
}

static inline uint8_t* mfifo_out_ptr(struct __mfifo *f)
{return f->buf+f->rpos;}

static inline void mfifo_move_out_ptr(struct __mfifo *f,size_t offset)
{f->rpos=(f->rpos+offset)&f->mask;}

static inline uint8_t mfifo_peek(struct __mfifo *f)
{return *mfifo_out_ptr(f);}

static inline size_t mfifo_get(struct __mfifo *f,uint8_t *data)
{
	if(mfifo_len(f)>=1) {
		*data=*mfifo_out_ptr(f);
		mfifo_move_out_ptr(f,1);
		return 1;
 	}
	else return 0;
}

//inline size_t mfifo_out_peek(struct __mfifo *f,uint8_t *buf,size_t n);
static inline size_t mfifo_out(struct __mfifo *f,void* buf,size_t n)
{
	size_t count = 0;
	uint8_t *ptr = (uint8_t*)buf;
	while(n--)
	{
		count += mfifo_get(f, ptr);
		ptr++;
	}
	return count;
}

static inline size_t mfifo_oput(struct __mfifo *f, uint8_t data)
{
	if(mfifo_avail(f)<1) mfifo_move_out_ptr(f, 1);
	*mfifo_in_ptr(f)=data;
	mfifo_move_in_ptr(f, 1);
	return 1;
}

static inline size_t mfifo_overwrite(struct __mfifo *f,void* buf,size_t n)
{
	size_t count = 0;
	uint8_t *ptr = (uint8_t*)buf;
	while(n--) count+=mfifo_oput(f, *ptr++);
	return count;
}

static inline size_t mfifo_block_len(struct __mfifo *f)
{
	if(f->wpos >= f->rpos) return (f->wpos - f->rpos);
	else return (f->mask + 1 - f->rpos);
}

#ifdef __cplusplus
}
#endif
#endif
