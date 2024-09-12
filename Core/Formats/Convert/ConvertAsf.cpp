

#include <stdafx.h>
#pragma hdrstop
#include "ConvertAsf.h"
//#include "Main.h"
#include "Gui.h"

//=============================
//		ConvertAsf.cpp
//=============================



long Clip16BitSample(LONG sample)
{
  if (sample > 32767)
	 return 32767;
  else if (sample < -32768)
	 return -32768;
  else
	 return sample;
}


void AsfToPcm (FILE* in, FILE* out, u_int offset)
{

long lCurSampleLeft, lCurSampleRight, lPrevSampleLeft, lPrevSampleRight;

long EATable[]=
{
  0x00000000,
  0x000000F0,
  0x000001CC,
  0x00000188,
  0x00000000,
  0x00000000,
  0xFFFFFF30,
  0xFFFFFF24,
  0x00000000,
  0x00000001,
  0x00000003,
  0x00000004,
  0x00000007,
  0x00000008,
  0x0000000A,
  0x0000000B,
  0x00000000,
  0xFFFFFFFF,
  0xFFFFFFFD,
  0xFFFFFFFC
};


int blocks;
fseek(in, offset -8 , 0);
fread(&blocks, sizeof(int), 1, in);
blocks <<= 8;


for (int x = 0; x < blocks && !feof(in); ++x)
{

int blockSize;
fseek(in, offset + 4, 0);
fread(&blockSize, sizeof(int), 1, in);

int scdlHdrSize = 20;
int bufSize = blockSize - scdlHdrSize;
char* InputBuffer = (char*) malloc (bufSize);
fseek(in, offset + scdlHdrSize, 0);
fread(InputBuffer, bufSize, 1, in);


struct
{
  DWORD dwOutSize;
  SHORT lCurSampleLeft;
  SHORT lPrevSampleLeft;
  SHORT lCurSampleRight;
  SHORT lPrevSampleRight;
}ASFChunkHeader;


fseek(in, offset + 4, 0);
fread(&ASFChunkHeader, sizeof(ASFChunkHeader), 1, in);

lCurSampleLeft = ASFChunkHeader.lCurSampleLeft;
lCurSampleRight = ASFChunkHeader.lCurSampleRight;
lPrevSampleLeft = ASFChunkHeader.lPrevSampleLeft;
lPrevSampleRight = ASFChunkHeader.lPrevSampleRight;


//gui.Msg(ASFChunkHeader.dwOutSize);
//gui.Msg(ASFChunkHeader.lCurSampleLeft);

//gui.Msg(bufSize);
//return;


//BYTE InputBuffer[InputBufferSize]; // Buffer containing audio data of "SCDl" block

BYTE  bInput;
DWORD dwOutSize = ASFChunkHeader.dwOutSize; // Outsize value from the ASFChunkHeader
DWORD i, bCount, sCount;
LONG  c1left, c2left, c1right, c2right, left, right;
BYTE  dleft, dright;

DWORD dwSubOutSize = 0x1c;

i = 0;

// Process integral number of (dwSubOutSize) samples
for (bCount = 0; bCount < (dwOutSize/dwSubOutSize); ++bCount)
{

  bInput = InputBuffer[++i];
  c1left = EATable[HINIBBLE(bInput)];   	// Predictor coeffs for left channel
  c2left = EATable[HINIBBLE(bInput) + 4];
  c1right = EATable[LONIBBLE(bInput)];  	// Predictor coeffs for right channel
  c2right = EATable[LONIBBLE(bInput) + 4];
  bInput = InputBuffer[++i];
  dleft = HINIBBLE(bInput) + 8;   			// Shift value for left channel
  dright = LONIBBLE(bInput) + 8;  			// Shift value for right channel
  for (sCount=0;sCount<dwSubOutSize;sCount++)
  {
	bInput = InputBuffer[++i];
	left = HINIBBLE(bInput);  				// HIGHER nibble for left channel
	right = LONIBBLE(bInput);				// LOWER nibble for right channel
	left = (left << 0x1c) >> dleft;
	right = (right << 0x1c) >> dright;
	left = (left + lCurSampleLeft * c1left + lPrevSampleLeft * c2left + 0x80) >> 8;
	right = (right + lCurSampleRight * c1right + lPrevSampleRight * c2right + 0x80) >> 8;
	left = Clip16BitSample(left);
	right = Clip16BitSample(right);
	lPrevSampleLeft = lCurSampleLeft;
	lCurSampleLeft = left;
	lPrevSampleRight = lCurSampleRight;
	lCurSampleRight = right;

    // Now we've got lCurSampleLeft and lCurSampleRight which form one stereo
    // sample and all is set for the next input byte...
	//Output((SHORT)lCurSampleLeft,(SHORT)lCurSampleRight); // send the sample to output


	fwrite((short*)&lCurSampleLeft, sizeof((short)lCurSampleLeft), 1, out);
	fwrite((short*)&lCurSampleRight, sizeof((short)lCurSampleRight), 1, out);

	//gui.ProgressStep();
  }
}

// process the rest (if any)
if ((dwOutSize % dwSubOutSize) != 0)
{
  bInput = InputBuffer[++i];
  c1left = EATable[HINIBBLE(bInput)];   		// Predictor coeffs for left channel
  c2left = EATable[HINIBBLE(bInput) + 4];
  c1right = EATable[LONIBBLE(bInput)];  		// Predictor coeffs for right channel
  c2right = EATable[LONIBBLE(bInput) + 4];
  bInput = InputBuffer[++i];
  dleft = HINIBBLE(bInput) + 8;   				// Shift value for left channel
  dright = LONIBBLE(bInput) + 8;  				// Shift value for right channel
  for (sCount = 0; sCount < (dwOutSize % dwSubOutSize); ++sCount)
  {
	bInput = InputBuffer[++i];
	left = HINIBBLE(bInput);  				// HIGHER nibble for left channel
	right = LONIBBLE(bInput); 				// LOWER nibble for right channel
	left = (left << 0x1c) >> dleft;
	right = (right << 0x1c) >> dright;
	left = (left+lCurSampleLeft * c1left + lPrevSampleLeft * c2left+0x80)>>8;
	right = (right+lCurSampleRight * c1right + lPrevSampleRight * c2right+0x80)>>8;
	left = Clip16BitSample(left);
	right = Clip16BitSample(right);
	lPrevSampleLeft = lCurSampleLeft;
	lCurSampleLeft = left;
	lPrevSampleRight = lCurSampleRight;
	lCurSampleRight = right;

	// Now we've got lCurSampleLeft and lCurSampleRight which form one stereo
    // sample and all is set for the next input byte...
	//Output((SHORT)lCurSampleLeft,(SHORT)lCurSampleRight); // send the sample to output

	fwrite((short*)&lCurSampleLeft, sizeof((short)lCurSampleLeft), 1, out);
	fwrite((short*)&lCurSampleRight, sizeof((short)lCurSampleRight), 1, out);
  }
}

free(InputBuffer);

}// End while loop


}






//------------------------------------------------------------------------------

void AsfSplitToPcm (FILE* in, FILE* out, u_int offset)
{

long EATable[]=
{
  0x00000000,
  0x000000F0,
  0x000001CC,
  0x00000188,
  0x00000000,
  0x00000000,
  0xFFFFFF30,
  0xFFFFFF24,
  0x00000000,
  0x00000001,
  0x00000003,
  0x00000004,
  0x00000007,
  0x00000008,
  0x0000000A,
  0x0000000B,
  0x00000000,
  0xFFFFFFFF,
  0xFFFFFFFD,
  0xFFFFFFFC
};


int blocks;
fseek(in, offset -8 , 0);
fread(&blocks, sizeof(int), 1, in);
blocks <<= 8;


for (int x = 0; x < blocks && !feof(in); ++x)
{

int blockSize;
fseek(in, offset + 4, 0);
fread(&blockSize, sizeof(int), 1, in);

int scdlHdrSize = 20;
int bufSize = blockSize - scdlHdrSize;
char* InputBuffer = (char*) malloc (bufSize);
fseek(in, offset + scdlHdrSize, 0);
fread(InputBuffer, bufSize, 1, in);


struct
{
  DWORD dwOutSize;
  DWORD dwLeftChannelOffset;
  DWORD dwRightChannelOffset;
}ASFSplitChunkHeader;

//const int temp = 1;


//SHORT lCurSampleLeft;
//SHORT lPrevSampleLeft;
short lCurSample;
short lPrevSample;
//BYTE  bLeftChannelData[temp]; // compressed data for left channel goes here...
//short lCurSampleRight;
//short lPrevSampleRight;
//BYTE  bRightChannelData[temp]; // compressed data for right channel goes here...


fseek(in, offset + 4, 0);
fread(&ASFSplitChunkHeader, sizeof(ASFSplitChunkHeader), 1, in);


lCurSample = ASFSplitChunkHeader.dwLeftChannelOffset;
lPrevSample = ASFSplitChunkHeader.dwRightChannelOffset;



//gui.Msg(ASFChunkHeader.dwOutSize);
//gui.Msg(ASFChunkHeader.lCurSampleLeft);

//gui.Msg(bufSize);
//return;



BYTE  bInput;
DWORD dwOutSize = ASFSplitChunkHeader.dwOutSize; // Outsize value from the ASFChunkHeader
DWORD i, bCount, sCount;

LONG  c1, c2, left;
BYTE  d;

DWORD dwSubOutSize = 0x0E;

i = 0;

// Process integral number of (dwSubOutSize) samples
for (bCount = 0; bCount < (dwOutSize/dwSubOutSize); ++bCount)
{

  bInput = InputBuffer[++i];
  c1=EATable[HINIBBLE(bInput)];	// predictor coeffs
  c2=EATable[HINIBBLE(bInput)+4];
  d=LONIBBLE(bInput)+8;  // shift value

  for (sCount=0;sCount<dwSubOutSize;sCount++)
  {
	bInput = InputBuffer[++i];
	left = HINIBBLE(bInput);  				// HIGHER nibble for left channel
	//right = LONIBBLE(bInput);				// LOWER nibble for right channel

	left = (left << 0x1c) >> d;

	left = (left + lCurSample * c1 + lPrevSample * c2 + 0x80) >> 8;

	left = Clip16BitSample(left);

	lPrevSample = lCurSample;
	lCurSample = left;


	// Output
	fwrite(&lCurSample, sizeof(lCurSample), 1, out);
  }
}

// process the rest (if any)
if ((dwOutSize % dwSubOutSize) != 0)
{
  bInput=InputBuffer[++i];
  c1=EATable[HINIBBLE(bInput)];	// predictor coeffs
  c2=EATable[HINIBBLE(bInput)+4];
  d=LONIBBLE(bInput)+8;  // shift value

  for (sCount = 0; sCount < (dwOutSize % dwSubOutSize); ++sCount)
  {
	bInput = InputBuffer[++i];
	left = HINIBBLE(bInput);  				// HIGHER nibble for left channel
	//right = LONIBBLE(bInput); 				// LOWER nibble for right channel
	left = (left << 0x1c) >> d;

	left = (left + lCurSample * c1 + lPrevSample * c2 + 0x80)>>8;

	left = Clip16BitSample(left);

	lPrevSample = lCurSample;
	lCurSample = left;


	// Output
	fwrite(&lCurSample, sizeof(lCurSample), 1, out);
  }
}

free(InputBuffer);


}// End while loop


}


//------------------------------------------------------------------------------



void TestAsfToPcm (FILE* in, FILE* out, u_int offset)
{


int blocks;
fseek(in, offset -8 , 0);
fread(&blocks, sizeof(int), 1, in);
blocks <<= 8;


for (int x = 0; x < blocks && !feof(in); ++x)
{

int blockSize;
fseek(in, offset + 4, 0);
fread(&blockSize, sizeof(int), 1, in);

int scdlHdrSize = 20;
int bufSize = blockSize - scdlHdrSize;
char* InputBuffer = (char*) malloc (bufSize);
fseek(in, offset + scdlHdrSize, 0);
fread(InputBuffer, bufSize, 1, in);


struct
{
  DWORD dwOutSize;
  DWORD dwLeftChannelOffset;
  DWORD dwRightChannelOffset;
}ASFSplitChunkHeader;



fseek(in, offset + 4, 0);
fread(&ASFSplitChunkHeader, sizeof(ASFSplitChunkHeader), 1, in);


//adpcm_decode_frame(void *data, int *data_size,
//				uint8_t *buf, int buf_size)


void* dataSamples = 0;

//int frame =
adpcm_decode_frame(dataSamples, (int*)ASFSplitChunkHeader.dwOutSize,
								(uint8_t*)InputBuffer, bufSize);


//fwrite(&frame, sizeof(frame), 1, out);


gui.Msg("done");

free(InputBuffer);


}// End while loop


}


//------------------------------------------------------------------------------


static int adpcm_decode_frame(void *data, int *data_size,
				uint8_t *buf, unsigned int buf_size)
{

    //int n, m, channel, i;
    //int block_predictor[2];
    short *samples;
	uint8_t *src;


	/* EA ADPCM state variables */
    uint32_t samples_in_chunk;
    int32_t previous_left_sample, previous_right_sample;
    int32_t current_left_sample, current_right_sample;
    int32_t next_left_sample, next_right_sample;
    int32_t coeff1l, coeff2l, coeff1r, coeff2r;
    uint8_t shift_left, shift_right;
    unsigned int count1, count2;


	if (!buf_size) return 0;

	//samples = data;
	samples = (short*)data;
	src = buf;
	//gui.Msg(sizeof(buf));
	//gui.Msg(sizeof(src));


	switch(1) {

	//case CODEC_ID_ADPCM_EA:
	case 1:
		//samples_in_chunk = LE_32(src);

		samples_in_chunk = *src;
		if (samples_in_chunk >= ((buf_size - 12) * 2)) {
			src += buf_size;

			//gui.Msg(samples_in_chunk);
			//gui.Msg(buf_size));

			gui.Msg("break");
			break;
		}

		src += 4;
		current_left_sample = (int16_t)LE_16(src);
        src += 2;
        previous_left_sample = (int16_t)LE_16(src);
        src += 2;
        current_right_sample = (int16_t)LE_16(src);
        src += 2;
        previous_right_sample = (int16_t)LE_16(src);
		src += 2;


		for (count1 = 0; count1 < samples_in_chunk/28; count1++) {
			coeff1l = ea_adpcm_table[(*src >> 4) & 0x0F];
            coeff2l = ea_adpcm_table[((*src >> 4) & 0x0F) + 4];
            coeff1r = ea_adpcm_table[*src & 0x0F];
            coeff2r = ea_adpcm_table[(*src & 0x0F) + 4];
			src++;

            shift_left = ((*src >> 4) & 0x0F) + 8;
            shift_right = (*src & 0x0F) + 8;
			src++;


            for (count2 = 0; count2 < 28; count2++) {
				next_left_sample = (((*src & 0xF0) << 24) >> shift_left);
				next_right_sample = (((*src & 0x0F) << 28) >> shift_right);
                src++;


                next_left_sample = (next_left_sample + 
                    (current_left_sample * coeff1l) + 
                    (previous_left_sample * coeff2l) + 0x80) >> 8;
                next_right_sample = (next_right_sample + 
                    (current_right_sample * coeff1r) + 
					(previous_right_sample * coeff2r) + 0x80) >> 8;


				CLAMP_TO_SHORT(next_left_sample);
				CLAMP_TO_SHORT(next_right_sample);

                previous_left_sample = current_left_sample;
                current_left_sample = next_left_sample;
                previous_right_sample = current_right_sample;
				current_right_sample = next_right_sample;
				//gui.Msg("++");

				*samples++ = (unsigned short)current_left_sample;
				*samples++ = (unsigned short)current_right_sample;

				//gui.Msg("end");
			}
		}
		gui.Msg("break2");
		break;


	default:
		gui.Msg("default");
		return -1;
	}

	gui.Msg("rtn");
	*data_size = (uint8_t *)samples - (uint8_t *)data;
	return src - buf;
}














//	case CODEC_ID_ADPCM_MS:
//		if (avctx->block_align != 0 && buf_size > avctx->block_align)
//			buf_size = avctx->block_align;
//		n = buf_size - 7 * avctx->channels;
//		if (n < 0)
//			return -1;
//		block_predictor[0] = clip(*src++, 0, 7);
//		block_predictor[1] = 0;
//		if (st)
//			block_predictor[1] = clip(*src++, 0, 7);
//		c->status[0].idelta = (int16_t)((*src & 0xFF) | ((src[1] << 8) & 0xFF00));
//		src+=2;
//		if (st){
//			c->status[1].idelta = (int16_t)((*src & 0xFF) | ((src[1] << 8) & 0xFF00));
//			src+=2;
//		}
//		c->status[0].coeff1 = AdaptCoeff1[block_predictor[0]];
//		c->status[0].coeff2 = AdaptCoeff2[block_predictor[0]];
//		c->status[1].coeff1 = AdaptCoeff1[block_predictor[1]];
//		c->status[1].coeff2 = AdaptCoeff2[block_predictor[1]];
//
//		c->status[0].sample1 = ((*src & 0xFF) | ((src[1] << 8) & 0xFF00));
//		src+=2;
//		if (st) c->status[1].sample1 = ((*src & 0xFF) | ((src[1] << 8) & 0xFF00));
//        if (st) src+=2;
//        c->status[0].sample2 = ((*src & 0xFF) | ((src[1] << 8) & 0xFF00));
//        src+=2;
//        if (st) c->status[1].sample2 = ((*src & 0xFF) | ((src[1] << 8) & 0xFF00));
//		if (st) src+=2;
//
//        *samples++ = c->status[0].sample1;
//        if (st) *samples++ = c->status[1].sample1;
//        *samples++ = c->status[0].sample2;
//		if (st) *samples++ = c->status[1].sample2;
//        for(;n>0;n--) {
//            *samples++ = adpcm_ms_expand_nibble(&c->status[0], (src[0] >> 4) & 0x0F);
//            *samples++ = adpcm_ms_expand_nibble(&c->status[st], src[0] & 0x0F);
//            src ++;
//		}
//        break;
