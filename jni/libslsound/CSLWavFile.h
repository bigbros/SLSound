#ifndef CSLWavFile_h
#define CSLWavFile_h

#include "CSLSoundFile.h"

class CSLWavFile : public CSLSoundFile
{
private:
	enum {
		WAV_PACKET	=	4096
		// .wav は非圧縮リニアフォーマットで収録されているもののみをサポートする。
		// .mp3 のようなパケットの概念を持たないが、便宜的にこのサイズが返るようにする。
	};
public:
	static CSLSoundFile * create(const char * fileName);

private:
	CSLWavFile(const char * fileName);

public:
	virtual ~CSLWavFile();
	long readPacket(void * buf, long blocks);
	bool seekTop();

private:
	short	get_short(unsigned char * ptr);
	long	get_long(unsigned char * ptr);

	bool	read_chunk(const char * chunkID, long * chunkSize);
	bool	skip_chunk(const char * chunkID, long * chunkSize);
	bool	read_fmt();
	bool	find_data();
	bool	check_riff();

	bool	sound_setup();

private:
	long			m_pcm_top_pos;
};


#endif // CSLWavFile_h
