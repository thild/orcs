#include <stdint.h>
#include <math.h>       /* log */

class ctp_t { 

    public:
        uint16_t *counters = NULL;   // conditional probability table
        
        void allocate(uint8_t num_evidences /*GBH width*/) {
            this->counters = new uint16_t[num_evidences];
            
        }

        ctp_t() {
            
        }


        ~ctp_t() {
            if (this->counters) delete [] counters;
        }      
};

class ctp_ltu_t { 

    public:

        ctp_t *ltu = NULL;   // conditional probability table
        uint8_t num_evidences = 0;

        ctp_ltu_t(uint8_t ltus /*BTB depth*/, uint8_t num_evidences /*GBH width*/) {
            this->num_evidences = num_evidences;
            auto numlines = (uint32_t)pow (2, ltus);
            this->ltu = new ctp_t[numlines];
            
            for(size_t i = 0; i < numlines; i++)
            {
                this->ltu[i].allocate(num_evidences);
            }
            
        }

        ~ctp_ltu_t() {
            if (this->ltu) delete [] ltu;
        }        

};



class nbbp_predictor_t: public branch_predictor_t { 
    private: 
        uint8_t extract(uint32_t index, uint8_t p) {
            return this->ctp_ltu->ltu[index].counters[p] & 0xF << p;
        }

    protected:
        ctp_ltu_t *ctp_ltu;
        uint8_t *py_ltu;

    public: 

        uint8_t btb_depth = 0;
        uint8_t gbh_width = 0;

        bool predict(uint32_t index, uint32_t gbh) {
            auto ctp = &this->ctp_ltu->ltu[index];
            auto py = &this->py_ltu[index];
            uint8_t p0 = (*py & 0xF0) >> 4;
            uint8_t p1 = *py & 0x0F;            
            auto evidences = gbh;
            uint32_t mask = 0x1;
            uint64_t y0 = (uint64_t)log10(p0), y1 = (uint64_t)log10(p1);
            for(size_t i = 0; i < this->gbh_width; i++)
            {
                auto counter = &ctp->counters[i];
                auto evidence = (uint8_t)((evidences & mask) == 1);
                uint8_t px1y1 = (*counter & 0xF);
                uint8_t px1y0 = (*counter & 0xF0) >> 4;
                uint8_t px0y1 = (*counter & 0xF00) >> 8;
                uint8_t px0y0 = (*counter & 0xF000) >> 12;

                if(evidence == 0) {
                    y0 += (uint64_t)log10(px0y0);
                    y1 += (uint64_t)log10(px0y1);
                }
                else {
                    y0 += (uint64_t)logf(px1y0);
                    y1 += (uint64_t)logf(px1y1);
                }
            }
            return (uint64_t)log(y0) < (uint64_t)logf(y1);
        }


        void update(uint32_t index, uint32_t gbh,  bool taken)  {
            auto ctp = &this->ctp_ltu->ltu[index];
            auto py = &this->py_ltu[index];
            
            auto evidences = gbh;
            uint32_t mask = 0x1;
            for(size_t i = 0; i < this->gbh_width; i++)
            {
                auto counter = &ctp->counters[i];
                auto evidence = (uint8_t)((evidences & mask) == 1);
                uint8_t px1y1 = (*counter & 0xF);
                uint8_t px1y0 = (*counter & 0xF0) >> 4;
                uint8_t px0y1 = (*counter & 0xF00) >> 8;
                uint8_t px0y0 = (*counter & 0xF000) >> 12;
                *counter = 0;

                if(taken) {
                    if(evidence == 0) {
                        if(px0y0 != 15) {
                            px0y0++;
                            px1y0 = (~px0y0) & 0x0F;
                        }
                    }
                    else {
                        if(px1y0 != 15) {
                            px1y0++;
                            px0y0 = (~px1y0) & 0x0F;
                        }
                    }
                }
                else {
                    if(evidence == 0) {
                        if(px0y1 != 15) {
                            px0y1++;
                            px1y1 = (~px0y1) & 0x0F;
                        }
                    }
                    else {
                        if(px1y1 != 15) {
                            px1y1++;
                            px0y1 = (~px1y1) & 0x0F;
                        }
                    }
                }
                *counter = (px0y0 << 12) | (px1y0 << 8) | (px0y1 << 4) | (px1y1);
                mask <<= 1;
            }            

            uint8_t p0 = (*py & 0xF0) >> 4;
            uint8_t p1 = *py & 0x0F;
            *py = 0x0;
            if(taken) {
                if(p0 != 15) {
                    p0++;
                    p1 = (~p0) & 0x0F;
                    *py |= p0 << 4;
                    *py |= p1;
                }
            }
            else {
                if(p1 != 0) {
                    p1--;
                    p0 = (~p1) & 0x0F;
                    *py |= p0 << 4;
                    *py |= p1;
                }
            }           
        }      

        nbbp_predictor_t(uint8_t btb_depth /* n */, uint8_t gbh_width /*GBH width - l*/) : branch_predictor_t() {
            this->btb_depth = btb_depth;        
            this->gbh_width = gbh_width;        
            this->ctp_ltu = new ctp_ltu_t(btb_depth, gbh_width);
            auto numlines = (uint32_t)pow (2, btb_depth);
            this->py_ltu = new uint8_t[numlines];
        }

        virtual ~nbbp_predictor_t() {
            if (this->ctp_ltu) delete this->ctp_ltu;
        }        
}; 
