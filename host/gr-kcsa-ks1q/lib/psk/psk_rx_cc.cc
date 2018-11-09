#include "psk_rx_cc.h"
#include "hb_taps.h"
#include <cmath>
//volk_32fc_32f_dot_prod_32fc_a


#define sign(X) ((X) < 0 ? -1.0 : 1.0)
#define expj(X) (gr_complex(cosf(X), sinf(X)))
#define saturate(X, MIN, MAX) ((X) > (MAX) ? (MAX) : ((X) < (MIN) ? (MIN) : (X)))

void ctrl_loop_set_rate(struct ctrl_loop_s *c, float rate, float dev)
{
	c->rate = rate;
	c->rate_min = rate - dev;
	c->rate_max = rate + dev;
}

void ctrl_loop_set_bw(struct ctrl_loop_s *c, float bw, float damp)
{
    float denom = (1.0 + 2.0*damp*bw + bw*bw);
	c->kp = (4*damp*bw) / denom;
	c->ki = (4*bw*bw) / denom;
}

//adj = ctrl_loop_work(&c, err);
float ctrl_loop_work(struct ctrl_loop_s *c, float err)
{
    c->rate = saturate(c->rate + c->ki * err, c->rate_min, c->rate_max);
	float adj = c->rate + c->kp * err;
	return adj;
}

psk_rx_cc::psk_rx_cc(
	enum mod_type _mod,
	int sps,	//must be even
	float sym_rate,
	float sym_rate_dev,
	float sym_bw,
	float carr_rate,
	float carr_rate_dev,
	float carr_bw
    ) : dbgfile(NULL)
{
    mod = _mod;

	sym_phase = 0;
	ctrl_loop_set_rate(&sym,  sym_rate,  sym_rate_dev);
	ctrl_loop_set_bw(&sym, sym_bw, 1.0);
	sym_adj = sym_rate;

	carr_phase = 0;
	ctrl_loop_set_rate(&carr, carr_rate, carr_rate_dev);
	ctrl_loop_set_bw(&carr, carr_bw, 1.0);
	carr_adj = carr_rate;

    //hb_state = new gr_complex[hb_ntaps];
    hb_state.resize(hb_ntaps, 0.0);
    hb_pos = hb_ntaps - 1;

    mf_ntaps = sps;
    //mf_taps = new float[mf_ntaps];
    //mf_state = new gr_complex[mf_ntaps];
    mf_taps.resize(mf_ntaps);
    mf_state.resize(mf_ntaps, 0.0);
    for(int i = 0; i < sps; i++) {
		mf_taps[i] = 1.0 / sps;
	}
    for(unsigned int i = sps; i < mf_ntaps; i++) {
		mf_taps[i] = 0.0;
	}
    mf_pos = mf_ntaps - 1;

	dd = 0;
	out_decim = sps / 2;
	gout = false;
	y1_1 = 0.0;
	y1_2 = 0.0;

    dbgfile = fopen("dbg.bin", "wb");
}

psk_rx_cc::~psk_rx_cc() {
    //delete [] hb_state;
    //delete [] mf_state;
    //delete [] mf_taps;
    if(dbgfile)
        fclose(dbgfile);
}

float psk_rx_cc::sym_rate()
{
    return sym.rate;
}

float psk_rx_cc::carr_rate()
{
    return carr.rate;
}

void psk_rx_cc::set_eye_diagram(bool en)
{
    if(en) {
        eye_buffer.resize(out_decim*2*8);
        eye_pos = -1;
    } else {
        eye_buffer.clear();
    }
}

std::vector<gr_complex> psk_rx_cc::eye_diagram()
{
    std::vector<gr_complex> y1;
    for(unsigned int i = 0; i < eye_buffer.size() - mf_ntaps; i++)
    {
        gr_complex yy1 = 0.0;
        for(unsigned int j = 0; j < mf_ntaps; j++) {
            yy1 += eye_buffer[i + j] * mf_taps[mf_ntaps-1-j];
        }
        y1.push_back(yy1);
    }
    return y1;
}

void psk_rx_cc::work(const gr_complex *in, int insize, int *consumed, gr_complex *out, int outsize, int *produced)
{
    if(!in || !consumed || !out || !produced)
        return;

	int ii = 0, oo = 0;
	while(1) {
		if(sym_phase < hb_nsteps) {
			int idx = floor(sym_phase);
            //printf("sym_phase: %f, idx: %d\n", sym_phase, idx);
			// interpolator
            gr_complex y = 0.0;
            for(unsigned int i = 0; i < hb_ntaps; i++) {
                y += hb_taps[idx][i] * hb_state[(hb_pos - i) % hb_ntaps];
			}
			sym_phase += sym_adj * hb_nsteps;
            // derotate
            gr_complex yy = y * expj(-carr_phase);
            carr_phase += carr_adj;
            // matched filter
            mf_pos = (mf_pos + 1 ) % mf_ntaps;
            mf_state[mf_pos] = yy;
            // eye diagram
            if(eye_buffer.size() > 0) {
                if(eye_pos >= 0) {
                    eye_buffer[eye_pos++] = yy;
                    if(eye_pos >= (int)eye_buffer.size()) {
                        eye_pos = -1;
                    }
                }
            }
			// decimate
			dd++;
			if(dd >= out_decim) {
                dd = 0;

                gr_complex y1 = 0.0;
                for(unsigned int i = 0; i < mf_ntaps; i++) {
                    y1 += mf_taps[i] * mf_state[(mf_pos - i) % mf_ntaps];
                }
                //printf("y1: %f, %f\n", real(y1), imag(y1));

				if(gout) {
					//symbol err
                    float sym_err = (real(y1 - y1_2)) * real(y1_1) + (imag(y1 - y1_2)) * imag(y1_1);
					//carrier err
					float carr_err;
					switch (mod) {
						case BPSK:
							carr_err = sign(real(y1_1)) * imag(y1_1);
							//carr_err = real(y1_1) * imag(y1_1);
							break;
						case QPSK:
							carr_err = sign(real(y1_1)) * imag(y1_1) - sign(imag(y1_1)) * real(y1_1);
							break;
						default: carr_err = 0.0;
					}
					//write output
                	out[oo++] = y1_1;
                    //eye diagram sync
                    if(eye_buffer.size() > 0) {
                        if(eye_pos < 0) eye_pos = 0;
                    }
					//loop filter
					sym_adj = ctrl_loop_work(&sym, sym_err);
					carr_adj = ctrl_loop_work(&carr, carr_err);
                    //debug output
                    if(dbgfile)
                    {
                        float _v1 = sym_rate();
                        float _v2 = carr_rate();
                        fwrite(&_v1, sizeof(float), 1, dbgfile);
                        fwrite(&_v2, sizeof(float), 1, dbgfile);
                    }
				}
                gout = !gout;
                y1_2 = y1_1;
                y1_1 = y1;
				if(oo >= outsize) {
					//exit here
					*consumed = ii;
					*produced = oo;
					return;
				}
            } //decimate
		} else {
            hb_pos = (hb_pos + 1 ) % hb_ntaps;
            hb_state[hb_pos] = in[ii++];
			sym_phase -= hb_nsteps;
			if(ii >= insize) {
				//exit here
				*consumed = ii;
				*produced = oo;
				return;
			}
		}
	}
}
