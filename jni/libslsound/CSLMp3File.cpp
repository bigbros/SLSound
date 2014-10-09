#include "CSLMp3File.h"

// ファクトリにエントリを追加させる。
static CSLSoundFactory factory(".mp3", CSLMp3File::init, CSLMp3File::exit, CSLMp3File::create);

lame_global_flags	*	CSLMp3File::ms_lgf = 0;

bool
CSLMp3File::init()
{
	ms_lgf = lame_init();
	return (ms_lgf != 0);
}

void
CSLMp3File::exit()
{
	lame_close(ms_lgf);
	ms_lgf = 0;
}

CSLSoundFile *
CSLMp3File::create(const char * fileName)
{
	return new CSLMp3File(fileName);
}

CSLMp3File::CSLMp3File(const char * fileName)
: CSLSoundFile(fileName)
{
	m_hip = hip_decode_init();
	if(m_hip) {
		m_ready = readMp3Header();
	} else {
		m_ready = false;
	}
}

CSLMp3File::~CSLMp3File()
{
	if(m_hip) hip_decode_exit(m_hip);
}

bool
CSLMp3File::readMp3Header()
{
	unsigned char pvBuffer[ FILE_BUFFER_SIZE ];
	size_t nSize, nPos;

	if(!skipID3tag(pvBuffer, &nSize, &nPos)) return false;
	if(!readHeaders(pvBuffer, &nSize, &nPos)) return false;


	return true;
}

bool
CSLMp3File::skipID3tag(unsigned char * pvBuffer, size_t * ret_nSize, size_t * ret_nPos)
{
	size_t nSize, nPos;
	size_t headerSize;

	nSize = read(m_fd, pvBuffer, FILE_BUFFER_SIZE);
	if(nSize < 4) {
		return false;
	}
	nPos = (size_t)-1;
	if(!memcmp(pvBuffer, "ID3", 3)) {
		if(nSize < 10) {
			nPos = read(m_fd, pvBuffer, 10 - nSize);
			if(nPos + nSize < 10) return false;
		}
		nSize += nPos;
	}
	headerSize = ((pvBuffer[6] & 0x7f) << 21) |
			((pvBuffer[7] & 0x7f) << 14) |
			((pvBuffer[8] & 0x7f) << 7) |
			(pvBuffer[9] & 0x7f);
	if(nSize - 10 >= headerSize) {
		nPos = 10 + headerSize;
		nSize -= nPos;
	} else {
		while(headerSize) {
			if(headerSize > FILE_BUFFER_SIZE) {
				nSize = FILE_BUFFER_SIZE;
			} else {
				nSize = headerSize;
			}
			nPos = read(m_fd, pvBuffer, nSize);
			if(!nPos) return false;
			headerSize -= nPos;
		}
		if(!headerSize) {
			nPos = 0;
			nSize = 0;
		}
	}
	*ret_nSize = nSize;
	*ret_nPos = nPos;
	return true;
}

bool
CSLMp3File::readHeaders(unsigned char * pvBuffer, size_t * ret_nSize, size_t * ret_nPos)
{
	int nSize = *ret_nSize;
	int nPos = *ret_nPos;
	int decodeCount, nEncDelay, nEncPadding;
	mp3data_struct mp3data;

	decodeCount = -1;
	nEncDelay = nEncPadding = -1;
	mp3data.header_parsed = 0;

	for(;;) {
		if(nSize) {
			decodeCount = hip_decode1_headersB(m_hip, pvBuffer + nPos, nSize,
				m_bufL, m_bufR, &mp3data, &nEncDelay, &nEncPadding);
			if(decodeCount < 0) return false;
			nPos = 0;
			if(mp3data.header_parsed) break;
		}
		nSize = read(m_fd, pvBuffer, FILE_BUFFER_SIZE);
		if(!nSize) return false;
	}
	*ret_nSize = nSize;
	*ret_nPos = nPos;
	return (decodeCount >= 0);
}

