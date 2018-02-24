/*
 * Reed-Solomon encoder/decoder
 * Copyright 2004, Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */

#ifndef _RS_H_
#define _RS_H_

/* CCSDS standard (255,223) RS codec with conventional (*not* dual-basis)
 * symbol representation
 */
void encode_rs(unsigned char *data, unsigned char *parity, int pad);
int decode_rs(unsigned char *data, int *eras_pos, int no_eras, int pad);

/* CCSDS standard (255,223) RS codec with dual-basis symbol representation
 */
void encode_rs_ccsds(unsigned char *data, unsigned char *parity, int pad);
int decode_rs_ccsds(unsigned char *data, int *eras_pos, int no_eras, int pad);

#endif /* _RS_H_ */
