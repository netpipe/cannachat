/* (C) 2007-2008 Jean-Marc Valin, CSIRO
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

#include "cc6_mathops.h"
#include "cc6_cwrs.h"
#include "cc6_vq.h"
#include "cc6_arch.h"
#include "cc6_os_support.h"

/** Takes the pitch vector and the decoded residual vector, computes the gain
    that will give ||p+g*y||=1 and mixes the residual with the pitch. */
static void cc6_mix_pitch_and_residual(int * __restrict iy, cc6_celt_norm_t * __restrict X, int N, int K, const cc6_celt_norm_t * __restrict P)
{
   int i;
   cc6_celt_word32_t Ryp, Ryy, Rpp;
   cc6_celt_word16_t ryp, ryy, rpp;
   cc6_celt_word32_t g;
   cc6_VARDECL(cc6_celt_norm_t, y);
#ifdef FIXED_POINT
   int yshift;
#endif
   cc6_SAVE_STACK;
#ifdef FIXED_POINT
   yshift = 13-cc6_celt_ilog2(K);
#endif
   cc6_ALLOC(y, N, cc6_celt_norm_t);

   Rpp = 0;
   i=0;
   do {
      Rpp = cc6_MAC16_16(Rpp,P[i],P[i]);
      y[i] = cc6_SHL16(iy[i],yshift);
   } while (++i < N);

   Ryp = 0;
   Ryy = 0;
   /* If this doesn't generate a dual MAC (on supported archs), fire the compiler guy */
   i=0;
   do {
      Ryp = cc6_MAC16_16(Ryp, y[i], P[i]);
      Ryy = cc6_MAC16_16(Ryy, y[i], y[i]);
   } while (++i < N);

   ryp = cc6_ROUND16(Ryp,14);
   ryy = cc6_ROUND16(Ryy,14);
   rpp = cc6_ROUND16(Rpp,14);
   /* g = (sqrt(Ryp^2 + Ryy - Rpp*Ryy)-Ryp)/Ryy */
   g = cc6_MULT16_32_Q15(cc6_celt_sqrt(cc6_MAC16_16(Ryy, ryp,ryp) - cc6_MULT16_16(ryy,rpp)) - ryp,
                     cc6_celt_rcp(cc6_SHR32(Ryy,9)));

   i=0;
   do 
      X[i] = cc6_ADD16(P[i], cc6_ROUND16(cc6_MULT16_16(y[i], g),11));
   while (++i < N);

   cc6_RESTORE_STACK;
}


void cc6_alg_quant(cc6_celt_norm_t *X, cc6_celt_mask_t *W, int N, int K, cc6_celt_norm_t *P, cc6_ec_enc *enc)
{
   cc6_VARDECL(cc6_celt_norm_t, y);
   cc6_VARDECL(int, iy);
   cc6_VARDECL(cc6_celt_word16_t, signx);
   int j, is;
   cc6_celt_word16_t s;
   int pulsesLeft;
   cc6_celt_word32_t sum;
   cc6_celt_word32_t xy, yy, yp;
   cc6_celt_word16_t Rpp;
   int N_1; /* Inverse of N, in Q14 format (even for float) */
#ifdef FIXED_POINT
   int yshift;
#endif
   cc6_SAVE_STACK;

#ifdef FIXED_POINT
   yshift = 13-cc6_celt_ilog2(K);
#endif

   cc6_ALLOC(y, N, cc6_celt_norm_t);
   cc6_ALLOC(iy, N, int);
   cc6_ALLOC(signx, N, cc6_celt_word16_t);
   N_1 = 512/N;

   sum = 0;
   j=0; do {
      X[j] -= P[j];
      if (X[j]>0)
         signx[j]=1;
      else {
         signx[j]=-1;
         X[j]=-X[j];
         P[j]=-P[j];
      }
      iy[j] = 0;
      y[j] = 0;
      sum = cc6_MAC16_16(sum, P[j],P[j]);
   } while (++j<N);
   Rpp = cc6_ROUND16(sum, cc6_NORM_SHIFT);

   cc6_celt_assert2(Rpp<=cc6_NORM_SCALING, "Rpp should never have a norm greater than unity");

   xy = yy = yp = 0;

   pulsesLeft = K;

   /* Do a pre-search by projecting on the pyramid */
   if (K > (N>>1))
   {
      cc6_celt_word16_t rcp;
      sum=0;
      j=0; do {
         sum += X[j];
      }  while (++j<N);

#ifdef FIXED_POINT
      if (sum <= K)
#else
      if (sum <= cc6_EPSILON)
#endif
      {
         X[0] = cc6_QCONST16(1.f,14);
         j=1; do
            X[j]=0;
         while (++j<N);
         sum = cc6_QCONST16(1.f,14);
      }
      /* Do we have sufficient accuracy here? */
      rcp = cc6_EXTRACT16(cc6_MULT16_32_Q16(K-1, cc6_celt_rcp(sum)));
      j=0; do {
#ifdef FIXED_POINT
         /* It's really important to round *towards zero* here */
         iy[j] = cc6_MULT16_16_Q15(X[j],rcp);
#else
         iy[j] = floor(rcp*X[j]);
#endif
         y[j] = cc6_SHL16(iy[j],yshift);
         yy = cc6_MAC16_16(yy, y[j],y[j]);
         xy = cc6_MAC16_16(xy, X[j],y[j]);
         yp += P[j]*y[j];
         y[j] *= 2;
         pulsesLeft -= iy[j];
      }  while (++j<N);
   }
   cc6_celt_assert2(pulsesLeft>=1, "Allocated too many pulses in the quick pass");

   while (pulsesLeft > 1)
   {
      int pulsesAtOnce=1;
      int best_id;
      cc6_celt_word16_t magnitude;
      cc6_celt_word32_t best_num = -cc6_VERY_LARGE16;
      cc6_celt_word16_t best_den = 0;
#ifdef FIXED_POINT
      int rshift;
#endif
      /* Decide on how many pulses to find at once */
      pulsesAtOnce = (pulsesLeft*N_1)>>9; /* pulsesLeft/N */
      if (pulsesAtOnce<1)
         pulsesAtOnce = 1;
#ifdef FIXED_POINT
      rshift = yshift+1+cc6_celt_ilog2(K-pulsesLeft+pulsesAtOnce);
#endif
      magnitude = cc6_SHL16(pulsesAtOnce, yshift);

      best_id = 0;
      /* The squared magnitude term gets added anyway, so we might as well 
         add it outside the loop */
      yy = cc6_MAC16_16(yy, magnitude,magnitude);
      /* Choose between fast and accurate strategy depending on where we are in the search */
         /* This should ensure that anything we can process will have a better score */
      j=0;
      do {
         cc6_celt_word16_t Rxy, Ryy;
         /* Select sign based on X[j] alone */
         s = magnitude;
         /* Temporary sums of the new pulse(s) */
         Rxy = cc6_EXTRACT16(cc6_SHR32(cc6_MAC16_16(xy, s,X[j]),rshift));
         /* We're multiplying y[j] by two so we don't have to do it here */
         Ryy = cc6_EXTRACT16(cc6_SHR32(cc6_MAC16_16(yy, s,y[j]),rshift));
            
            /* Approximate score: we maximise Rxy/sqrt(Ryy) (we're guaranteed that 
         Rxy is positive because the sign is pre-computed) */
         Rxy = cc6_MULT16_16_Q15(Rxy,Rxy);
            /* The idea is to check for num/den >= best_num/best_den, but that way
         we can do it without any division */
         /* OPT: Make sure to use conditional moves here */
         if (cc6_MULT16_16(best_den, Rxy) > cc6_MULT16_16(Ryy, best_num))
         {
            best_den = Ryy;
            best_num = Rxy;
            best_id = j;
         }
      } while (++j<N);
      
      j = best_id;
      is = pulsesAtOnce;
      s = cc6_SHL16(is, yshift);

      /* Updating the sums of the new pulse(s) */
      xy = xy + cc6_MULT16_16(s,X[j]);
      /* We're multiplying y[j] by two so we don't have to do it here */
      yy = yy + cc6_MULT16_16(s,y[j]);
      yp = yp + cc6_MULT16_16(s, P[j]);

      /* Only now that we've made the final choice, update y/iy */
      /* Multiplying y[j] by 2 so we don't have to do it everywhere else */
      y[j] += 2*s;
      iy[j] += is;
      pulsesLeft -= pulsesAtOnce;
   }
   
   if (pulsesLeft > 0)
   {
      cc6_celt_word16_t g;
      cc6_celt_word16_t best_num = -cc6_VERY_LARGE16;
      cc6_celt_word16_t best_den = 0;
      int best_id = 0;
      cc6_celt_word16_t magnitude = cc6_SHL16(1, yshift);

      /* The squared magnitude term gets added anyway, so we might as well 
      add it outside the loop */
      yy = cc6_MAC16_16(yy, magnitude,magnitude);
      j=0;
      do {
         cc6_celt_word16_t Rxy, Ryy, Ryp;
         cc6_celt_word16_t num;
         /* Select sign based on X[j] alone */
         s = magnitude;
         /* Temporary sums of the new pulse(s) */
         Rxy = cc6_ROUND16(cc6_MAC16_16(xy, s,X[j]), 14);
         /* We're multiplying y[j] by two so we don't have to do it here */
         Ryy = cc6_ROUND16(cc6_MAC16_16(yy, s,y[j]), 14);
         Ryp = cc6_ROUND16(cc6_MAC16_16(yp, s,P[j]), 14);

            /* Compute the gain such that ||p + g*y|| = 1 
         ...but instead, we compute g*Ryy to avoid dividing */
         g = cc6_celt_psqrt(cc6_MULT16_16(Ryp,Ryp) + cc6_MULT16_16(Ryy,cc6_QCONST16(1.f,14)-Rpp)) - Ryp;
            /* Knowing that gain, what's the error: (x-g*y)^2 
         (result is negated and we discard x^2 because it's constant) */
         /* score = 2*g*Rxy - g*g*Ryy;*/
#ifdef FIXED_POINT
         /* No need to multiply Rxy by 2 because we did it earlier */
         num = cc6_MULT16_16_Q15(cc6_ADD16(cc6_SUB16(Rxy,g),Rxy),g);
#else
         num = g*(2*Rxy-g);
#endif
         if (cc6_MULT16_16(best_den, num) > cc6_MULT16_16(Ryy, best_num))
         {
            best_den = Ryy;
            best_num = num;
            best_id = j;
         }
      } while (++j<N);
      iy[best_id] += 1;
   }
   j=0;
   do {
      P[j] = cc6_MULT16_16(signx[j],P[j]);
      X[j] = cc6_MULT16_16(signx[j],X[j]);
      if (signx[j] < 0)
         iy[j] = -iy[j];
   } while (++j<N);
   cc6_encode_pulses(iy, N, K, enc);
   
   /* Recompute the gain in one pass to reduce the encoder-decoder mismatch
   due to the recursive computation used in quantisation. */
   cc6_mix_pitch_and_residual(iy, X, N, K, P);
   cc6_RESTORE_STACK;
}


/** Decode pulse vector and combine the result with the pitch vector to produce
    the final normalised signal in the current band. */
void cc6_alg_unquant(cc6_celt_norm_t *X, int N, int K, cc6_celt_norm_t *P, cc6_ec_dec *dec)
{
   cc6_VARDECL(int, iy);
   cc6_SAVE_STACK;
   cc6_ALLOC(iy, N, int);
   cc6_decode_pulses(iy, N, K, dec);
   cc6_mix_pitch_and_residual(iy, X, N, K, P);
   cc6_RESTORE_STACK;
}

cc6_celt_word16_t cc6_renormalise_vector(cc6_celt_norm_t *X, cc6_celt_word16_t value, int N, int stride)
{
   int i;
   cc6_celt_word32_t E = cc6_EPSILON;
   cc6_celt_word16_t rE;
   cc6_celt_word16_t g;
   cc6_celt_norm_t *xptr = X;
   for (i=0;i<N;i++)
   {
      E = cc6_MAC16_16(E, *xptr, *xptr);
      xptr += stride;
   }

   rE = cc6_celt_sqrt(E);
#ifdef FIXED_POINT
   if (rE <= 128)
      g = cc6_Q15ONE;
   else
#endif
      g = cc6_MULT16_16_Q15(value,cc6_celt_rcp(cc6_SHL32(rE,9)));
   xptr = X;
   for (i=0;i<N;i++)
   {
      *xptr = cc6_PSHR32(cc6_MULT16_16(g, *xptr),8);
      xptr += stride;
   }
   return rE;
}

static void cc6_fold(const cc6_CELTMode *m, int N, cc6_celt_norm_t *Y, cc6_celt_norm_t * __restrict P, int N0, int B)
{
   int j;
   const int C = cc6_CHANNELS(m);
   int id = (N0*C) % (C*B);
   /* Here, we assume that id will never be greater than N0, i.e. that 
      no band is wider than N0. In the unlikely case it happens, we set
      everything to zero */
   /*{
	   int offset = (N0*C - (id+C*N))/2;
	   if (offset > C*N0/16)
		   offset = C*N0/16;
	   offset -= offset % (C*B);
	   if (offset < 0)
		   offset = 0;
	   //printf ("%d\n", offset);
	   id += offset;
   }*/
   if (id+C*N>N0*C)
      for (j=0;j<C*N;j++)
         P[j] = 0;
   else
      for (j=0;j<C*N;j++)
         P[j] = Y[id++];
}

void cc6_intra_fold(const cc6_CELTMode *m, cc6_celt_norm_t * __restrict x, int N, int *pulses, cc6_celt_norm_t *Y, cc6_celt_norm_t * __restrict P, int N0, int B)
{
   int c;
   cc6_celt_word16_t pred_gain;
   const int C = cc6_CHANNELS(m);

   cc6_fold(m, N, Y, P, N0, B);
   c=0;
   do {
      int K = pulses[c];
      if (K==0)
         pred_gain = cc6_Q15ONE;
      else
         pred_gain = cc6_celt_div((cc6_celt_word32_t)cc6_MULT16_16(cc6_Q15_ONE,N),(cc6_celt_word32_t)(N+2*K*(K+1)));

      cc6_renormalise_vector(P+c, pred_gain, N, C);
   } while (++c < C);
}

