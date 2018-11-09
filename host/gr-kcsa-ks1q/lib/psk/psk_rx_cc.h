#pragma once

#include <cstdio>
#include <gnuradio/gr_complex.h>
#include <vector>
//volk_32fc_32f_dot_prod_32fc_a

struct ctrl_loop_s {
    float rate;
    float rate_min;
    float rate_max;
    float kp;
    float ki;
};

void ctrl_loop_set_rate(struct ctrl_loop_s *c, float rate, float dev);
void ctrl_loop_set_bw(struct ctrl_loop_s *c, float bw, float damp);

//adj = ctrl_loop_work(&c, err);
float ctrl_loop_work(struct ctrl_loop_s *c, float err);

class psk_rx_cc {

public:
    enum mod_type {
        BPSK = 0,
        QPSK = 1,
        _8PSK = 2,
    } mod;

private:
    //gr_complex *hb_state;
    std::vector<gr_complex> hb_state;
    unsigned int hb_pos;

    unsigned int mf_ntaps;
    std::vector<float> mf_taps;
    std::vector<gr_complex> mf_state;
    //float *mf_taps;
    //gr_complex *mf_state;
    unsigned int mf_pos;

    struct ctrl_loop_s sym, carr;
    float sym_phase, carr_phase;
    float sym_adj, carr_adj;

    int dd;
    int out_decim;
    gr_complex y1_1, y1_2;
    bool gout;

    bool eye_diag;
    std::vector<gr_complex> eye_buffer;
    int eye_pos;// -1 = sync, 0~(len-1) capture

    FILE *dbgfile;

public:
    psk_rx_cc(enum mod_type _mod,
           int sps,	//must be even
           float sym_rate,
           float sym_rate_dev,
           float sym_bw,
           float carr_rate,
           float carr_rate_dev,
           float carr_bw
           );

    ~psk_rx_cc();

    void work(const gr_complex *in, int insize, int *consumed,
              gr_complex *out, int outsize, int *produced);

    float sym_rate();
    float carr_rate();
    void set_eye_diagram(bool en = true);
    std::vector<gr_complex> eye_diagram();

};
