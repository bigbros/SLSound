#ifndef CSLWavFile_h
#define CSLWavFile_h

#include "CSLSoundFile.h"

class CSLWavFile : public CSLSoundFile
{
private:
	enum {
		WAV_PACKET	=	4096
		// .wav はブロック境界単位であればどんなサイズで読んでも構わないが、
		// パケットサイズ単位で読み書きする場合の為に、4096byteを推奨パケットサイズとしておく。
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
