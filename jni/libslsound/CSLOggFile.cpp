#include "CSLOggFile.h"

static CSLSoundFactory factory(".ogg", CSLOggFile::init, 0, CSLOggFile::create);
ov_callbacks	CSLOggFile::ms_callbacks;

extern "C" {

static size_t
ovread(void * ptr, size_t size, size_t nmemb, void * datasource)
{
	CSLOggFile * ogg = (CSLOggFile *)datasource;
	return ogg->ovRead(ptr, size, nmemb);
}

static int
ovseek(void * datasource, ogg_int64_t offset, int whence)
{
	CSLOggFile * ogg = (CSLOggFile *)datasource;
	return ogg->ovSeek(offset, whence);
}

static int
ovclose(void * datasource)
{
	return 0;
}

static long
ovtell(void * datasource)
{
	CSLOggFile * ogg = (CSLOggFile *)datasource;
	return ogg->ovTell();
}
} // extern "C"

bool
CSLOggFile::init()
{
	ms_callbacks.read_func	= ovread;
	ms_callbacks.seek_func	= ovseek;
	ms_callbacks.close_func	= ovclose;
	ms_callbacks.tell_func	= ovtell;

	return true;
}

CSLSoundFile *
CSLOggFile::create(const char * fileName)
{
	return new CSLOggFile(fileName);
}


CSLOggFile::CSLOggFile(const char * fileName)
: CSLSoundFile(fileName)
{
	int result = ov_open_callbacks(this, &m_ovFile, 0, 0, ms_callbacks);
	if(!result) {
		vorbis_info * vi = ov_info(&m_ovFile, -1);
		m_rate		= vi->rate;
		m_channels	= vi->channels;
		m_depth		= 2;
		m_block		= m_channels * m_depth;
		m_length	= ov_pcm_total(&m_ovFile, -1);
		m_packet	= OV_PACKET;
	} else {
		m_ready = false;
	}
}

CSLOggFile::~CSLOggFile()
{
}

long
CSLOggFile::readPacket(void * ptr, long blocks)
{
	int current_section;
	long size = ov_read(&m_ovFile, (char *)ptr, blocks * m_block, 0, m_depth, 1, &current_section);
	return size / m_block;
}

bool
CSLOggFile::seekTop()
{
	int result = ov_pcm_seek(&m_ovFile, 0L);
	return (!result);
}

size_t
CSLOggFile::ovRead(void * ptr, size_t size, size_t nmemb)
{
	return read(m_fd, ptr, size * nmemb);
}

int
CSLOggFile::ovSeek(ogg_int64_t offset, int whence)
{
	return lseek(m_fd, offset, whence);
}

long
CSLOggFile::ovTell()
{
	return lseek(m_fd, 0L, SEEK_CUR);
}
