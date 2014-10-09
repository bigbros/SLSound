#ifndef CSLOggFile_h
#define CSLOggFile_h

#include "CSLSoundFile.h"
#include "ogg/ogg.h"
#include "vorbis/vorbisfile.h"

class CSLOggFile : public CSLSoundFile
{
private:
	static ov_callbacks		ms_callbacks;

	OggVorbis_File			m_ovFile;

public:
	static bool	init();
	static CSLSoundFile * create(const char * fileName);

private:
	CSLOggFile(const char * fileName);
public:
	virtual ~CSLOggFile();
	long readPacket(void * buf, long blocks);
	bool seekTop();

public:
	size_t	ovRead(void * ptr, size_t size, size_t nmemb);
	int		ovSeek(ogg_int64_t offset, int whence);
	long	ovTell();
private:
	enum {
		OV_PACKET	= 4096
	};
};


#endif // CSLOggFile_h
