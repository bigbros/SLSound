#ifndef CSLMp3File_h
#define CSLMp3File_h

#include "CSLSoundFile.h"
#include "mp3lame/lame.h"

class CSLMp3File : public CSLSoundFile
{
private:
	static lame_global_flags	*	ms_lgf;

	hip_t							m_hip;

public:
	static bool init();
	static void exit();
	static CSLSoundFile * create(const char * fileName);
private:
	CSLMp3File(const char * fileName);
public:
	virtual ~CSLMp3File();
	long readPacket(void * buf, long blocks);
	bool seekTop();


private:
	bool	readMp3Header();
	bool	skipID3tag(unsigned char * pvBuffer, size_t * ret_nSize, size_t * ret_nPos);
	bool	readHeaders(unsigned char * pvBuffer, size_t * ret_nSize, size_t * ret_nPos);

	enum {
		FILE_BUFFER_SIZE	=	8192,
		LAME_BUFFER_MAX_FRAME	=	1152,
	};

	short		m_bufL[ LAME_BUFFER_MAX_FRAME ];
	short		m_bufR[ LAME_BUFFER_MAX_FRAME ];
};

#endif // CSLMp3File_h
