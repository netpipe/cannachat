/* (C) 2007 Jean-Marc Valin, CSIRO
*/
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   
   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   
   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   
   - Neither the name of the Xiph.org Foundation nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.
   
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cc6_laplace.h"

int cc6_ec_laplace_get_start_freq(int decay)
{
   int fs = (((cc6_ec_uint32)32768)*(16384-decay))/(16384+decay);
   /* Making fs even so we're sure that all the range is used for +/- values */
   fs -= (fs&1);
   return fs;
}

void cc6_ec_laplace_encode_start(cc6_ec_enc *enc, int *value, int decay, int fs)
{
   int i;
   int fl;
   unsigned int ft;
   int s = 0;
   int val = *value;
   if (val < 0)
   {
      s = 1;
      val = -val;
   }
   ft = 32768;
   fl = -fs;
   for (i=0;i<val;i++)
   {
      int tmp_l, tmp_s;
      tmp_l = fl;
      tmp_s = fs;
      fl += fs*2;
      fs = (fs*(cc6_ec_int32)decay)>>14;
      if (fs == 0)
      {
         if (fl+2 <= ft)
         {
            fs = 1;
         } else {
            fs = tmp_s;
            fl = tmp_l;
            if (s)
               *value = -i;
            else
               *value = i;
            break;
         }
      }
   }
   if (fl < 0)
      fl = 0;
   if (s)
      fl += fs;
   cc6_ec_encode(enc, fl, fl+fs, ft);
}

void cc6_ec_laplace_encode(cc6_ec_enc *enc, int *value, int decay)
{
   int fs = cc6_ec_laplace_get_start_freq(decay);
   cc6_ec_laplace_encode_start(enc, value, decay, fs);
}


int cc6_ec_laplace_decode_start(cc6_ec_dec *dec, int decay, int fs)
{
   int val=0;
   int fl, fh, fm;
   unsigned int ft;
   fl = 0;
   ft = 32768;
   fh = fs;
   fm = cc6_ec_decode(dec, ft);
   while (fm >= fh && fs != 0)
   {
      fl = fh;
      fs = (fs*(cc6_ec_int32)decay)>>14;
      if (fs == 0 && fh+2 <= ft)
      {
         fs = 1;
      }
      fh += fs*2;
      val++;
   }
   if (fl>0)
   {
      if (fm >= fl+fs)
      {
         val = -val;
         fl += fs;
      } else {
         fh -= fs;
      }
   }
   /* Preventing an infinite loop in case something screws up in the decoding */
   if (fl==fh)
      fl--;
   cc6_ec_dec_update(dec, fl, fh, ft);
   return val;
}

int cc6_ec_laplace_decode(cc6_ec_dec *dec, int decay)
{
   int fs = cc6_ec_laplace_get_start_freq(decay);
   return cc6_ec_laplace_decode_start(dec, decay, fs);
}
